#include "subway.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include <map>
#include <utility> // For std::pair

// Definition of the Edge structure
struct Edge {
    int u;  // Node in the left set (1 to n)
    int v;  // Node in the right set (1 to n)
    int id; // Original ID of the edge
    Edge(int u_val, int v_val, int id_val) : u(u_val), v(v_val), id(id_val) {}
};
// Helper function for Kuhn's algorithm (DFS part)
// u_left: current left node to find a match for
// adj: adjacency list for left nodes (adj[u_left] = list of right_node neighbors)
// matchR: matchR[v_right] stores the u_left it's matched with, or -1 if unmatched.
// visited_dfs: keeps track of visited left nodes in the current DFS traversal to avoid cycles.
bool kuhn_dfs_helper(int u_left,
                     const std::vector<std::vector<int>>& adj,
                     std::vector<int>& matchR,
                     std::vector<bool>& visited_dfs) {
    // Mark current left node as visited in this DFS path
    visited_dfs[u_left] = true;

    // Try to match u_left with its neighbors on the right side
    for (int v_right : adj[u_left]) {
        // If v_right is unmatched, or its current match (matchR[v_right])
        // can find an alternative match (recursive call to dfs)
        if (matchR[v_right] == -1 || (!visited_dfs[matchR[v_right]] && kuhn_dfs_helper(matchR[v_right], adj, matchR, visited_dfs))) {
            matchR[v_right] = u_left; // Match u_left with v_right
            return true; // Augmenting path found
        }
    }
    return false; // No augmenting path found starting from u_left through its direct neighbors
}

// Function to find a perfect matching in a bipartite graph
// n: number of nodes on each side (nodes are numbered 1 to n)
// allEdges: vector of all available edges in the graph
// Returns a vector of IDs of edges in the perfect matching, or an empty vector if no perfect matching exists.
std::vector<int> findPerfectMatching(int n, const std::vector<Edge>& allEdges) {
    if (n == 0) {
        return {}; // No nodes, empty matching is perfect.
    }

    // Adjacency list: adj[u_left] stores a vector of v_right neighbors.
    // Nodes are 1-indexed, so size is n+1.
    std::vector<std::vector<int>> adj(n + 1);

    // Map to quickly find an edge ID for a (u,v) pair.
    // This stores the ID of the *first* encountered edge for a given (u,v) pair from input.
    std::map<std::pair<int, int>, int> first_edge_id_for_pair;

    for (const auto& edge : allEdges) {
        // Ensure edges are within the valid node range [0, n-1]
        if (edge.u >= 0 && edge.u < n && edge.v >= 0 && edge.v < n) {
            adj[edge.u].push_back(edge.v);
            // Store the ID of the first edge found for this (u,v) pair
            if (first_edge_id_for_pair.find({edge.u, edge.v}) == first_edge_id_for_pair.end()) {
                first_edge_id_for_pair[{edge.u, edge.v}] = edge.id;
            }
        }
    }

    // matchR[v_right] stores the u_left it's matched with, or -1 if unmatched.
    // Sized n+1 for 1-based indexing of right nodes.
    std::vector<int> matchR(n + 1, -1);
    int matches_count = 0;

    // Iterate through all left nodes (1 to n) to find matches
    for (int u_left_node = 0; u_left_node < n; ++u_left_node) {
        // visited_dfs must be reset for each augmenting path search starting from a new u_left_node.
        // This vector tracks visited left nodes within the *current* DFS call.
        std::vector<bool> visited_dfs(n + 1, false);
        if (kuhn_dfs_helper(u_left_node, adj, matchR, visited_dfs)) {
            matches_count++;
        }
    }

    std::vector<int> result_edge_ids;
    // A perfect matching exists if the number of matches equals n
    if (matches_count == n) {
        result_edge_ids.reserve(n);
        // Reconstruct the edge IDs from the matching
        for (int v_right_node = 0; v_right_node < n; ++v_right_node) {
            if (matchR[v_right_node] != -1) { // If right node v_right_node is matched
                int u_left_node = matchR[v_right_node]; // Get the left node it's matched with

                // Find the ID of an edge connecting u_left_node and v_right_node
                // using the pre-populated map.
                auto it = first_edge_id_for_pair.find({u_left_node, v_right_node});
                if (it != first_edge_id_for_pair.end()) {
                    result_edge_ids.push_back(it->second);
                } else {
                    // This state should ideally not be reached if inputs are consistent.
                    // It means Kuhn's algorithm found a match (u,v) for which
                    // no original edge ID was stored. This could indicate an issue if
                    // adj was built with edges not in first_edge_id_for_pair or vice-versa.
                    // For robustness, one might clear result_edge_ids and return empty.
                    // Or throw an exception.
                    // Assuming valid graph construction, an ID will always be found.
                }
            }
        }
        // Ensure that we actually collected n edge IDs. This should be true if matchR is correctly populated.
        if (result_edge_ids.size() != n) {
             // Something went wrong in reconstructing IDs, not a valid perfect matching.
            return {};
        }
    }

    // If matches_count != n, no perfect matching was found,
    // result_edge_ids will be empty and returned.
    return result_edge_ids;
}

vector<int> cnt_in, cnt_out;
bool assign_roads(int n, int m, std::vector<int> a, std::vector<int> b)
{
    cnt_in.resize(n+1);
    int d=m/n;
    cnt_out.resize(n+1);
    for(int i=0;i<n;i++)cnt_in[i]=0;
    for(int i=0;i<n;i++)cnt_out[i]=0;
    for(int i=0;i<m;i++)
    {
        cnt_out[a[i]]++;
        cnt_in[b[i]]++;
    }
    for(int i=0;i<n;i++)
    {
        if(cnt_in[i]!=d)return 0;
        if(cnt_out[i]!=d)return 0;
    }
    vector<Edge> active;
    vector<Edge> aux;
    vector<int> roads_assigned;
    for(int i=0;i<m;i++)
    {
        active.push_back(Edge(a[i],b[i], i));
    }
    for(int i=0;i<d;i++)
    {
        roads_assigned = findPerfectMatching(n, active);
        answer(i, roads_assigned);
        sort(roads_assigned.begin(),roads_assigned.end());
        int ptr=0;
        aux.clear();
        for(int j=0;j<active.size();j++)
        {
            if(ptr < roads_assigned.size() && active[j].id == roads_assigned[ptr])
            {
                ptr++;
                continue;
            }
            aux.push_back(active[j]);
        }
        active=aux;
    }
    return 1;
}