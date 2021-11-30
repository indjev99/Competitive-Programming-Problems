#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;

vector<int> ans;
int n, S, L;

int main()
{
    int st = 0;
    int i, j;

    scanf("%d %d %d", &n, &S, &L);

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
