#include <iostream>
#include <stdio.h>
using namespace std;
typedef long long llong;

pair<llong,int> MAX(pair<llong,int> A, pair<llong,int> B)
{
    if (A.first > B.first)
        return A;
    else
        return B;
}

int n,m;
pair<llong,int> edges[500111];
llong vals[500111];
pair<llong,int> IT[2000111];
int LEAFOFFSET;

pair<llong,int> recMax(int ver,int L,int R,int l,int r)
{
    if (L > r || R < l)
        return make_pair(-1, 0);
    else if (L >= l && R <= r)
        return IT[ver];
    else
        return MAX(recMax(2*ver, L, (L+R)/2, l, r), recMax(2*ver+1, (L+R)/2+1, R, l, r));
}

int maxInd(int L,int R)
{
    return recMax(1, 1, LEAFOFFSET+1, L, R).second;
}

int root;
pair<int,int> verq[500111];
llong verval[500111];
int verind[500111];
int s1[500111];
int s2[500111];
int treeCtr = 0;

int gen(int L,int R)
{
    if (L > R)
        return 0;

    int ind = maxInd(L, R);

    treeCtr++;
    int curVer = treeCtr;
    verq[curVer] = make_pair(L, R);
    verval[curVer] = vals[ind];
    verind[curVer] = ind;

    s1[curVer] = gen(L, ind - 1);
    s2[curVer] = gen(ind + 1, R);

    return curVer;
}

int Key = 1;
int Age[500111];
int father[500111];
int components = 0;

void newAge()
{
    Key++;
    components = n;
}

inline void RefreshAge(int ver)
{
    if (Age[ver] != Key)
    {
        Age[ver] = Key;
        father[ver] = 0;
    }
}

int Find(int ver)
{
    RefreshAge(ver);

    if (father[ver] == 0)
        return ver;
    else
    {
        father[ver] = Find(father[ver]);
        return father[ver];
    }
}

inline void Union(int r1,int r2)
{
    components--;
    father[r1] = r2;
}

void Insert(int ind)
{
    int v1 = edges[ind].first;
    int v2 = edges[ind].second;

    int r1 = Find(v1);
    int r2 = Find(v2);

    if (r1 != r2)
    {
        Union(r1, r2);
    }
}

llong score = -1;

void solve(int ver)
{
    if (ver == 0)
        return;

    int k1 = s1[ver], k2 = s2[ver];
    pair<int,int> toadd;

    if (verq[k1].second - verq[k1].first > verq[k2].second - verq[k2].first)
    {
        solve(k2);
        newAge();
        solve(k1);
        toadd = verq[k2];
    }
    else
    {
        solve(k1);
        newAge();
        solve(k2);
        toadd = verq[k1];
    }

    Insert(verind[ver]);

    int i;
    for (i=toadd.first;i<=toadd.second;i++)
    {
        Insert(i);
    }

    llong ans = (llong)verval[ver] * (llong)components;

    if (ans < score || score == -1)
        score = ans;
}

int main()
{
    int i,j;

    scanf("%d %d",&n,&m);

    LEAFOFFSET = 1;
    while(LEAFOFFSET < m)
        LEAFOFFSET *= 2;
    LEAFOFFSET--;

    for (i=LEAFOFFSET+1;i<=2*LEAFOFFSET+1;i++)
    {
        IT[i] = make_pair(-1, 0);
    }

    for (i=1;i<=m;i++)
    {
        scanf("%d %d %lld",&edges[i].first,&edges[i].second,&vals[i]);

        IT[i+LEAFOFFSET] = make_pair(vals[i], i);
    }

    for (i=LEAFOFFSET;i>=1;i--)
    {
        IT[i] = MAX(IT[2*i], IT[2*i+1]);
    }

    root = gen(1,m);

    newAge();
    solve(root);

    printf("%lld\n",score);

    return 0;
}
