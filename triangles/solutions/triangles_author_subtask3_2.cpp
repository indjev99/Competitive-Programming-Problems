#include <iostream>
#include <algorithm>

const int MAX_N=34e4;
int n;
int lens[MAX_N];
long long ans;

int sums[MAX_N*3+2];
void solve()
{
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<=i;++j)
        {
            ++sums[lens[i]+lens[j]];
        }
        for (int j=lens[i]+1;j<=3*n;++j)
        {
            ans+=sums[j];
        }
    }
}
long long triangles(const int newLens[], int newN)
{
    n=newN;
    for (int i=0;i<n;++i)
    {
        lens[i]=newLens[i];
    }
	std::sort(lens,lens+n);
    solve();
    return ans;
}
