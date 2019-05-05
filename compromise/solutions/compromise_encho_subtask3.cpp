#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
using namespace std;
typedef long long llong;

const llong INF = 1000000000000000001LL;
int n,m;
pair<int,int> edg[500111];
llong val[500111];

pair<llong,int> IT[2000111];
int LEAFOFFSET;

pair<llong,int> MAX(pair<llong,int> A, pair<llong,int> B)
{
    if (A.first > B.first)
        return A;
    else
        return B;
}

pair<int,int> qs[500111];
int q = 0;

pair<llong,int> recMax(int ver,int L,int R,int l,int r)
{
    if (L > r || R < l)
        return make_pair(-1,0);
    else if (L >= l && R <= r)
        return IT[ver];
    else
        return MAX( recMax(2*ver, L, (L+R)/2, l, r), recMax(2*ver+1, (L+R)/2+1, R, l, r) );
}

int getMax(int L,int R)
{
    return recMax(1,1,LEAFOFFSET+1,L,R).second;
}

llong getMaxVal(int L,int R)
{
    return recMax(1,1,LEAFOFFSET+1,L,R).first;
}

void gen(int L,int R)
{
    if (L > R)
        return;

    int ind = getMax(L,R);

    q++;
    qs[q] = make_pair(L, R);

    gen(L, ind-1);
    gen(ind+1, R);
}

///DSU
int father[500111];
int rnk[500111];

int Find_compress(int ver)
{
    if (father[ver] == 0)
        return ver;
    else
    {
        father[ver] = Find_compress(father[ver]);
        return father[ver];
    }
}

int Find_nocompress(int ver)
{
    if (father[ver] == 0)
        return ver;
    else
        return Find_nocompress(father[ver]);
}

int Union_forget(int r1,int r2)
{
    if (rnk[r1] > rnk[r2])
    {
        father[r2] = r1;

        return r1;
    }
    else
    {
        father[r1] = r2;
        rnk[r2] += (int)(rnk[r1] == rnk[r2]);
        return r2;
    }
}

pair<int,int> f_rem[500111];
pair<int,int> r_rem[500111];
int remL = 0;

int Union_remember(int r1,int r2)
{
    remL++;

    if (rnk[r1] > rnk[r2])
    {
        f_rem[remL] = make_pair(r2, father[r2]);
        r_rem[remL] = make_pair(r1, rnk[r1]);

        father[r2] = r1;
        return r1;
    }
    else
    {
        f_rem[remL] = make_pair(r1, father[r1]);
        r_rem[remL] = make_pair(r2, rnk[r2]);

        father[r1] = r2;
        rnk[r2] += (int)(rnk[r1] == rnk[r2]);
        return r2;
    }
}

void Undo()
{
    father[ f_rem[remL].first ] = f_rem[remL].second;
    rnk[ r_rem[remL].first ] = r_rem[remL].second;
    remL--;

    return;
}

const int BUCKET = 300;
vector< pair<int,int> > buckets[50111];
vector<int> answers[50111];

int main()
{
    int i,j,in;
    int lstb = 0;

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
        scanf("%d %d %lld",&edg[i].first,&edg[i].second,&val[i]);

        IT[i+LEAFOFFSET] = make_pair(val[i], i);
    }

    for (i=LEAFOFFSET;i>=1;i--)
    {
        IT[i] = MAX(IT[2*i], IT[2*i+1]);
    }

    gen(1,m);

    for (i=1;i<=q;i++)
    {
        int bnd = (qs[i].first-1)/BUCKET + 1;

        buckets[bnd].push_back(make_pair(qs[i].second, qs[i].first));

        if (bnd > lstb)
            lstb = bnd;
    }

    for (i=1;i<=lstb;i++)
    {
        sort(buckets[i].begin(), buckets[i].end());

        memset(father,0,sizeof(father));
        memset(rnk,0,sizeof(rnk));

        int bend = i * BUCKET + 1;

        int cur = 0;
        while(cur < buckets[i].size() && buckets[i][cur].first < bend)
        {

            int ctr = 0;
            for (j=buckets[i][cur].second;j<=buckets[i][cur].first;j++)
            {
                int r1 = Find_nocompress( edg[j].first );
                int r2 = Find_nocompress( edg[j].second );

                if (r1 != r2)
                {
                    ctr++;
                    Union_remember(r1, r2);
                }
            }

            answers[i].push_back(n - ctr);

            while(ctr > 0)
            {
                ctr--;
                Undo();
            }

            cur++;
        }

        int rp = bend - 1;
        int compctr = 0;
        for (j=cur;j<buckets[i].size();j++)
        {
            while(rp < buckets[i][j].first)
            {
                rp++;

                int r1 = Find_compress( edg[rp].first );
                int r2 = Find_compress( edg[rp].second );

                if (r1 != r2)
                {
                    compctr++;
                    Union_forget(r1, r2);
                }
            }

            int minctr = 0;
            for (in=bend-1;in>=buckets[i][j].second;in--)
            {
                int r1 = Find_nocompress( edg[in].first );
                int r2 = Find_nocompress( edg[in].second );

                if (r1 != r2)
                {
                    minctr++;
                    Union_remember(r1, r2);
                }
            }

            answers[i].push_back(n - (compctr+minctr));

            while(minctr > 0)
            {
                minctr--;
                Undo();
            }
        }
    }

    llong res = -1;

    for (i=1;i<=lstb;i++)
    {
        for (j=0;j<answers[i].size();j++)
        {
            llong cres = (llong)answers[i][j] * (llong)getMaxVal(buckets[i][j].second, buckets[i][j].first);\

            if (res == -1 || res > cres)
            {
                res = cres;
            }
        }
    }

    printf("%lld\n",res);

    return 0;
}
