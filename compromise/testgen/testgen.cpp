#include <iostream>
#include <fstream>
#include <set>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <math.h>

using namespace std;

const int MAX_N=1e6;
const int MAX_M=1e6;
const long long MAX_T=1e12;

struct Edge
{
    int from,to;
    long long time;
};

int n,m;
Edge edges[MAX_M];
long long targetAns;
long long targetLength;

set<long long> taken;

long long random_number()
{
    long long ans=rand()%MAX_T;
    ans=ans*(RAND_MAX+1ll)%MAX_T+rand();
    ans=ans*(RAND_MAX+1ll)%MAX_T+rand();
    ans=ans*(RAND_MAX+1ll)%MAX_T+rand();
    return ans;
}

int lastTime[MAX_N];
int parent[MAX_N];
int setSize[MAX_N];
int components;
int timestamp=0;
void clear_sets()
{
    ++timestamp;
    components=n;
}
int find_set(int a)
{
    if (lastTime[a]<timestamp)
    {
        lastTime[a]=timestamp;
        parent[a]=-1;
        setSize[a]=1;
        return a;
    }
    if (parent[a]==-1) return a;
    return parent[a]=find_set(parent[a]);
}
void unite_sets(int a, int b)
{
    a=find_set(a);
    b=find_set(b);
    if (a!=b)
    {
        --components;
        if (setSize[a]>=setSize[b])
        {
            parent[b]=a;
            setSize[a]=setSize[a]+setSize[b];
        }
        else
        {
            parent[a]=b;
            setSize[b]=setSize[a]+setSize[b];
        }
    }
}

void gen_edges(int type)
{
    if (type<=3 || type==7)
    {
        for (int i=0;i<m;++i)
        {
            edges[i].from=random_number()%n;
            edges[i].to=random_number()%n;
        }
        return;
    }
    else
    {
        for (int i=0;i<m;++i)
        {
            edges[i].from=i%n;
            edges[i].to=(i+1)%n;
            if (random_number()%2) swap(edges[i].from,edges[i].to);
        }
        return;
    }
}

struct Range
{
    int left,right;
    int leftChild,rightChild;
    int components;
    int size() const
    {
        return right-left+1;
    }
    void finish();
};
Range ranges[MAX_M+1];
void Range::finish()
{
    if (leftChild==-1 || (rightChild>=0 && ranges[leftChild].size()<ranges[rightChild].size())) std::swap(leftChild,rightChild);
}

int split_range(int length, int type)
{
    if (type%3==1) return random_number()%2 ? 0 : length-1;
    if (type%3==2) return random_number()%length;
    if (type%3==0) return length/2;
}

int gen_range(int left, int right, int type)
{
    if (right<left) return -1;
    int pos=left+split_range(right-left+1,type);

    ranges[pos].left=left;
    ranges[pos].right=right;

    ranges[pos].leftChild=gen_range(left,pos-1,type);
    ranges[pos].rightChild=gen_range(pos+1,right,type);

    ranges[pos].finish();

    return pos;
}

int gen_ranges(int type)
{
    if (type==7) return -1;
    return gen_range(0,m-1,type);
}

int currLeft,currRight;
void components_DFS(int pos)
{
    if (ranges[pos].rightChild!=-1) components_DFS(ranges[pos].rightChild);
    if (ranges[pos].leftChild!=-1) components_DFS(ranges[pos].leftChild);
    else
    {
        clear_sets();
        currLeft=pos;
        currRight=pos-1;
    }
    while (currLeft>ranges[pos].left)
    {
        --currLeft;
        unite_sets(edges[currLeft].from,edges[currLeft].to);
    }
    while (currRight<ranges[pos].right)
    {
        ++currRight;
        unite_sets(edges[currRight].from,edges[currRight].to);
    }
    long long currAns;
    unite_sets(edges[pos].from,edges[pos].to);
    ranges[pos].components=components;
}
unordered_map<long long,long long> lastTriedHigh;
unordered_map<long long,long long> lastTriedLow;
long long cnt;
long long cnt2;
void times_DFS(int pos, long long maxTime, int type)
{
    if (pos==-1) return;
    ++cnt;
    //if (cnt%10000==0) cerr<<cnt/10000<<' '<<cnt2<<'\n';
    long long targetTime;
    targetTime=round(targetAns*1.0/ranges[pos].components);
    if (ranges[pos].size()<targetLength) targetTime*=1.05;
    long long time=-1;
    if (lastTriedHigh.find(targetTime)==lastTriedHigh.end())
    {
        lastTriedHigh[targetTime]=targetTime-1;
        lastTriedLow[targetTime]=targetTime+1;
    }
    long long tryTime=lastTriedHigh[targetTime]+1;
    if (tryTime<1) tryTime=1;
    if (tryTime>=maxTime) tryTime=maxTime-1;
    for (;tryTime<maxTime;++tryTime)
    {
        ++cnt2;
        //cerr<<cnt<<" "<<ranges[pos].components<<" : "<<tryTime<<endl;
        if (taken.find(tryTime)==taken.end())
        {
            time=tryTime;
            break;
        }
    }
    lastTriedHigh[targetTime]=tryTime;
    if (time==-1)
    {
        //cerr<<"Warning: "<<ranges[pos].size()<<" "<<ranges[pos].components<<endl;
        long long tryTime=lastTriedLow[targetTime]-1;
        if (tryTime<1) tryTime=1;
        if (tryTime>=maxTime) tryTime=maxTime-1;
        for (;tryTime>0;--tryTime)
        {
            ++cnt2;
            if (taken.find(tryTime)==taken.end())
            {
                time=tryTime;
                break;
            }
        }
        lastTriedLow[targetTime]=tryTime;
    }
    if (time==-1)
    {
        cerr<<"Rip: couldn't find time."<<endl;
        //throw "Chao!";
    }
    edges[pos].time=time;
    taken.insert(time);

    //cerr<<ranges[pos].components<<" * "<<time<<" = "<<ranges[pos].components * 1ll * time<<endl;

    if (random_number()%2)
    {
        times_DFS(ranges[pos].leftChild,time,type);
        times_DFS(ranges[pos].rightChild,time,type);
    }
    else
    {
        times_DFS(ranges[pos].rightChild,time,type);
        times_DFS(ranges[pos].leftChild,time,type);
    }
}
long long gen_random_time()
{
    long long time;
    do
    {
        time=random_number()%(m)+m;
    }
    while (taken.find(time)!=taken.end());
    taken.insert(time);
    return time;
}
void gen_random_times(int type)
{
    for (int i=0;i<m;++i)
    {
        edges[i].time=i+m;
    }
    random_shuffle(edges,edges+m);
}

void gen_test(ofstream& outfile, int type)
{
    srand(time(0));

    cnt=0;
    cnt2=0;
    taken.clear();
    lastTriedHigh.clear();
    lastTriedLow.clear();

    gen_edges(type);
    cerr<<"Generated edges"<<endl;

    if (type<7)
    {
        int root=gen_ranges(type);
        cerr<<"Generated ranges"<<endl;

        components_DFS(root);
        cerr<<"Calculated components: "<<components<<endl;

        targetAns=MAX_T*components*0.9;
        cerr<<"Calculated target ans: "<<targetAns<<endl;

        targetLength=random_number()%(m/5+1)+1;
        cerr<<"Generate target length: "<<targetLength<<endl;

        times_DFS(root,MAX_T+1,type);
    }
    else gen_random_times(type);
    cerr<<"Generated times"<<endl;

    outfile<<n<<' '<<m<<'\n';
    for (int i=0;i<m;++i)
    {
        outfile<<edges[i].from+1<<' '<<edges[i].to+1<<' '<<edges[i].time<<'\n';
    }
}
int main()
{
    ios::sync_with_stdio(false);
    //cin.tie(nullptr);
    int num,type;
    int on,om;
    while (1)
    {
        cout<<"Test num? ";
        cin>>num;
        cout<<"N? ";
        cin>>n;
        cout<<"M? ";
        cin>>m;
        cout<<"Type? ";
        cin>>type;

        if (n==0) n=on;
        else on=n;
        if (m==0) m=om;
        else om=m;

        string nums="";
        nums+=(num/10+'0');
        nums+=(num%10+'0');
        ofstream outfile("compromise."+nums+".in");

        gen_test(outfile,type);
    }
    return 0;
}
