#include<bits/stdc++.h>
#define MAXN 1007
#include "triangle.h"

using namespace std;

bool q(int a, int b, int c) { return query(a - 1, b - 1, c - 1); }

vector<int> ans;
int n,perm[MAXN];
int pos[MAXN];

bool res[MAXN][MAXN];

vector<int> solve(int N){
    n=N;

    for(int i=1;i<=n;i++)perm[i]=0;

    for(int i=1;i<=n;i++){
        int bad=0;

        for(int f=1;f<=n;f++){
            if(i==f)continue;

            res[i][f]=q(i,i,f);
            if(!res[i][f])bad++;
        }

        if(bad>0){
            perm[i]=(n-bad)/2+1;
            pos[perm[i]]=i;
        }
    }

    for(int i=1;i<=n;i++){
        if(perm[i]>0)continue;

        for(int f=n/2;f>=1;f--){
            if(!res[pos[f]][i]){
                if(f==n/2 or q(pos[f],pos[f+1],i))perm[i]=n-(n/2-f)*2;
                else perm[i]=n-(n/2-f)*2+1;
                break;
            }
        }
    }

    ans.clear();
    for(int i=1;i<=n;i++)ans.push_back(perm[i]);

    return ans;
}