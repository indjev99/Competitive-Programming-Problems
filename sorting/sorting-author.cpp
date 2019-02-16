#include<iostream>
#include<math.h>
#include<vector>
#include<fstream>
#include<queue>
using namespace std;

#define INF 1e9

#define official
#ifdef official
ifstream inF("sorting.in");
ofstream outF("sorting.out");
#define cin inF
#define cout outF
#endif

const int MAX_N=400;
int n;
int perm[MAX_N];
int tp;
int cp[MAX_N][MAX_N];
int tv;
int cv[MAX_N][MAX_N];

int permOriginal[MAX_N];
int totalPrice;
long long opCounter;

struct Swap
{
    int f,t;
};
vector<Swap> s;

void input()
{
    cin>>n;
    for (int i=0;i<n;++i)
    {
        cin>>perm[i];
        --perm[i];
        permOriginal[i]=perm[i];
    }
    cin>>tp;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            cin>>cp[i][j];
        }
    }
    cin>>tv;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            cin>>cv[i][j];
        }
    }
}
void output()
{
    cerr<<tp<<' '<<tv<<": "<<totalPrice<<'\n';
    //cerr<<s.size()<<'\n';
    cout<<s.size()<<'\n';
    for (int i=0;i<s.size();++i)
    {
        cout<<s[i].f<<' '<<s[i].t<<'\n';
        //cerr<<s[i].f<<' '<<s[i].t<<'\n';
    }
}
void reset()
{
    s.resize(0);
    totalPrice=0;
    for (int i=0;i<n;++i)
    {
        perm[i]=permOriginal[i];
    }
}
void performSwap(int a, int b)
{
    int currPrice=cp[a][b]+cv[perm[a]][perm[b]];
    //cerr<<currPrice<<'\n';
    totalPrice+=currPrice;
    s.push_back({a+1,b+1});
    swap(perm[a],perm[b]);
}
void bubbleSort()
{
    for (int i=n-1;i>=0;--i)
    {
        for (int j=0;j<i;++j)
        {
            ++opCounter;
            if (perm[j]>perm[j+1])
            {
                performSwap(j,j+1);
            }
        }
    }
}
int findCheapestAdjSwap()
{
    int minPos=-1;
    int minPrice=INF;
    int currPrice;
    for (int i=0;i<n-1;++i)
    {
        ++opCounter;
        if (perm[i]<perm[i+1]) continue;
        currPrice=cp[i][i+1]+cv[perm[i]][perm[i+1]];
        if (minPrice>currPrice)
        {
            minPrice=currPrice;
            minPos=i;
        }
    }
    return minPos;
}
void cheapestAdjSwapSort(int maxSwaps=-1)
{
    int cnt=0;
    int curr=findCheapestAdjSwap();
    while (cnt!=maxSwaps && curr>=0)
    {
        performSwap(curr,curr+1);
        ++cnt;
        curr=findCheapestAdjSwap();
    }
}

const int FCS_RADIUS=400;
pair<int, int> findCheapestSwap()
{
    pair <int, int> minPos={-1,-1};
    int minPrice=INF;
    int currPrice;
    for (int i=0;i<n;++i)
    {
        for (int j=i+1;j<min(n,i+FCS_RADIUS);++j)
        {
            ++opCounter;
            if (perm[i]<perm[j]) continue;
            currPrice=(cp[i][j]+cv[perm[i]][perm[j]]);
            if (minPrice>currPrice)
            {
                minPrice=currPrice;
                minPos={i,j};
            }
        }
    }
    return minPos;
}
void cheapestSwapSort(int maxSwaps=-1)
{
    int cnt=0;
    pair<int, int> curr=findCheapestSwap();
    while (cnt!=maxSwaps && curr.first>=0 && curr.second>=0)
    {
        performSwap(curr.first,curr.second);
        ++cnt;
        curr=findCheapestSwap();
    }
}

int ASC_MAX_LEN;
void superCycleSort(int maxCycle);

int findCheapestInsertion()
{
    int minPos=-1;
    int minPrice=INF;
    int currPrice;
    int next;
    for (int i=0;i<n;++i)
    {
        ++opCounter;
        next=perm[i];
        if (next==i) continue;
        currPrice=cp[i][next]+cv[perm[i]][perm[next]];
        if (minPrice>currPrice)
        {
            minPrice=currPrice;
            minPos=i;
        }
    }
    return minPos;
}
void cheapestInsertionSort(int maxInsertions=-1)
{
    int cnt=0;
    int curr=findCheapestInsertion();
    while (cnt!=maxInsertions && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr=findCheapestInsertion();
    }
}
int FCIR_RADIUS=400;
pair<int, int> findCheapestInversionReduction()
{
    pair <int, int> minPos={-1,-1};
    double minPrice=INF;
    double currPrice;
    int inversions;
    for (int i=0;i<n;++i)
    {
        if (perm[i]==i) continue;
        for (int j=i+1;j<min(n,i+FCIR_RADIUS);++j)
        {
            if (perm[j]==j) continue;
            if (perm[i]<perm[j]) continue;
            currPrice=(cp[i][j]+cv[perm[i]][perm[j]]);
            inversions=1;
            for (int k=i+1;k<j;++k)
            {
                if (currPrice>=minPrice*(inversions+2*(j-k))) break;
                ++opCounter;
                if (perm[k]<perm[i] && perm[k]>perm[j]) inversions+=2;
            }
            currPrice/=inversions;
            if (minPrice>currPrice)
            {
                minPrice=currPrice;
                minPos={i,j};
            }
        }
    }
    return minPos;
}
void cheapestInversionReductionSort(int maxInversionReductions=-1)
{
    int cnt=0;
    pair<int, int> curr=findCheapestInversionReduction();
    while (cnt!=maxInversionReductions && curr.first>=0 && curr.second>=0)
    {
        performSwap(curr.first,curr.second);
        ++cnt;
        curr=findCheapestInversionReduction();
    }
}
pair<int, int> findCheapestDistanceReduction()
{
    pair <int, int> minPos={-1,-1};
    double minPrice=INF;
    double currPrice;
    int distance;
    for (int i=0;i<n;++i)
    {
        if (perm[i]==i) continue;
        for (int j=i+1;j<n;++j)
        {
            ++opCounter;
            if (perm[j]==j) continue;
            if (perm[i]<perm[j]) continue;
            currPrice=(cp[i][j]+cv[perm[i]][perm[j]]);
            distance=fabs(i-perm[i])+fabs(j-perm[j])-fabs(j-perm[i])-fabs(i-perm[j]);
            if (distance<=0) continue;
            currPrice/=distance;
            if (minPrice>currPrice)
            {
                minPrice=currPrice;
                minPos={i,j};
            }
        }
    }
    return minPos;
}
void cheapestDistanceReductionSort(int maxDistanceReductions=-1)
{
    int cnt=0;
    pair<int, int> curr=findCheapestDistanceReduction();
    while (cnt!=maxDistanceReductions && curr.first>=0 && curr.second>=0)
    {
        performSwap(curr.first,curr.second);
        ++cnt;
        curr=findCheapestDistanceReduction();
    }
}
int findCheapestIRI()
{
    int minPos=-1;
    double minPrice=INF;
    double currPrice;
    int inversions;
    int next;
    for (int i=0;i<n;++i)
    {
        next=perm[i];
        if (next==i) continue;
        currPrice=cp[i][next]+cv[perm[i]][perm[next]];
        inversions=1;
        for (int k=i+1;k<next;++k)
        {
            ++opCounter;
            if (perm[k]<perm[i] && perm[k]>perm[next]) inversions+=2;
        }
        currPrice/=inversions;
        if (minPrice>currPrice)
        {
            minPrice=currPrice;
            minPos=i;
        }
    }
    return minPos;
}
void cheapestIRISort(int maxIRIs=-1) //inversion reduction by insertion
{
    int cnt=0;
    int curr=findCheapestIRI();
    while (cnt!=maxIRIs && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr=findCheapestIRI();
    }
}
int findCheapestDRI()
{
    int minPos=-1;
    double minPrice=INF;
    double currPrice;
    int distance;
    int next;
    for (int i=0;i<n;++i)
    {
        ++opCounter;
        next=perm[i];
        if (next==i) continue;
        currPrice=cp[i][next]+cv[perm[i]][perm[next]];
        distance=fabs(i-perm[i])+fabs(next-perm[next])-fabs(next-perm[i])-fabs(i-perm[next]);
        if (distance<=0) continue;
        currPrice/=distance;
        if (minPrice>currPrice)
        {
            minPrice=currPrice;
            minPos=i;
        }
    }
    return minPos;
}
void cheapestDRISort(int maxDRIs=-1) //distance reduction by insertion
{
    int cnt=0;
    int curr=findCheapestDRI();
    while (cnt!=maxDRIs && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr=findCheapestDRI();
    }
}
vector<int> cycle;
int cheapestCycleStart()
{
    int minPos=-1;
    int minPrice=INF;
    int curr;
    int other;
    int next;
    int currPrice;
    for (int i=0;i<cycle.size();++i)
    {
        curr=cycle[i];
        currPrice=0;
        for (int j=0;j<cycle.size();++j)
        {
            ++opCounter;
            other=cycle[j];
            next=perm[other];
            currPrice+=cp[curr][other];
            if (next!=curr) currPrice+=cv[perm[other]][perm[next]];
        }
        if (minPrice>currPrice)
        {
            minPrice=currPrice;
            minPos=curr;
        }
    }
    return minPos;
}
void solveCycle(int start, int maxCycle)
{
    int curr=start;
    cycle.resize(0);
    do
    {
        cycle.push_back(curr);
        curr=perm[curr];
    }
    while (curr!=start);
    if (cycle.size()>maxCycle) return;
    curr=cheapestCycleStart();
    while (perm[curr]!=curr)
    {
        performSwap(curr,perm[curr]);
    }
}
void cycleSort(int maxCycle=400)
{
    for (int i=0;i<n;++i)
    {
        if (perm[i]==i) continue;
        //cerr<<"hey"<<endl;
        solveCycle(i,maxCycle);
    }
}
void superSolveCycle(int start, int maxCycle)
{
    int curr=start;
    cycle.resize(0);
    do
    {
        cycle.push_back(curr);
        curr=perm[curr];
    }
    while (curr!=start);
    if (cycle.size()>maxCycle) return;
    curr=cheapestCycleStart();
    performSwap(curr,perm[curr]);
    if (perm[curr]!=curr) superSolveCycle(curr,maxCycle);
}
void superCycleSort(int maxCycle=400)
{
    for (int i=0;i<n;++i)
    {
        if (perm[i]==i) continue;
        //cerr<<"hey"<<endl;
        superSolveCycle(i,maxCycle);
    }
}
void alternatingSort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestInsertion();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestInversionReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestInversionReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        curr=findCheapestInsertion();
        ++cnt2;
    }
}
void alternatingIRISort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestIRI();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestInversionReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestInversionReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        curr=findCheapestIRI();
        ++cnt2;
    }
}
void cheapestInsertionSCSort(int maxInsertions=-1)
{
    int cnt=0;
    int curr=findCheapestInsertion();
    while (cnt!=maxInsertions && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        superCycleSort(ASC_MAX_LEN);
        curr=findCheapestInsertion();
    }
}
void cheapestIRISCSort(int maxIRIs=-1) //inversion reduction by insertion
{
    int cnt=0;
    superCycleSort(ASC_MAX_LEN);
    int curr=findCheapestIRI();
    while (cnt!=maxIRIs && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        superCycleSort(ASC_MAX_LEN);
        curr=findCheapestIRI();
    }
}
void alternatingSCSort(int maxOperations=-1)
{
    int cnt=0;
    superCycleSort(ASC_MAX_LEN);
    int curr=findCheapestInsertion();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        superCycleSort(ASC_MAX_LEN);
        curr2=findCheapestInversionReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            superCycleSort(ASC_MAX_LEN);
            curr2=findCheapestInversionReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        superCycleSort(ASC_MAX_LEN);
        curr=findCheapestInsertion();
        ++cnt2;
    }
}
void alternatingDRSort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestInsertion();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestDistanceReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestDistanceReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        curr=findCheapestInsertion();
        ++cnt2;
    }
}
void alternatingDRSCSort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestInsertion();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestDistanceReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestDistanceReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        superCycleSort(ASC_MAX_LEN);
        curr=findCheapestInsertion();
        ++cnt2;
    }
}
void alternatingDRISort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestDRI();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestInversionReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestInversionReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        curr=findCheapestDRI();
        ++cnt2;
    }
}
void alternatingDRISCSort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestDRI();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestInversionReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestInversionReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        superCycleSort(ASC_MAX_LEN);
        curr=findCheapestDRI();
        ++cnt2;
    }
}
void alternatingDRDRISort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestDRI();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestDistanceReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestDistanceReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        curr=findCheapestDRI();
        ++cnt2;
    }
}
void alternatingDRDRISCSort(int maxOperations=-1)
{
    int cnt=0;
    int curr=findCheapestDRI();
    pair<int, int> curr2;
    int cnt2=0;
    while (cnt!=maxOperations && curr>=0)
    {
        performSwap(curr,perm[curr]);
        ++cnt;
        curr2=findCheapestDistanceReduction();
        if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
        {
            performSwap(curr2.first,curr2.second);
            ++cnt;
        }
        else break;
        if (cnt2%4==0)
        {
            curr2=findCheapestDistanceReduction();
            if (cnt!=maxOperations && curr2.first>=0 && curr2.second>=0)
            {
                performSwap(curr2.first,curr2.second);
                ++cnt;
            }
            else break;
        }
        superCycleSort(ASC_MAX_LEN);
        curr=findCheapestDRI();
        ++cnt2;
    }
}
void bestOfTwo(void (*sortAlg1)(int), int arg1, void (*sortAlg2)(int), int arg2)
{
    vector<Swap> s2;
    int totalPrice2;
    sortAlg1(arg1);
    s2=s;
    totalPrice2=totalPrice;
    reset();
    sortAlg2(arg2);
    if (totalPrice2<totalPrice)
    {
        s=s2;
        totalPrice=totalPrice2;
    }
}
void compositeSort(int ascMaxLen=11)
{
    ASC_MAX_LEN=ascMaxLen;

    if (tp==4 || tv==4)
    {
        if (tv==0) superCycleSort(); //4 0
        else if (tp!=1 && tv!=1) cheapestIRISCSort(); //2 4, 3 4, 4 2, 4 3, 4 4
        else cheapestInsertionSCSort(); //1 4, 4 1
    }
    else if (tp==1 || tv==1)
    {
        if (tp==2 || tv==2) alternatingDRSort(); //1 2, 2 1
        else if (tv==0) alternatingDRSCSort(); // 1 0
        else if (tp==tv) alternatingSCSort(); //1 1
        else bestOfTwo(alternatingSCSort,-1,alternatingDRDRISCSort,-1); //1 3, 3 1
    }
    else bestOfTwo(cheapestDistanceReductionSort,-1, alternatingDRDRISort,-1); //2 0, 2 2, 2 3, 3 0, 3 2, 3 3
}
void solve1()
{
    if (tp==2 && tv==0) FCIR_RADIUS=25;
    ASC_MAX_LEN=11;

    //bubbleSort();
    //cheapestAdjSwapSort();
    //cheapestSwapSort();
    //cycleSort();

    //cheapestInversionReductionSort();
    //cheapestDistanceReductionSort();
    //superCycleSort();
    //cheapestInsertionSort();
    //cheapestIRISort();
    //cheapestDRISort();
    //alternatingSort();
    //alternatingIRISort();

    bestOfTwo(compositeSort,11,compositeSort,14);

    /*cheapestInsertionSort(100); //combo 1
    superCycleSort(200);
    cheapestInsertionSort();*/

    /*cheapestInversionReductionSort(350); //combo 2
    cheapestInsertionSort();*/

    /*alternatingSort(550); //combo 3 //1 0, 1 1, 1 2, 1 3, 3 1 stuff
    //alternatingIRISort(550);
    superCycleSort();*/

    /*cheapestIRISort(100); //combo 4
    superCycleSort(200);
    cheapestIRISort();*/

    cheapestInsertionSort(); //precaution

    //cerr<<">"<<opCounter/1000<<endl;
}

struct Insertion
{
    int pos;
    int cost;
};
bool operator<(const Insertion& a, const Insertion& b)
{
    return a.cost<b.cost;
}
struct PermutationSolver
{
    priority_queue<Insertion> poss;
    int costs[MAX_N];
    int myPerm[MAX_N];
    int antiPerm[MAX_N];
    int totalCost;
    int correct;
    int ops;
    PermutationSolver(const int originalPerm[MAX_N])
    {
        totalCost=0;
        correct=0;
        ops=0;
        //set myPerm and antiPerm
        int val;
        for (int i=0;i<n;++i)
        {
            val=originalPerm[i];
            myPerm[i]=val;
            antiPerm[val]=i;
        }
    }
    void findAllInsertions()
    {
        int next;
        int currPrice;
        for (int i=0;i<n;++i)
        {
            ++ops;
            next=myPerm[i];
            if (next==i)
            {
                ++correct;
                costs[i]=0;
                continue;
            }
            if (myPerm[next]==i && i>next) continue;
            currPrice=cp[i][next]+cv[myPerm[i]][myPerm[next]];
            poss.push({i,currPrice});
            costs[i]=currPrice;
        }
    }
    void doCheapestInsertionFast()
    {
        Insertion toDo;
        int curr, next, prev;
        int currVal, nextVal, prevVal;
        int currPrice;
        int prevPrice;
        while (true)
        {
            ++ops;
            toDo=poss.top();
            poss.pop();
            curr=toDo.pos;
            if (costs[curr]!=toDo.cost) continue;
            totalCost+=toDo.cost;
            ++correct;

            next=currVal=myPerm[curr];
            nextVal=myPerm[next];
            //performSwap(curr,next); //for testing
            swap(myPerm[curr],myPerm[next]);
            swap(antiPerm[currVal],antiPerm[nextVal]);
            costs[next]=0;

            next=myPerm[curr];
            if (next==curr)
            {
                ++correct;
                costs[curr]=0;
                break;
            }
            //currVal=myPerm[curr]; //next
            nextVal=myPerm[next];
            prev=antiPerm[curr];
            //prevVal=myPerm[prev]; //curr

            /// TO DO: optimize - crucial
            currPrice=cp[curr][next]+cv[nextVal][next];
            poss.push({curr,currPrice});
            costs[curr]=currPrice;
            prevPrice=cp[curr][prev]+cv[nextVal][curr];
            poss.push({prev,prevPrice});
            costs[prev]=prevPrice;

            break;
        }
    }
    int solve()
    {
        ops=0;
        findAllInsertions();
        //cerr<<ops<<endl;
        ops=0;
        while (correct<n)
            doCheapestInsertionFast();
        //cerr<<ops<<endl;
        return totalCost;
    }
};
int main()
{
    input();
    //solve1();
    for (int i=0;i<10000;++i)
    {
        PermutationSolver(perm).solve();
    }
    //cheapestInsertionSort();
    output();

    return 0;
}
