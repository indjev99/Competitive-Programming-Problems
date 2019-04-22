#include <iostream>
#include <algorithm>

const int MAX_N=34e4;
int n;
int lens[MAX_N];
long long ans;

int sums[MAX_N*3+2];
void find_sums()
{
    for (int i=0;i<n;++i)
    {
        for (int j=i;j<n;++j)
        {
            ++sums[lens[i]+lens[j]];
        }
    }
}
void solve()
{
    find_sums();
    long long sum=0;
    int j=-1;
    for (int i=0;i<n;++i)
    {
        while (j<lens[i])
        {
            ++j;
            sum+=sums[j];
        }
        long long s=i+1;
        ans+=(s*s+s)/2-sum;
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
