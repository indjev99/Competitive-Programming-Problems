#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int __MAX_N=1e5+10;
int __n;
bool __direction[__MAX_N*4+1];

const long long __HASH_BASE=133337;
const long long __HASH_MOD=1234567891;
long long __hash_input_with(long long beg)
{
    long long hsh=(beg+__HASH_MOD)%__HASH_MOD;
    hsh=(hsh*__HASH_BASE+__n)%__HASH_MOD;
    for (int i=0; i<4*__n+1; ++i)
    {
        hsh=(hsh*__HASH_BASE+__direction[i])%__HASH_MOD;
    }
    return hsh;
}
void gen_test(ofstream& outfile)
{
    srand(time(0));
    int n=100000-rand()%1000;
    //int n=99876;
    __n=n;
    outfile<<n<<'\n';
    bool lastDir=true;
    bool direction;
    for (int i=0; i<1*n; ++i)
    {
        if (i%2 || i==0 || i==4*n) __direction[i]=direction=true;
        else if (lastDir==false) __direction[i]=lastDir=direction=true;
        else __direction[i]=lastDir=direction=(rand()%5);
        //else __direction[i]=lastDir=direction=(true);
        outfile<<direction<<' ';
    }
    for (int i=1*n; i<4*n+1; ++i)
    {
        if (i%2 || i==0 || i==4*n) direction=true;
        else if (lastDir==false) lastDir=direction=true;
        else lastDir=direction=!(rand()%5);
        //else lastDir=direction=(true);
        outfile<<direction<<' ';
    }
    outfile<<'\n';
    cout<<"Adaptive? ";
    int ad,side;
    cin>>ad;
    if (ad)
    {
        cin>>side;
        outfile<<__hash_input_with(0)<<' '<<side<<'\n';
    }
}
int main()
{
    int num;
    cout<<"Test num? ";
    cin>>num;
    string nums="";
    nums+=(num/10+'0');
    nums+=(num%10+'0');
    ofstream outfile("park."+nums+".in");
    gen_test(outfile);
    return 0;
}
