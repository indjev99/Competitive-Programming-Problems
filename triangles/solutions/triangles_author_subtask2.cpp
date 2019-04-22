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
            int l=j,m,r=n;
            while (r-l>1)
            {
                m=(l+r)/2;
                if (lens[i]+lens[j]>lens[m]) l=m;
                else r=m;
            }
            ans+=l+1-j;
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
