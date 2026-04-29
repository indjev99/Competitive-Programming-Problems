#include<bits/stdc++.h>
#define MAXN 1007
#include "triangle.h"

using namespace std;

bool q(int a, int b, int c) { return query(a - 1, b - 1, c - 1); }

vector<int> ans;
int n,perm[MAXN],m;
int pos[MAXN];

int find_one(){

    int pos=1;
    for(int i=2;i<=n;i++){
        if(!q(i,i,pos))pos=i;
    }

    return pos;
}

pair<int,int> find_two_three(){
    pair<int,int> w;
    
    int pos=1;
    if(perm[1]==1)pos++;

    for(int i=1;i<=n;i++){
        if(i==pos or perm[i]==1)continue;
        if(!q(i,i,pos))pos=i;
    }

    w.first=pos;

    pos=n;
    if(perm[n]==1)pos--;

    for(int i=n;i>=1;i--){
        if(i==pos or perm[i]==1)continue;
        if(!q(i,i,pos))pos=i;
    }

    w.second=pos;

    return w;
}

int find_four(int a,int b){
    for(int i=1;i<=n;i++){
        if(perm[i]>0)continue;
        if(q(a,b,i))return i;
    }
}

void recur(int l,int r,vector<int> poss){
    if(l==r){
        perm[poss[0]]=l;
        pos[l]=poss[0];
        return;
    }

    int mid=(l+r)/2;

    vector<int> ll,rr;
    for(int i:poss){
        if(q(pos[m],pos[mid+1-m],i))ll.push_back(i);
        else rr.push_back(i);
    }

    recur(l,mid,ll);
    recur(mid+1,r,rr);
}

vector<int> solve(int N){
    n=N;

    for(int i=1;i<=n;i++)perm[i]=pos[i]=0;
    
    int x=find_one();
    perm[x]=1; pos[1]=x;

    pair<int,int> s=find_two_three();
    if(s.first==s.second){
        perm[s.first]=2; pos[2]=s.first; m=2;
    }else{
        perm[s.first]=2; perm[s.second]=3;
        pos[2]=s.first; pos[3]=s.second;

        int y=find_four(s.first,s.second);

        perm[y]=4; pos[4]=y;
        
        if(q(s.first,s.first,y)){
            swap(perm[s.first],perm[s.second]);
            swap(pos[2],pos[3]);
        }

        m=4;
    }

    while(m<n){
        vector<int> nxt;
        for(int i=1;i<=n;i++){
            if(perm[i]>0)continue;

            if(m>n/2 or q(pos[m],pos[m],i))nxt.push_back(i);
        }

        recur(m+1,m+int(nxt.size()),nxt);
        m+=int(nxt.size());
    }

    ans.clear();
    for(int i=1;i<=n;i++)ans.push_back(perm[i]);

    return ans;
}