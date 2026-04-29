#include "triangle.h"
#include<bits/stdc++.h>
using namespace std;

/*
static const int MAXN = 1e3 + 10;
static int perm[MAXN];
static int n;

int queries = 0;
bool query(int a, int b, int c)
{
    if (!(1 <= a && a <= n) || !(1 <= b && b <= n) || !(1 <= c && c <= n))
    {
        assert(false);
    }

    queries++;

    return perm[a] + perm[b] > perm[c] && perm[b] + perm[c] > perm[a] && perm[c] + perm[a] > perm[b];
}
*/

map< pair< pair<int,int>, int>, bool> cached;

int confirmedOne;

vector< pair<int,int> > bounds;

int _query(int a,int b,int c)
{
    if(a>b)swap(a,b);
    if(a>c)swap(a,c);
    if(b>c)swap(b,c);

    //not all 3 are equal by construction

    if((a == confirmedOne && b == confirmedOne) || (b == confirmedOne && c == confirmedOne) || (c == confirmedOne && a == confirmedOne))
        return 0;

    if(a == confirmedOne)
        return b == c;

    if(b == confirmedOne)
        return c == a;

    if(c == confirmedOne)
        return a == b;

    if(bounds[a].second + bounds[b].second <= bounds[c].first ||
       bounds[b].second + bounds[c].second <= bounds[a].first ||
       bounds[c].second + bounds[a].second <= bounds[b].first)
        return 0;

    if(bounds[a].first + bounds[b].first > bounds[c].second &&
       bounds[b].first + bounds[c].first > bounds[a].second &&
       bounds[c].first + bounds[a].first > bounds[b].second)
        return 1;

    pair< pair<int,int>, int> state={{a,b},c};

    if(cached.count(state))
        return cached[state];

    cached[state]=query(a-1,b-1,c-1);

    return cached[state];
}

vector<int> output,valueToPosition;

mt19937 rng(51);

void fixOrdered(vector<int> ordered, vector<int> large)
{
    if(large.size() == 0)
        return;

    if(large.size() == 1)
    {
        int value = ordered.size() + 1;

        output[large[0]] = value;
        valueToPosition[value] = large[0];

        bounds[large[0]]={value,value};

        return;
    }

    int N = ordered.size() + large.size();

    int other = min(ordered.size() - 1, large.size() / 2);

    int sizeNewSmall = other;
    int sizeNewLarge = large.size() - sizeNewSmall;

    vector<int> newSmall={};
    vector<int> newLarge={};

    for(auto id:large)
    {
        if(newSmall.size() == sizeNewSmall)
            newLarge.push_back(id);
        else if(newLarge.size() == sizeNewLarge)
            newSmall.push_back(id);
        else if(_query(ordered.back(), ordered[other], id))
            newSmall.push_back(id);
        else
            newLarge.push_back(id);
    }

    for(auto idI:newSmall)
        bounds[idI].second = min(bounds[idI].second, int(ordered.size()) + sizeNewSmall);

    for(auto idI:newLarge)
        bounds[idI].first = max(bounds[idI].first, int(ordered.size()) + sizeNewSmall + 1);

    fixOrdered(ordered, newSmall);

    for(int i=1;i<=sizeNewSmall;i++)
        ordered.push_back(valueToPosition[ordered.size() + 1]);

    fixOrdered(ordered, newLarge);
}

void getOrdered(vector<int> active)
{
    shuffle(active.begin(),active.end(),rng); // difference of < 5

    if(active.size()==1)
    {
        output[active[0]]=1;
        valueToPosition[1]=active[0];

        bounds[valueToPosition[1]]={1,1};
        return;
    }

    if(active.size()==2)
    {
        if(_query(active[0],active[1],active[1]))
        {
            valueToPosition[1]=active[0];
            valueToPosition[2]=active[1];
        }

        else
        {
            valueToPosition[1]=active[1];
            valueToPosition[2]=active[0];
        }

        output[valueToPosition[1]]=1;
        output[valueToPosition[2]]=2;

        bounds[valueToPosition[1]]={1,1};
        bounds[valueToPosition[2]]={2,2};

        return;
    }

    int N = active.size();

    int C = 11;

    if(N >= C)
    {
        int steps = 1 + log2(N);
        steps = min(steps, N);

        int i = 0;
        for(int j = 1; j < steps ; j++)
            if(!_query(active[i], active[i], active[j]))
                i = j;

        int mem = active[0];

        active[0] = active[i];
        active[i] = mem;
    }

    for(auto idI:active)
    {
        vector<int> small={},large={idI};

        for(auto idJ:active)
            if(idI != idJ)
            {
                if(_query(idI, idJ, idJ))
                    large.push_back(idJ);
                else
                    small.push_back(idJ);
            }

        bounds[idI]={2 * small.size(), 2 * small.size() + 1};

        if(small.size()==0)
        {
            confirmedOne=idI;
            continue;
        }

        if(small.size()==1 || small.size()==3)
        {
            continue;
        }

        getOrdered(small);

        vector<int> ordered={};

        for(int i=1;i<=small.size();i++)ordered.push_back(valueToPosition[i]);

        fixOrdered(ordered,large);

        return;
    }

    assert(0);
}

std::vector <int> solve(int N)
{
    bounds={};

    for(int i=0;i<=N;i++)
        bounds.push_back({1,N});

    confirmedOne=0;

    cached={};

    output={};
    for(int i=0;i<=N;i++)
        output.push_back(0);

    valueToPosition=output;

    vector<int> active={};
    for(int i=1;i<=N;i++)
        active.push_back(i);

    getOrdered(active);

    output.erase(output.begin(),output.begin()+1);

    return output;
}

/*
int main()
{
    int totalQueries=0;

    int T;
    std::cin >> T;
    for (int t = 1 ; t <= T ; ++t)
    {
        std::cin >> n;
        for (int i = 1 ; i <= n ; ++i)
        {
            //std::cin >> perm[i];
            perm[i] = n + 1 - i;
        }

        queries = 0;

        std::vector <int> ans = solve(n);

        cout<<queries<<endl;
        for (const int &val : ans) std::cout << val << ' ';
        std::cout << '\n';

        totalQueries += queries;
    }

    cout<< totalQueries * 1.0 / T <<endl;

    return 0;
}
*/
