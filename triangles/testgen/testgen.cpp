#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>

using namespace std;

const int MAX_N=34e4;
bool taken[MAX_N*3/2+1];
int lens[MAX_N];
int n;

long long random_number()
{
    return rand()*(RAND_MAX+1ll)+rand();
}
void gen_test(ofstream& outfile)
{
    srand(time(0));
    int maxLen=n*3/2;
    fill(taken,taken+maxLen+1,false);
    for (int i=0;i<n;++i)
    {
        int len;
        do
        {
            len=random_number()%maxLen+1;
        }
        while (taken[len]);
        taken[len]=true;
    }
    int j=0;
    for (int i=1;i<=maxLen;++i)
    {
        if (taken[i]) lens[j++]=i;
    }
    assert(j==n);
    random_shuffle(lens,lens+n);
    outfile<<n<<'\n';
    for (int i=0;i<n;++i)
    {
        if (i) outfile<<' ';
        outfile<<lens[i];
    }
    outfile<<'\n';
}
int main()
{
    int num;
    while (1)
    {
        cout<<"Test num? ";
        cin>>num;
        cout<<"N? ";
        cin>>n;
        string nums="";
        nums+=(num/10+'0');
        nums+=(num%10+'0');
        ofstream outfile("triangles."+nums+".in");
        gen_test(outfile);
    }
    return 0;
}
