#include<iostream>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="rome";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=300;
int n;
vector<pair<int,int>> e;

bool adj[MAX_N][MAX_N];

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
    outF<<n<<' '<<e.size()<<'\n';
    for (int i=0;i<e.size();++i)
    {
        outF<<e[i].first+1<<' '<<e[i].second+1<<'\n';
    }
}
bool dirHub1[MAX_N],revHub1[MAX_N],dirHub2[MAX_N],revHub2[MAX_N];
int perm[MAX_N];
void setPerm()
{
    for (int i=0;i<n;++i)
    {
        perm[i]=i;
    }
    for (int i=n-1;i>=0;--i)
    {
        swap(perm[i],perm[randomNumber()%(i+1)]);
    }
}
void dirDFS(int curr, bool vis[])
{
    vis[curr]=1;
    for (int i=0;i<n;++i)
    {
        if (!vis[i] && adj[curr][i])
        {
            dirDFS(i,vis);
        }
    }
}
void revDFS(int curr, bool vis[])
{
    vis[curr]=1;
    for (int i=0;i<n;++i)
    {
        if (!vis[i] && adj[i][curr])
        {
            revDFS(i,vis);
        }
    }
}
void genTest(ofstream &outF, int type)
{
    // 0 - one hub
    // 1 - only edge between them
    // 2 - only two edges between them
    // 3 - paths between them, edge
    // 4 - paths between them, no edge

    e.resize(0);
    for (int i=0;i<MAX_N;++i)
    {
        dirHub1[i]=0;
        revHub1[i]=0;
        dirHub2[i]=0;
        revHub2[i]=0;
        for (int j=0;j<MAX_N;++j)
        {
            adj[i][j]=0;
        }
    }

    int hub1,hub2;
    n=MAX_N-randomNumber()%30;
    hub1=randomNumber()%n;
    if (type==0) hub2=-1;
    else
    {
        do
        {
            hub2=randomNumber()%n;
        }
        while (hub2==hub1);
    }
    if (type==1 || type==2 || type==3)
    {
        adj[hub1][hub2]=1;
        e.push_back({hub1,hub2});
    }
    if (type==2)
    {
        adj[hub2][hub1]=1;
        e.push_back({hub2,hub1});
    }

    int m=randomNumber()%(3*n/2);
    int f,t;
    for (int i=0;i<m;++i)
    {
        do
        {
            f=randomNumber()%n;
            t=randomNumber()%n;
        }
        while (f==hub1 || t==hub1 || f==hub2 || t==hub2 || f==t || adj[f][t]);
        adj[f][t]=1;
        e.push_back({f,t});
        if (randomNumber()%5 && adj[t][f]==0)
        {
            adj[t][f]=1;
            e.push_back({t,f});
        }
    }
    int curr;
    setPerm();
    for (int i=0;i<n;++i)
    {
        curr=perm[i];
        if (curr==hub1 || curr==hub2 || adj[hub1][curr] || adj[curr][hub1]) continue;
        if (dirHub1[curr]==0 && revHub1[curr]==0)
        {
            if (randomNumber()%2)
            {
                adj[hub1][curr]=1;
                e.push_back({hub1,curr});
                dirDFS(curr,dirHub1);
            }
            else
            {
                adj[curr][hub1]=1;
                e.push_back({curr,hub1});
                revDFS(curr,revHub1);
            }
        }
        else if (dirHub1[curr]==1)
        {
            if (randomNumber()%3)
            {
                adj[hub1][curr]=1;
                e.push_back({hub1,curr});
                dirDFS(curr,dirHub1);
            }
        }
        else //revHub1[curr]==1
        {
            if (randomNumber()%3)
            {
                adj[curr][hub1]=1;
                e.push_back({curr,hub1});
                revDFS(curr,revHub1);
            }
        }
    }
    /*for (int i=0;i<n;++i)
    {
        cerr<<dirHub1[i]<<" "<<revHub1[i]<<endl;
    }*/
    if (type>0)
    {
        setPerm();
        for (int i=0;i<n;++i)
        {
            curr=perm[i];
            if (curr==hub1 || curr==hub2 || adj[hub2][curr] || adj[curr][hub2]) continue;
            if (dirHub2[curr]==0 && revHub2[curr]==0)
            {
                if (revHub1[curr]==1) //so not to make cycles
                {
                    adj[curr][hub2]=1;
                    e.push_back({curr,hub2});
                    revDFS(curr,revHub2);
                }
                else if (type==1 || type==2)
                {
                    adj[hub2][curr]=1;
                    e.push_back({hub2,curr});
                    dirDFS(curr,dirHub2);
                }
                else
                {
                    if (randomNumber()%2)
                    {
                        adj[hub2][curr]=1;
                        e.push_back({hub2,curr});
                        dirDFS(curr,dirHub2);
                    }
                    else
                    {
                        adj[curr][hub2]=1;
                        e.push_back({curr,hub2});
                        revDFS(curr,revHub2);
                    }
                }
            }
            else if (dirHub2[curr]==1)
            {
                if (randomNumber()%3)
                {
                    adj[hub2][curr]=1;
                    e.push_back({hub2,curr});
                    dirDFS(curr,dirHub2);
                }
            }
            else //revHub2[curr]==1
            {
                if (randomNumber()%3)
                {
                    adj[curr][hub2]=1;
                    e.push_back({curr,hub2});
                    revDFS(curr,revHub2);
                }
            }
        }
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
