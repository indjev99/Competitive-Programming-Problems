#include "subway.h"
#include <iostream>
#include <vector>
using namespace std;
vector<int> cnt_in, cnt_out;
vector<vector<int> >masks;
vector<int>current_choice;
vector<int>active;
bool flag = false;
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
    active.resize(m);
    current_choice.resize(m);
    for(int i=0;i<m;i++)active[i]=1;
    vector<vector<int> >adj(n, vector<int>(n));
    vector<vector<vector<int> > > edge_ids(n, vector<vector<int> >(n));
    for(int i=0;i<m;i++)
    {
        adj[a[i]][b[i]]++;
        edge_ids[a[i]][b[i]].push_back(i);
    }
    // need to generate all possible assignments among the m edges to the d companies.
    // generate masks one by one
    // will have m!/((n!)^d d!) assignments, check each in time O(m)
    // for n=5, m = 20 have 4 8886 4376
    // for n=4, m=16 have 2 627 625 seems fine
    // for n=2, m=16 have 2 027 025 seems fine
    // for n=3, m=15 have 1 401 400 seems fine
    // so n,m <= 16 works as a contraint for this
    // equivalent to choosing d masks each having n bits from n, 2n, 3n, etc..
    vector<pair<int,int> > possible_edges;
    for(int id=0;id<m/n;id++)
    {
        //cout<<"id: "<<id<<endl;
        int good_mask=-1;
        possible_edges.clear();
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(adj[i][j]>0)possible_edges.push_back({i,j});
            }
        }
        //cout<<"possible edges: "<<possible_edges.size()<<endl;
        for(int mask=0;mask <= (1<<int(possible_edges.size()));mask++)
        {
            if(__builtin_popcount(mask)!=n)continue;
            for(int i=0;i<n;i++)
            {
                cnt_in[i]=0;cnt_out[i]=0;
            }
            for(int j=0;j<possible_edges.size();j++)
            {
                if(mask&(1<<j))
                {
                    int u=possible_edges[j].first, v=possible_edges[j].second;
                    cnt_out[u]++;cnt_in[v]++;
                }
            }
            bool flag=1;
            for(int i=0;i<n;i++)
            {
                if(cnt_in[i]!=1 || cnt_out[i]!=1)flag=0;
            }
            if(flag==1)
            {
                good_mask=mask;break;
            }
        }
        //cout<<"good_mask: "<<good_mask<<endl;
        vector<int> assignment_here;
        for(int i=0;i<possible_edges.size();i++)
        {
            if(good_mask&(1<<i))
            {
                assignment_here.push_back(edge_ids[possible_edges[i].first][possible_edges[i].second].back());
                edge_ids[possible_edges[i].first][possible_edges[i].second].pop_back();
                adj[possible_edges[i].first][possible_edges[i].second]--;
            }
        }
        answer(id, assignment_here);
    }
    return 1;
}