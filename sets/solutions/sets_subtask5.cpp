#include <iostream>

const int MAX_N = 2e6;
const int MAX_TREE = 1 << 22;
const int INF = 2e8;

int n;
int x[MAX_N], y[MAX_N];

int ans;

void input()
{
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> x[i] >> y[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

// prefY[i] = Y[0, i)
int prefMinY[MAX_N + 1], prefMaxY[MAX_N + 1];

// suffY[i] = Y[i, n)
int suffMinY[MAX_N + 1], suffMaxY[MAX_N + 1];

// leftNone[i] = -X[i]
// leftMin[i] = -X[i] - min prefY[i]
// leftMax[i] = -X[i] + max prefY[i]
// leftMinMax[i] = -X[i] + max prefY[i] - min prefY[i]

#define NONE 0
#define MIN 1
#define MAX 2
#define MINMAX 3

template<int type>
inline int getVal(int i)
{
    if constexpr (type == NONE) return -x[i];
    if constexpr (type == MIN) return -x[i] - prefMinY[i];
    if constexpr (type == MAX) return -x[i] + prefMaxY[i];
    if constexpr (type == MINMAX) return -x[i] + prefMaxY[i] - prefMinY[i];
}

template<int type>
void buildTreeUtil(int *tree, int curr, int cl, int cr)
{
    if (cr - cl == 1)
    {
        tree[curr] = getVal<type>(cl);
        return;
    }

    int cm = (cl + cr) / 2;
    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    buildTreeUtil<type>(tree, left, cl, cm);
    buildTreeUtil<type>(tree, right, cm, cr);
    tree[curr] = std::min(tree[left], tree[right]);
}

int queryTreeUtil(int *tree, int curr, int cl, int cr, int ql, int qr)
{
    if (qr <= cl || cr <= ql) return INF;
    if (ql <= cl && cr <= qr) return tree[curr];

    int cm = (cl + cr) / 2;
    int left = curr * 2 + 1;
    int right = curr * 2 + 2;
    return std::min(queryTreeUtil(tree, left, cl, cm, ql, qr), queryTreeUtil(tree, right, cm, cr, ql, qr));
}

template<int type>
void buildTree(int *tree)
{
    buildTreeUtil<type>(tree, 0, 0, n + 1);
}

int queryTree(int *tree, int ql, int qr)
{
    if (qr <= ql) return INF;
    return queryTreeUtil(tree, 0, 0, n + 1, ql, qr);
}

//int leftNoneTree[MAX_TREE];
int leftMinTree[MAX_TREE];
int leftMaxTree[MAX_TREE];
int leftMinMaxTree[MAX_TREE];

template<int type>
int findChangePos(int r)
{
    int l = 0;
    int rest = (type == MIN) ? suffMinY[r] : suffMaxY[r];
    while (r - l > 1)
    {
        int mid = (l + r + 1) / 2;
        if constexpr (type == MIN)
        {
            if (prefMinY[mid] <= rest) r = mid;
            else l = mid;
        }
        else
        {
            if (prefMaxY[mid] >= rest) r = mid;
            else l = mid;
        }
    }
    return r;
}

void solve()
{
    prefMinY[0] = INF;
    prefMaxY[0] = -INF;
    for (int i = 1; i <= n; ++i)
    {
        prefMinY[i] = std::min(prefMinY[i - 1], y[i - 1]);
        prefMaxY[i] = std::max(prefMaxY[i - 1], y[i - 1]);
    }

    suffMinY[n] = INF;
    suffMaxY[n] = -INF;
    for (int i = n - 1; i >= 0; --i)
    {
        suffMinY[i] = std::min(suffMinY[i + 1], y[i]);
        suffMaxY[i] = std::max(suffMaxY[i + 1], y[i]);
    }

    //buildTree<NONE>(leftNoneTree);
    buildTree<MIN>(leftMinTree);
    buildTree<MAX>(leftMaxTree);
    buildTree<MINMAX>(leftMinMaxTree);

    ans = x[n - 1] + queryTree(leftMinMaxTree, 1, n);
    for (int r = 1; r < n; ++r)
    {
        int minChangePos = findChangePos<MIN>(r);
        int maxChangePos = findChangePos<MAX>(r);

        //int currAnsNone = x[r - 1] + queryTree(leftNoneTree, 0, std::min(minChangePos, maxChangePos)) + suffMaxY[r] - suffMinY[r];
        int currAnsNone = x[r - 1] - x[std::min(minChangePos, maxChangePos) - 1] + suffMaxY[r] - suffMinY[r];
        int currAnsMin = x[r - 1] + queryTree(leftMinTree, minChangePos, maxChangePos) + suffMaxY[r];
        int currAnsMax = x[r - 1] + queryTree(leftMaxTree, maxChangePos, minChangePos) - suffMinY[r];
        int currAnsMinMax = x[r - 1] + queryTree(leftMinMaxTree, std::max(minChangePos, maxChangePos), r);

        ans = std::min(ans, currAnsNone);
        ans = std::min(ans, currAnsMin);
        ans = std::min(ans, currAnsMax);
        ans = std::min(ans, currAnsMinMax);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    solve();
    output();

    return 0;
}
