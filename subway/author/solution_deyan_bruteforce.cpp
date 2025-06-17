#include "subway.h"
#include <iostream>
#include <vector>
using namespace std;
vector<int> cnt_in, cnt_out;
vector<vector<int> >masks;
vector<int>current_choice;
vector<int>active;
bool flag = false;
void try_all(int id, int n, int m, vector<int> &a, vector<int> &b)
{
    int d=m/n;
    if(flag==true)return;
    if(id==d+1)
    {
        flag=true;
        vector<int> aux;
        for(int i=1;i<=d;i++)
        {
            aux.clear();
            for(int j=0;j<m;j++)
            {
                if(current_choice[j] == i) aux.push_back(j);
            }
            answer(i-1, aux);
        }
        return;
        //chosen all and worked
    }
    for(int i=0;i<masks[id].size();i++)
    {
        vector<int>assigned_here;
        int mask=masks[id][i];
        for(int j=0;j<active.size();j++)
        {
            if(active[j]==0)continue;
            if(mask%2==1)
            {
                assigned_here.push_back(j);
            }
            mask/=2;
        }
        for(int j=0;j<n;j++)
        {
             cnt_in[j]=0;cnt_out[j]=0;
        }
        for(int j=0;j<assigned_here.size();j++)
        {
            cnt_out[a[assigned_here[j]]]++;
            cnt_in[b[assigned_here[j]]]++;
        }
        bool good=true;
        for(int j=0;j<n;j++)
        {
            if(cnt_in[j]!=1 || cnt_out[j]!=1)
            {
                good=false;break;
            }
        }
        if(good)
        {
            for(int j=0;j<assigned_here.size();j++)
            {
                current_choice[assigned_here[j]]=id;
                active[assigned_here[j]]=0;
            }
            try_all(id+1,n,m,a,b);
            for(int j=0;j<assigned_here.size();j++)
            {
                active[assigned_here[j]]=1;
            }
        }
    }
}
bool assign_roads(int n, int m, std::vector<int> a, std::vector<int> b)
{
    cnt_in.resize(n+1);
    int d=m/n;
    cnt_out.resize(n+1);
    for(int i=0;i<n;i++)cnt_in[i]=0;
    for(int i=0;i<n;i++)cnt_out[i]=0;
    for(int i=0;i<m;i++)
    {
        cnt_out[a[i]]++;
        cnt_in[b[i]]++;
    }
    for(int i=0;i<n;i++)
    {
        if(cnt_in[i]!=d)return 0;
        if(cnt_out[i]!=d)return 0;
    }
    active.resize(m);
    current_choice.resize(m);
    for(int i=0;i<m;i++)active[i]=1;
    // need to generate all possible assignments among the m edges to the d companies.
    // generate masks one by one
    // will have m!/((n!)^d d!) assignments, check each in time O(m)
    // for n=5, m = 20 have 4 8886 4376
    // for n=4, m=16 have 2 627 625 seems fine
    // for n=2, m=16 have 2 027 025 seems fine
    // for n=3, m=15 have 1 401 400 seems fine
    // so n,m <= 16 works as a contraint for this
    // equivalent to choosing d masks each having n bits from n, 2n, 3n, etc..
    masks.resize(d+1);
    for(int mask=0; mask <= (1<<m); mask++)
    {
        if(__builtin_popcount(mask) == n)
        {
            for(int i=1;i<=d;i++)
            {
                if(mask < (1<<(n*i)))masks[d-i+1].push_back(mask);
            }
        }
    }
    try_all(1,n,m,a,b);
    return flag;
}