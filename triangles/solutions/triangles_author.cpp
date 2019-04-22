#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
# define PI 3.14159265358979323846

const int MAX_N=34e4;
int n;
int lens[MAX_N];
long long ans;

struct ComplexNumber
{
    double real,imag;
};
ComplexNumber operator+(const ComplexNumber& a, const ComplexNumber& b)
{
    return {a.real+b.real,a.imag+b.imag};
}
ComplexNumber operator-(const ComplexNumber& a, const ComplexNumber& b)
{
    return {a.real-b.real,a.imag-b.imag};
}
ComplexNumber operator*(const ComplexNumber& a, const ComplexNumber& b)
{
    return {a.real*b.real-a.imag*b.imag,a.real*b.imag+a.imag*b.real};
}
void operator/=(ComplexNumber& a, const double& b)
{
    a.real/=b;
    a.imag/=b;
}
int bit_reverse(int a, int log2n)
{
    int ans=0;
    for (int i=0; i<log2n; ++i)
    {
        ans<<=1;
        ans|=(a&1);
        a>>=1;
    }
    return ans;
}
void fft(const ComplexNumber a[], int log2n, ComplexNumber ans[], bool inv=0)
{
    int n=1<<log2n;
    for (int i=0;i<n;++i)
    {
        ans[i]=a[bit_reverse(i,log2n)];
    }
    ComplexNumber w,wn,u,t;
    double f=inv?-1:1;
    for (int s=1; s<=log2n;++s)
    {
        int m=1<<s;
        int m2=m>>1;
        w={1,0};
        wn={cos(f*PI/m2),sin(f*PI/m2)};
        for (int j=0;j<m2;++j)
        {
            for (int k=j;k<n;k+=m)
            {
                u=ans[k];
                t=ans[k+m2]*w;
                ans[k]=u+t;
                ans[k+m2]=u-t;
            }
            w=w*wn;
        }
    }
    if (inv)
    {
        for (int i=0;i<n;++i)
        {
            ans[i]/=n;
        }
    }
}
const int MAX_LOG=1<<20;
ComplexNumber ac[MAX_LOG];
ComplexNumber bc[MAX_LOG];
ComplexNumber aft[MAX_LOG];
ComplexNumber bft[MAX_LOG];
void mul(const int a[], const int b[], int n, int ans[])
{
    int log2n=0;
    while (1<<log2n<2*n)
    {
        ++log2n;
    }
    for (int i=0;i<n;++i)
    {
        ac[i]={a[i],0};
        bc[i]={b[i],0};
    }
    std::fill(ac+n,ac+(1<<log2n),ComplexNumber{0,0});
    std::fill(bc+n,bc+(1<<log2n),ComplexNumber{0,0});
    fft(ac,log2n,aft);
    fft(bc,log2n,bft);
    for (int i=0;i<1<<log2n;++i)
    {
        aft[i]=aft[i]*bft[i];
    }
    fft(aft,log2n,ac,1);
    for (int i=0;i<2*n;++i)
    {
        ans[i]=round(ac[i].real);
    }
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
