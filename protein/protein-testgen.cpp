#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="protein";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const long long MAX_K=1e9;
const int MAX_S=10;
const int MAX_M=10;
bool nums[MAX_S+1];
int n, m;
int lnum;
long long k;

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_K;
    r=(r*(RAND_MAX+1)+rand())%MAX_K;
    r=(r*(RAND_MAX+1)+rand())%MAX_K;
    return r;
}
void printTest(ofstream &outF)
{
    bool first;

    outF<<k<<'\n';
    outF<<n<<'\n';
    first=true;
    for (int i=1;i<=MAX_S;++i)
    {
        if (nums[i])
        {
            if (first) first=false;
            else outF<<' ';
            outF<<i*10;
        }
    }
    outF<<'\n';
    outF<<lnum<<' '<<m<<'\n';

    cerr<<k<<'\n';
    cerr<<n<<'\n';
    first=true;
    for (int i=1;i<=MAX_S;++i)
    {
        if (nums[i])
        {
            if (first) first=false;
            else cerr<<' ';
            cerr<<i*10;
        }
    }
    cerr<<'\n';
    cerr<<lnum<<' '<<m<<'\n';
}
void genTest(ofstream &outF)
{
    k=randomNumber()%MAX_K+1;
    k*=10;
    n=randomNumber()%(MAX_S-1)+2;
    m=randomNumber()%(MAX_M+1);
    for (int i=0;i<=MAX_S;++i)
    {
        nums[i]=0;
    }
    int num;
    for (int i=0;i<n;++i)
    {
        num=randomNumber()%MAX_S+1;
        while (nums[num])
        {
            num=randomNumber()%MAX_S+1;
        }
        nums[num]=1;
    }
    num=randomNumber()%MAX_S+1;
    while (!nums[num])
    {
        num=randomNumber()%MAX_S+1;
    }
    lnum=num;
    lnum*=10;
    printTest(outF);
}
int curr=1;
string getName()
{
    string name;
    name=TASK_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=INPUT_EXT;
    ++curr;
    return name;
}
void genBatch()
{
    string name;
    ofstream currOutF;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        name=getName();
        cerr<<name<<endl;
        currOutF.open(name.c_str());
        genTest(currOutF);
        currOutF.close();
    }

}
int main()
{
    cin>>SEED;
    cin>>TESTS_IN_BATCH;
    srand(SEED);
    genBatch();
    return 0;
}
