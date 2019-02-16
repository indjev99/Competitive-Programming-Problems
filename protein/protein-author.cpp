#include<iostream>
#include<fstream>
using namespace std;

#define official
#ifdef official
ifstream inF("protein.in");
ofstream outF("protein.out");
#define cin inF
#define cout outF
#endif

const long long MOD=1e9+7;
const int MAX_S=10;
const int MAX_M=10+1;
bool nums[MAX_S+1];
int n, m;
int lnum;
long long k;
long long ans;

struct Vector
{
    long long x[MAX_M*MAX_S];
};
Vector operator+(const Vector& u, const Vector v)
{
    Vector r;
    for (int i=0;i<m*MAX_S;++i)
    {
        r.x[i]=(u.x[i]+v.x[i])%MOD;
    }
    return r;
}
struct Matrix
{
    long long a[MAX_M*MAX_S][MAX_M*MAX_S];
};
Vector operator*(const Matrix& a, const Vector& u)
{
    Vector r;
    for (int i=0;i<m*MAX_S;++i)
    {
        r.x[i]=0;
        for (int t=0;t<m*MAX_S;++t)
        {
            r.x[i]=(r.x[i]+a.a[i][t]*u.x[t])%MOD;
        }
    }
    return r;
}
Matrix operator*(const Matrix& a, const Matrix& b)
{
    Matrix r;
    for (int i=0;i<m*MAX_S;++i)
    {
        for (int j=0;j<m*MAX_S;++j)
        {
            r.a[i][j]=0;
            for (int t=0;t<m*MAX_S;++t)
            {
                r.a[i][j]=(r.a[i][j]+a.a[i][t]*b.a[t][j])%MOD;
            }
        }
    }
    return r;
}
void input()
{
    cin>>k;
    k/=10;
    cin>>n;
    int a;
    for (int i=0;i<n;++i)
    {
        cin>>a;
        a/=10;
        nums[a]=1;
    }
    cin>>lnum>>m;
    lnum/=10;
    nums[lnum]=0;
}
void output()
{
    cout<<ans<<endl;
}

void generateVector(Vector& v)
{
    for (int i=0;i<m;++i)
    {
        for (int j=0;j<MAX_S;++j)
        {
            if (i==m-1 && j==0) v.x[i*MAX_S+j]=1;
            else v.x[i*MAX_S+j]=0;
        }
    }
}
void printVector(const Vector& v)
{
    for (int i=0;i<m*MAX_S;++i)
    {
        cerr<<v.x[i]<<' ';
        if (i%MAX_S==MAX_S-1) cerr<<' ';
    }
    cerr<<endl;
}
void generateMatrix(Matrix& M)
{
    for (int i=0;i<m;++i)
    {
        for (int j=0;j<MAX_S;++j)
        {
            M.a[i*MAX_S][i*MAX_S+j]=nums[j+1];
        }
        if (i<m-1) M.a[i*MAX_S][i*MAX_S+MAX_S-1+lnum]=1;
        for (int j=1;j<MAX_S;++j)
        {
            M.a[i*MAX_S+j][i*MAX_S+j-1]=1;
        }
    }
}
void generateUnitMatrix(Matrix& M)
{
    for (int i=0;i<m*MAX_S;++i)
    {
        for (int j=0;j<m*MAX_S;++j)
        {
            if (i!=j) M.a[i][j]=0;
            else M.a[i][j]=1;
        }
    }
}
Matrix R;
void raiseMatrixToPowerIterative(const Matrix& M, int power)
{
    generateUnitMatrix(R);
    for (int i=31;i>=0;--i)
    {
        R=R*R;
        if (power&(1<<i)) R=R*M;
    }
}
Matrix raiseMatrixToPowerRecursive(const Matrix& M, int power)
{
    if (power==1) return M;
    if (power%2==0) return raiseMatrixToPowerRecursive(M*M,power/2);
    return M*raiseMatrixToPowerRecursive(M,power-1);
}
void printMatrix(const Matrix& M)
{
    for (int i=0;i<m*MAX_S;++i)
    {
        for (int j=0;j<m*MAX_S;++j)
        {
            cerr<<M.a[i][j]<<' ';
        }
        cerr<<'\n';
    }
    cerr<<endl;
}
void getAnswer(const Vector& v)
{
    ans=0;
    for (int i=0;i<m;++i)
    {
        ans=(ans+v.x[i*MAX_S])%MOD;
    }
}
Vector v;
Matrix M;
void solve()
{
    ++m;
    generateVector(v);
    generateMatrix(M);
    raiseMatrixToPowerIterative(M,k);
    v=R*v;
    getAnswer(v);
}

int main()
{
    input();
    solve();
    output();

    return 0;
}
