#include <iostream>
#include <algorithm>

const int MAX_N=34e4;
int n;
int lens[MAX_N];
long long ans;

void solve()
{
    for (int i=0;i<n;++i)
    {
        for (int j=i;j<n;++j)
        {
            for (int k=j;k<n;++k)
            {
                if (lens[i]+lens[j]>lens[k]) ++ans;
            }
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
