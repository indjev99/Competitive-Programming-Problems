#include <iostream>
#include <algorithm>

const int MAX_N=34e4;
int n;
int lens[MAX_N];
long long ans;

inline void add(const int a[], const int b[], int n, int ans[])
{
    for (int i=0;i<n;++i)
    {
        ans[i]=a[i]+b[i];
    }
}
inline void sub(const int a[], const int b[], int n, int ans[])
{
    for (int i=0;i<n;++i)
    {
        ans[i]=a[i]-b[i];
    }
}
void base_mul(const int a[], const int b[], int n, int ans[])
{
    std::fill(ans,ans+2*n,0);
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            ans[i+j]+=a[i]*b[j];
        }
    }
}
void mul(const int a[], const int b[], int n, int ans[])
{
    if (n<=30)
    {
        base_mul(a,b,n,ans);
        return;
    }

    // a = a2<<h + a1
    // b = b2<<h + b1

    int h=(n+1)/2;
    int r=n-h;
    int* tmp=new int[2*h];

    // tmp[0..h] = a1+a2
    add(a,a+h,r,tmp);
    if (h>r) tmp[r]=a[r];

    // tmp[0..h] = b1+b2
    add(b,b+h,r,tmp+h);
    if (h>r) tmp[h+r]=b[r];

    // ans[0..2h] = (a1+a2)*(b1+b2) = z1
    mul(tmp,tmp+h,h,ans);

    // ans[2h..2n] = a2*b2 = z2
    mul(a+h,b+h,r,ans+2*h);

    // tmp[0..2r] = z1-z2
    sub(ans,ans+2*h,2*r,tmp);
    if (h>r)
    {
        tmp[2*r]=ans[2*r];
        tmp[2*r+1]=ans[2*r+1];
    }

    // ans[0..2h] = a1*b1 = z0
    mul(a,b,h,ans);

    // tmp[0..2r] = z1-(z0+z2)
    sub(tmp,ans,2*h,tmp);

    // ans = z2<<2h + (z1-(z0+z2))<<h + z0
    add(ans+h,tmp,2*h,ans+h);

    delete[] tmp;
}

int vals[MAX_N*3/2+1];
int sums[MAX_N*3+2];
void find_sums()
{
    for (int i=0;i<n;++i)
    {
        ++vals[lens[i]];
    }
    mul(vals,vals,n*3/2+1,sums);
    for (int i=0;i<=3*n;++i)
    {
        sums[i]=(sums[i]+1)/2;
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
