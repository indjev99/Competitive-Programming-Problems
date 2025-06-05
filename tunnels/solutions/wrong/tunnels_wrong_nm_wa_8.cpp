#include "tunnels.h"

struct Node
{
    int cost;

    int l, r;

    Node* leftChild;
    Node* rightChild;
};

void solve(int n, int m, int k, const std::vector<std::vector<bool>>& walls)
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
        std::vector<Node*> stack;

        auto mergeBack = [&]()
        {
            Node* rightChild = stack.back();
            stack.pop_back();

            Node* leftChild = stack.back();
            stack.pop_back();

            Node* node = &nodes[numNodes++];

            node->cost = std::max(leftChild->cost, rightChild->cost) + 1;
            node->l = leftChild->l;
            node->r = rightChild->r;
            node->leftChild = leftChild;
            node->rightChild = rightChild;

            stack.push_back(node);
        };

        for (int y = minY; y <= maxY; y++)
        {
            int newCost = dpCost[x + 1][y];
            if (newCost == -1) continue;

            bool merged = false;
            while (stack.size() >= 2 && stack[stack.size() - 2]->cost <= newCost)
            {
                merged = true;
                mergeBack();
            }

            if (merged)
            {
                while (stack.size() >= 2 && stack[stack.size() - 2]->cost <= stack[stack.size() - 1]->cost)
                {
                    mergeBack();
                }
            }

            Node* node = &nodes[numNodes++];

            node->cost = newCost;
            node->l = y;
            node->r = y;
            node->leftChild = nullptr;
            node->rightChild = nullptr;

            stack.push_back(node);

            if (stack.size() >= 2 && stack[stack.size() - 2]->cost <= stack[stack.size() - 1]->cost)
            {
                mergeBack();
            }
        }

        while (stack.size() >= 2)
        {
            mergeBack();
        }

        Node* root = nullptr;
        if (!stack.empty())
        {
            root = stack.back();
            stack.pop_back();
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
            if (endY == m || walls[x][endY])
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
        while (min > 0 && !walls[x][min - 1]) min--;

        int max = y;
        while (max < m - 1 && !walls[x][max + 1]) max++;

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
