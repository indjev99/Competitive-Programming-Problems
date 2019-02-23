#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="space";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=100000;
const int MAX_M=100000;
const int MAX_Q=100000;
const int MAX_K=20;
int n,m,s,k,q;
int qs[MAX_Q];
int be[MAX_N];
vector<int> e[MAX_N];
vector<int> atD[MAX_N];
int d[MAX_N];

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<n<<' '<<m<<' '<<s+1<<' '<<k<<'\n';
    for (int i=1;i<n;++i)
    {
        if (i>1) outF<<' ';
        outF<<be[i]+1;
    }
    outF<<'\n';
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<e[i].size();++j)
        {
            outF<<i+1<<' '<<e[i][j]+1<<'\n';
        }
    }
    outF<<q<<'\n';
    for (int i=0;i<q;++i)
    {
        if (i) outF<<' ';
        outF<<qs[i]+1;
    }
    outF<<'\n';
}
bool isEdge(int a, int b)
{
    for (int i=0;i<e[a].size();++i)
    {
        if (e[a][i]==b) return 1;
    }
    return 0;
}
void genTest(ofstream &outF, int type)
{
    for (int i=0;i<n;++i)
    {
        e[i].resize(0);
        atD[i].resize(0);
    }
    n=MAX_N;
    m=randomNumber()%MAX_M;
    if (type==0) s=randomNumber()%(n-1)+1;
    else s=0;
    k=randomNumber()%MAX_K+1;
    d[0]=0;
    atD[0].push_back(0);
    for (int i=1;i<n;++i)
    {
        be[i]=randomNumber()%i;
        d[i]=d[be[i]]+1;
        atD[d[i]].push_back(i);
        if (type && d[i]>d[s]) s=i;
    }
    cerr<<d[s]<<' '<<k<<endl;
    int a,b,ops;
    int rnd;
    vector<int>* pool;
    for (int i=0;i<m;++i)
    {
        a=randomNumber()%(n-1)+1;
        rnd=randomNumber()%100;
        if (rnd<25) pool=&atD[d[a]+1];
        else if (rnd<75) pool=&atD[d[a]+1]; ///maybe parameter
        else pool=&atD[d[a]+1];
        ops=pool->size();
        if (ops==0)
        {
            --i;
            continue;
        }
        b=(*pool)[randomNumber()%ops];
        if (a>b) swap(a,b);
        if (b==0 || a==b || isEdge(a,b))
        {
            --i;
            continue;
        }
        e[a].push_back(b);
    }
    q=MAX_Q;
    for (int i=0;i<q;++i)
    {
        qs[i]=randomNumber()%(n-1)+1;
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
