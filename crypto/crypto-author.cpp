#include<iostream>
#include<fstream>
using namespace std;

#define official
#ifdef official
ifstream inF("crypto.in");
ofstream outF("crypto.out");
#define cin inF
#define cout outF
#endif

string s;
int n;
int ans;

void input()
{
    cin>>s;
}
void output()
{
    cout<<ans<<'\n';
}
bool checkPeriod(int p)
{
    for (int i=0;i<n-p;++i)
    {
        if (s[i]!=s[i+p]) return 0;
    }
    return 1;
}
void solve()
{
    n=s.size();
    for (int i=1;i<=n;++i)
    {
        if (checkPeriod(i))
        {
            ans=i;
            break;
        }
    }
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
