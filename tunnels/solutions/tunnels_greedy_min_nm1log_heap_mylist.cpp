#include "tunnels.h"

#include <queue>
#include <tuple>

struct Node
{
    int cost;

    int l, r;

    Node* leftChild;
    Node* rightChild;
};

template <class T>
struct List
{
    struct Node
    {
        T val;

        Node* next;
        Node* prev;

        bool valid;
    };

    List()
    {
        _size = 0;
        
        nodes.emplace_back();
        root = &nodes.back();

        root->val = T();

        root->next = root;
        root->prev = root;

        root->valid = false;
    }

    Node* begin()
    {
        return root->next;
    }

    Node* end()
    {
        return root;
    }

    void insert(Node* it, T val)
    {
        _size++;

        nodes.emplace_back();
        Node* newIt = &nodes.back();

        newIt->val = val;

        newIt->next = it->next;
        newIt->prev = it;

        it->next = newIt;
        newIt->next->prev = newIt;

        newIt->valid = true;
    }

    void erase(Node* it)
    {
        _size--;

        it->next->prev = it->prev;
        it->prev->next = it->next;

        it->next = nullptr;
        it->prev = nullptr;

        it->valid = false;
    }

    int size()
    {
        return _size;
    }

    int _size;
    Node* root;
    std::deque<Node> nodes;
};

void solve(int n, int m, int k, const std::vector<std::vector<bool>>& blocked)
{
    std::vector<std::vector<int>> dpCost(
        n + 1, std::vector<int>(m, -1)
    );

    for (int y = 0; y < m; y++)
    {
        dpCost[n][y] = 0;
    }

    int numNodes = 0;
    std::vector<Node> nodes(2 * m);

    auto solveRow = [&](int x, int minY, int maxY)
    {
        List<Node*> list;

        using Iter = List<Node*>::Node*;

        auto mergeIt = [&](Iter it)
        {
            Node* rightChild = it->next->val;
            list.erase(it->next);

            Node* leftChild = it->val;

            Node* node = &nodes[numNodes++];

            node->cost = std::max(leftChild->cost, rightChild->cost) + 1;
            node->l = leftChild->l;
            node->r = rightChild->r;
            node->leftChild = leftChild;
            node->rightChild = rightChild;

            it->val = node;
        };

        for (int y = minY; y <= maxY; y++)
        {
            int newCost = dpCost[x + 1][y];
            if (newCost == -1) continue;

            Node* node = &nodes[numNodes++];

            node->cost = newCost;
            node->l = y;
            node->r = y;
            node->leftChild = nullptr;
            node->rightChild = nullptr;

            list.insert(list.end()->prev, node);
        }

        auto cmp = [](const std::tuple<int, int, Iter>& left, const std::tuple<int, int, Iter>& right)
        {
            if (std::get<0>(left) != std::get<0>(right)) return std::get<0>(left) > std::get<0>(right);
            return std::get<1>(left) > std::get<1>(right);
        };

        std::priority_queue<std::tuple<int, int, Iter>, std::vector<std::tuple<int, int, Iter>>, decltype(cmp)> pq(cmp);

        for (Iter it = list.begin(); it->next != list.end(); it = it->next)
        {
            int cost = 1 + std::max(it->val->cost, it->next->val->cost);
            pq.push({cost, it->val->l, it});
        }

        while (list.size() > 1)
        {
            auto [cost, l, it] = pq.top();
            pq.pop();

            if (!it->valid || !it->next->valid) continue;

            int reCost = 1 + std::max(it->val->cost, it->next->val->cost);

            if (reCost != cost) continue;

            mergeIt(it);

            if (it->next != list.end())
            {
                int newCost = 1 + std::max(it->val->cost, it->next->val->cost);
                pq.push({newCost, l, it});
            }

            if (it != list.begin())
            {
                int newCost = 1 + std::max(it->prev->val->cost, it->val->cost);
                pq.push({newCost, l, it->prev});
            }
        }

        Node* root = nullptr;
        if (list.size() > 0)
        {
            root = list.begin()->val;
            list.erase(list.begin());
        }

        int cost = root ? root->cost : -1;
        for (int y = minY; y <= maxY; y++)
        {
            dpCost[x][y] = cost;
        }

        return root;
    };

    for (int x = n - 1; x >= 0; x--)
    {
        int startY = -1;
        for (int endY = 0; endY <= m; endY++)
        {
            if (endY == m || blocked[x][endY])
            {
                if (startY + 1 < endY)
                {
                    numNodes = 0;
                    solveRow(x, startY + 1, endY - 1);
                }
                startY = endY;
            }
        }
    }

    int x = 0;
    int y = k;

    while (x < n)
    {
        int min = y;
        while (min > 0 && !blocked[x][min - 1]) min--;

        int max = y;
        while (max < m - 1 && !blocked[x][max + 1]) max++;

        numNodes = 0;
        Node* node = solveRow(x, min, max);

        while (node->l < node->r)
        {
            int mid = node->leftChild->r;

            if (mid >= y)
            {
                if (!investigate(mid + 1)) node = node->leftChild;
                else node = node->rightChild;
            }
            else
            {
                if (!investigate(mid)) node = node->rightChild;
                else node = node->leftChild;
            }
        }

        x++;
        y = node->l;

        goDeeper(y);
    }
}
