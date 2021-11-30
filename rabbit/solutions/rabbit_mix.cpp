#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;

vector<int> pushpull(int n, int S, int L)
{
    vector<int> ans;
    int st = 0;
    int i, j;
    int l = 1;

    while(l <= n)
    {
        if (st < S)
        {
            ans.push_back(l);

            if (l < n - 1)
                l += 2;
            else
                l++;
        }
        else
        {
            ans.push_back(n);

            l += 1;
        }

        st = (st + 1) % (L + S);
    }

    return ans;
}

vector<int> ans;
int n, S, L;

int main()
{
    int st = 0;
    int i, j;

    scanf("%d %d %d", &n, &S, &L);

    if (L > S)
    {
        int mid = n / 2;
        int track = n;

        while(track > mid && ans.size() < 2 * n)
        {
            ans.push_back(mid);
            if (st < S)
            {
                track = min(track+1, n);
            }
            else
            {
                track--;
            }

            st = (st + 1) % (S + L);
        }

        ans.push_back(mid);
    }
    else
    {
        int mid = n / 2;
        int track = mid + 1;

        while(track < n && ans.size() < 2 * n)
        {
            ans.push_back(mid);
            if (st < S)
            {
                track = min(track + 1, n);
            }
            else
            {
                track--;
            }

            st = (st + 1) % (S + L);
        }

        if (st < S)
        {
            ans.push_back(1);
            ans.push_back(n);
        }
        else
        {
            ans.push_back(1);
            ans.push_back(n - 1);
        }
    }

    vector<int> alt = pushpull(n, S, L);

    if (alt.size() < ans.size())
        ans = alt;

    printf("%d\n", ans.size());

    for (i=0;i<ans.size();i++)
    {
        if (i != 0)
            printf(" ");
        printf("%d", ans[i]);
    }
    printf("\n");

    return 0;
}
