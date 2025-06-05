#include "tunnels.h"

struct Node
{
    int cost;

    int l, r;

    Node* leftChild;
    Node* rightChild;
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

    std::vector<Node*> stack;

    auto solveRow = [&](int x, int minY)
    {
        stack.clear();

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

        int y = minY;

        for (; y < m; y++)
        {
            if (blocked[x][y]) break;

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

                while (stack.size() >= 2 && stack[stack.size() - 2]->cost == stack[stack.size() - 1]->cost)
                {
                    mergeBack();
                }
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
        for (int y2 = minY; y2 < y; y2++)
        {
            dpCost[x][y2] = cost;
        }

        return root;
    };

    std::vector<int> stackOpt;

    auto solveRowOpt = [&](int x, int minY)
    {
        stackOpt.clear();

        auto mergeBack = [&]()
        {
            int left = stackOpt.back();
            stackOpt.pop_back();

            int right = stackOpt.back();
            stackOpt.pop_back();

            stackOpt.push_back(std::max(left, right) + 1);
        };

        int y = minY;

        for (; y < m; y++)
        {
            if (blocked[x][y]) break;

            int newCost = dpCost[x + 1][y];
            if (newCost == -1) continue;

            bool merged = false;
            while (stackOpt.size() >= 2 && stackOpt[stackOpt.size() - 2] <= newCost)
            {
                merged = true;
                mergeBack();
            }

            if (merged)
            {
                while (stackOpt.size() >= 2 && stackOpt[stackOpt.size() - 2] <= stackOpt[stackOpt.size() - 1])
                {
                    mergeBack();
                }
            }

            stackOpt.push_back(newCost);

            if (stackOpt.size() >= 2 && stackOpt[stackOpt.size() - 2] <= stackOpt[stackOpt.size() - 1])
            {
                mergeBack();

                while (stackOpt.size() >= 2 && stackOpt[stackOpt.size() - 2] == stackOpt[stackOpt.size() - 1])
                {
                    mergeBack();
                }
            }
        }

        while (stackOpt.size() >= 2)
        {
            mergeBack();
        }

        int cost = -1;
        if (!stackOpt.empty())
        {
            cost = stackOpt.back();
            stackOpt.pop_back();
        }

        for (int y2 = minY; y2 < y; y2++)
        {
            dpCost[x][y2] = cost;
        }

        return y;
    };

    for (int x = n - 1; x >= 0; x--)
    {
        int y = 0;
        while (y < m && blocked[x][y])
        {
            y++;
        }
        while (y < m)
        {
            numNodes = 0;
            y = solveRowOpt(x, y);
            while (y < m && blocked[x][y])
            {
                y++;
            }
        }
    }

    int x = 0;
    int y = k;

    while (x < n)
    {
        int min = y;
        while (min > 0 && !blocked[x][min - 1]) min--;

        numNodes = 0;
        Node* node = solveRow(x, min);

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
