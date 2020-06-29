#include<iostream>
#include<algorithm>
#include<string.h>
#include<stdio.h>
#include<vector>
#include<queue>
using namespace std;
#define MAXN 150000
struct edge {
       int flow,cap;
       int to,rev;
};
vector <edge> a[MAXN+2];
vector <int> orig[MAXN],inds[MAXN];
int dep[MAXN+2],seen[MAXN+2],used[MAXN+2];
int min1,fl,sink,src,maxf,n,m,s,t;
void add_edge (int vr1, int vr2, int t) {
     edge p;
     p.flow=0;

     p.cap=t;
     p.to=vr2; p.rev=a[vr2].size();
     a[vr1].push_back(p);

     p.cap=0;
     p.to=vr1; p.rev=a[vr1].size()-1;
     a[vr2].push_back(p);
}
void dfs (int vr) {
    if (vr==sink) {
       fl=1;
       return ;
       }
    int i;
    for (i=seen[vr]; i<a[vr].size(); i++) {
        if ((dep[a[vr][i].to]-dep[vr]==1)&&(a[vr][i].flow<a[vr][i].cap)) {
           min1=min(min1,(a[vr][i].cap-a[vr][i].flow));
           dfs(a[vr][i].to);
           if (fl==1) {
              a[vr][i].flow+=min1;
              a[a[vr][i].to][a[vr][i].rev].flow-=min1;
              return ;
              }
           }
        seen[vr]++;
        }
}
void max_flow () {
     queue <int> bfs;
     edge p;
     int i,exq;
     for (;;) {
         for (i=0; i<n; i++) {
             seen[i]=dep[i]=0;
             }
         bfs.push(src);
         dep[src]=1;
         for (;;) {
             if (bfs.empty()==1) break;
             exq=bfs.front();
             seen[exq]=0;
             for (i=0; i<a[exq].size(); i++) {
                 if ((dep[a[exq][i].to]==0)&&(a[exq][i].flow<a[exq][i].cap)) {
                    dep[a[exq][i].to]=dep[exq]+1;
                    bfs.push(a[exq][i].to);
                    }
                 }
             bfs.pop();
             }
         if (dep[sink]==0) break;
         for (;;) {
             min1=1e9; fl=0;
             dfs(src);
             if (fl==0) break;
             maxf+=min1;
             }
         }
}

int used_vers[MAXN],other[MAXN],other_ind[MAXN],flag[MAXN],flag_edges[MAXN];
vector <int> ans[6];
void dfs2 (int vr) {
    if (flag[vr]==1) return ;
    flag[vr]=1;
    int i,to;
    for (i=0; i<orig[vr].size(); i++) {
        to=orig[vr][i];
        flag_edges[inds[vr][i]]=1;
        flag_edges[other_ind[to]]=1;
        dfs2(other[to]);
        }
}

vector <int> order;
int comp[MAXN],cnt[MAXN],num=0;
void dfs3 (int vr) {
    int i,to;
    used[vr]=1; //cout << vr+1 << " ";
    for (i=0; i<orig[vr].size(); i++) {
        if (flag_edges[inds[vr][i]]==1) continue;
        to=orig[vr][i];
        if ((vr<s)&&(to==other[vr])) continue;
        if ((vr>=s)&&(to!=other[vr])) continue;
        if (used[to]==0) dfs3(to);
        }
    order.push_back(vr);
}
void dfs4 (int vr) {
    int i,to;
    comp[vr]=num; cnt[num]++;
    used[vr]=1;
    for (i=0; i<orig[vr].size(); i++) {
        if (flag_edges[inds[vr][i]]==1) continue;
        to=orig[vr][i];
        if ((vr<s)&&(to!=other[vr])) continue;
        if ((vr>=s)&&(to==other[vr])) continue;
        if (used[to]==0) dfs4(to);
        }
}

int main () {
    int i,j,h,x,y;
    scanf("%d%d%d",&m,&s,&t);
    n=s+t+2; src=s+t; sink=s+t+1;
    for (i=0; i<m; i++) {
        scanf("%d%d",&x,&y);
        x--; y--;
        add_edge(x,s+y,1);
        orig[x].push_back(s+y); inds[x].push_back(i);
        orig[s+y].push_back(x); inds[s+y].push_back(i);
        }
    for (i=0; i<s; i++) {
        add_edge(src,i,1);
        }
    for (i=0; i<t; i++) {
        add_edge(s+i,sink,1);
        }

    maxf=0;
    max_flow();

    for (i=0; i<a[src].size(); i++) {
        used_vers[a[src][i].to]=a[src][i].flow;
        }
    for (i=0; i<a[sink].size(); i++) {
        used_vers[a[sink][i].to]=-a[sink][i].flow;
        }

    for (i=0; i<s; i++) {
        for (j=0; j<a[i].size(); j++) {
            if ((a[i][j].cap==1)&&(a[i][j].flow==1)) {
               other[i]=a[i][j].to;
               other[a[i][j].to]=i;
               for (h=0; h<orig[i].size(); h++) {
                   if (orig[i][h]==a[i][j].to) {
                      other_ind[i]=other_ind[a[i][j].to]=inds[i][h];
                      break;
                      }
                   }
               //cout << maxf << " " << i+1 << " " << a[i][j].to+1 << endl ;
               break;
               }
            }
        }

    for (i=0; i<s+t; i++) {
        if (used_vers[i]==0) {
           for (j=0; j<orig[i].size(); j++) {
               //cout << i+1 << " " << orig[i][j]+1 << " " << other[orig[i][j]]+1 << endl ;
                //cout << other[orig[i][j]]+1 << " ";
               flag_edges[inds[i][j]]=1;
               flag_edges[other_ind[orig[i][j]]]=1;
               dfs2(other[orig[i][j]]);
               }
           }
        }
    for (i=0; i<s+t; i++) {
        if (used_vers[i]==1) {
           if (flag[i]==0) {
              if (i<s) ans[2].push_back(i);
              else ans[4].push_back(i);
              }
           }
        else if (orig[i].size()==0) {
           if (i<s) ans[3].push_back(i);
           else ans[5].push_back(i);
           }
        }
    memset(used,0,sizeof(used));
    for (i=0; i<n-2; i++) {
        if (used[i]==0) dfs3(i);
        }
    memset(used,0,sizeof(used));
    for (i=order.size()-1; i>=0; i--) {
        if (used[order[i]]==0) {
           dfs4(order[i]);
           num++;
           }
        }

    for (i=0; i<s; i++) {
        for (j=0; j<orig[i].size(); j++) {
            if (flag_edges[inds[i][j]]==1) {
                    //cout << i+1 << " " << orig[i][j]+1 << endl ;
            continue; }
            //cout << i+1 << " " << orig[i][j]+1 << " " << comp[i] << " " << comp[orig[i][j]] << " " << cnt[comp[i]] << endl ;
            if ((other[i]==orig[i][j])&&(cnt[comp[i]]<=2)) ans[0].push_back(inds[i][j]);
            else if ((other[i]!=orig[i][j])&&(comp[i]!=comp[orig[i][j]])) ans[1].push_back(inds[i][j]);
            }
        }

    printf("%d",maxf);
    for (i=0; i<6; i++) {
        printf(" %d",ans[i].size());
        }
    printf("\n");
    sort(ans[0].begin(),ans[0].end());
    sort(ans[1].begin(),ans[1].end());
    for (i=0; i<6; i++) {
        for (auto num : ans[i]) {
            if (i<=3) printf("%d ",num+1);
            else printf("%d ",num+1-s);
            }
        printf("\n");
        }
}
