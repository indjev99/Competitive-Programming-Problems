#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="bases";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=20;
const int MAX_NUM=1e6;
const int MAX_K=25;
const int MAX_Q=10000;
int n,k,q;
vector<int> digs;
vector<int> qs;

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_NUM;
    r=(r*(RAND_MAX+1)+rand())%MAX_NUM;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<n<<' '<<k<<'\n';
    for (int i=0;i<k;++i)
    {
        if (i) outF<<' ';
        outF<<digs[i];
    }
    outF<<'\n';
    outF<<q<<'\n';
    for (int i=0;i<q;++i)
    {
        if (i) outF<<' ';
        outF<<qs[i];
    }
    outF<<'\n';
}
bool isNotFree(int d)
{
    for (int i=0;i<digs.size();++i)
    {
        if (digs[i]==d) return 1;
    }
    return 0;
}
void genTest(ofstream &outF, int type)
{
    digs.resize(0);
    qs.resize(0);
    int pSmall;
    if (type==0) pSmall=10;
    if (type==1) pSmall=25;
    if (type==2) pSmall=50;
    if (type==3) pSmall=75;
    if (type==4) pSmall=90;
    n=randomNumber()%(MAX_N-1)+2;
    n=2;
    k=MAX_K*n;
    int d;
    for (int i=0;i<k;++i)
    {
        if (randomNumber()%100<pSmall) d=randomNumber()%(3*n);
        else d=randomNumber()%(MAX_NUM/2);
        if (isNotFree(d)) --i;
        else digs.push_back(d);
    }
    q=MAX_Q;
    for (int i=0;i<q;++i)
    {
        if (randomNumber()%10==0) d=randomNumber()%(10*n)+1;
        else d=randomNumber()%MAX_NUM+1;
        qs.push_back(d);
    }
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
        genTest(currOutF,i%5);
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
