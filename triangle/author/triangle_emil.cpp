#include "triangle.h"
#include <numeric>
#include <random>
#include <algorithm>

const int MAX_N = 1e3;

int n;
int p[MAX_N + 1];

std::mt19937 gen(0);

void sort23(int rr)
{
    for (int i = 4; i <= rr; i++)
    {
        if (query(p[2], p[2], p[i]))
        {
            std::swap(p[2], p[3]);
            return;
        }
    }
}

void find2(int r)
{
    for (int i = 3; i <= r; i++)
    {
        int cnt = 0;
        for (int j = 2; j <= r; j++)
        {
            if (j == i) continue;
            cnt += query(p[i], p[i], p[j]);
            if (cnt == 2) break;
        }

        if (cnt == 1)
        {
            std::swap(p[i], p[2]);
            return;
        }
    }
}

void sortChunkSmart(int l, int r, int pivot, int pivotVal)
{
    std::vector<int> elems(p + l, p + r + 1);

    auto it = std::find(elems.begin(), elems.end(), pivot);
    if (it != elems.end())
    {
        std::swap(*it, elems.front());
    }

    std::fill(p + l, p + r + 1, 0);

    std::vector<int> opts(r - l + 1);
    std::iota(opts.begin(), opts.end(), l);

    for (int x : elems)
    {
        int le = 0;
        int re = opts.size();

        while (re - le > 1)
        {
            int me = (le + re) / 2;
            int q = opts[me];
            int q1 = q / 2;
            int q2 = (q + 1) / 2;

            if (x == pivot)
            {
                if (pivotVal >= q)
                {
                    le = me;
                    continue;
                }
                else if (pivotVal + 1 < q)
                {
                    re = me;
                    continue;
                }
            }

            if (query(p[q1], p[q2], x)) re = me;
            else le = me;
        }

        int val = opts[le];
        p[val] = x;

        opts.erase(opts.begin() + le);
    }
}

void sortChunkRec(int l, int r, int rr, int pivot = -1, int pivotVal = -1)
{
    if (r <= l) return;

    if (l == 2)
    {
        if (r == 3)
        {
            sort23(rr);
            return;
        }
        else
        {
            find2(r);
            sortChunkRec(3, r, rr);
            return;
        }
    }

    if (2 * l - 2 >= r)
    {
        sortChunkSmart(l, r, pivot, pivotVal);
        return;
    }

    int q = (l + r + 1) / 2;
    int q1 = q / 2;
    int q2 = (q + 1) / 2;

    q1 = std::min(q1, l - 1);
    q2 = std::min(q2, l - 1);
    q = q1 + q2;

    int ls = q - l;
    int rs = r - l + 1 - ls;

    std::vector<int> left, right;

    for (int i = l; i <= r; i++)
    {
        if ((int) right.size() == rs) left.push_back(p[i]);
        else if ((int) left.size() == ls) right.push_back(p[i]);
        else if (p[i] == pivot && pivotVal + 1 < q) left.push_back(p[i]);
        else if (p[i] == pivot && pivotVal >= q) left.push_back(p[i]);
        else
        {
            if (query(p[q1], p[q2], p[i])) left.push_back(p[i]);
            else right.push_back(p[i]);
        }
    }

    for (int i = 0; i < ls; i++)
    {
        p[l + i] = left[i];
    }

    for (int i = 0; i < rs; i++)
    {
        p[r - i] = right[i];
    }

    sortChunkRec(l, l + ls - 1, r, pivot, pivotVal);
    sortChunkRec(l + ls, r, rr, pivot, pivotVal);
}

void splitSortRec(int r, int rr)
{
    if (r <= 1) return;

    std::uniform_int_distribution<int> distr(1, r);

    int pivot = p[distr(gen)];
    for (int i = 0; i < 7; i++)
    {
        int pivot2;
        do
        {
            pivot2 = p[distr(gen)];
        }
        while (pivot2 == pivot);

        if (!query(pivot, pivot, pivot2))
        {
            pivot = pivot2;
        }
    }

    std::vector<int> left, right;

    right.push_back(pivot);
    for (int i = 1; i <= r; i++)
    {
        if (p[i] == pivot) continue;

        if (query(pivot, p[i], p[i])) right.push_back(p[i]);
        else left.push_back(p[i]);
    }

    int ls = left.size();
    int rs = right.size();

    if (ls == 0)
    {
        splitSortRec(r, rr);
        return;
    }

    for (int i = 0; i < ls; i++)
    {
        p[1 + i] = left[i];
    }

    for (int i = 0; i < rs; i++)
    {
        p[r - i] = right[i];
    }

    splitSortRec(ls, r);

    sortChunkRec(ls + 1, r, rr, pivot, 2 * ls);
}

std::vector<int> solve(int _n)
{
    n = _n;

    std::iota(p + 1, p + n + 1, 0);
    std::shuffle(p + 1, p + n + 1, gen);

    splitSortRec(n, n);

    std::vector<int> ans(n);

    for (int i = 1; i <= n; i++)
    {
        ans[p[i]] = i;
    }

    return ans;
}
