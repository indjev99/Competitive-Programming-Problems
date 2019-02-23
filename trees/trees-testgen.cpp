#include<iostream>
#include<fstream>
#include<stack>
#include<queue>

using namespace std;

const string TASK_NAME="trees";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=5e4;
const int MAX_Q=5e4;

struct Change
{
    int f, t;
    int node1;
    int treeSize1;
    int node2;
    int par2;
};
int n,q;
int par[MAX_N];
int treeSize[MAX_N];
stack<Change> changes;
priority_queue<pair<int,pair<int,int>>> added;
void init()
{
    for (int i=0;i<n;++i)
    {
        par[i]=i;
        treeSize[i]=1;
    }
    while (!changes.empty()) changes.pop();
    while (!added.empty()) added.pop();
}
int findRoot(int p)
{
    while (par[p]!=p) p=par[p];
    return p;
}
void addEdge(int f, int t)
{
    if (f>t) swap(f,t);
    Change curr;
    curr.f=f;
    curr.t=t;
    int p1=findRoot(f);
    int p2=findRoot(t);
    if (p1==p2) cerr<<"ERROR 2"<<endl;
    if (treeSize[p1]<treeSize[p2]) swap(p1,p2);
    curr.node1=p1;
    curr.treeSize1=treeSize[p1];
    curr.node2=p2;
    curr.par2=par[p2];
    changes.push(curr);
    par[p2]=p1;
    treeSize[p1]+=treeSize[p2];
}
queue<pair<int,int>> toProcess;
void removeEdge(int f, int t)
{
    if (f>t) swap(f,t);
    Change curr;
    while (1)
    {
        if (changes.empty()) cerr<<"ERROR 3"<<endl;
        Change curr=changes.top();
        changes.pop();
        treeSize[curr.node1]=curr.treeSize1;
        par[curr.node2]=curr.par2;
        if (curr.f!=f || curr.t!=t) toProcess.push({curr.f,curr.t});
        else break;
    }
    while (!toProcess.empty())
    {
        f=toProcess.front().first;
        t=toProcess.front().second;
        toProcess.pop();
        addEdge(f,t);
    }
}

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void genTest(ofstream &outF, int testType)
{
    n=MAX_N;
    q=MAX_Q;
    outF<<n<<' '<<q<<'\n';
    init();
    int type,f,t;
    for (int i=0;i<q;++i)
    {
        if (!added.empty() && randomNumber()%9<=testType) type=2;
        else type=1;
        if (type==2)
        {
            f=added.top().second.first;
            t=added.top().second.second;
            added.pop();
            removeEdge(f,t);
        }
        else
        {
            f=randomNumber()%n;
            t=randomNumber()%n;
            if (findRoot(f) == findRoot(t))
            {
                --i;
                continue;
            }
            addEdge(f,t);
            added.push({randomNumber()%100,{f,t}});
        }
        outF<<type<<' '<<f+1<<' '<<t+1<<'\n';
    }
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
