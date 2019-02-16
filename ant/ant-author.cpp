#include<iostream>
#include<fstream>
#include<algorithm>
using namespace std;

#define official
#ifdef official
ifstream inF("ant.in");
ofstream outF("ant.out");
#define cin inF
#define cout outF
#endif

int a[3];
int ans;

void input()
{
    cin>>a[0]>>a[1]>>a[2];
}
void output()
{
    cout<<ans<<endl;
}
void solve()
{
    sort(a,a+3);
    ans=(a[0]+a[1])*(a[0]+a[1])+a[2]*a[2];
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
