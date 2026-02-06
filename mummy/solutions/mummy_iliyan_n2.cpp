#include "mummy.h"
#include<iostream>
#include<algorithm>
#include<assert.h>
#include<vector>
#include<random>
#include<set>
using namespace std;
const int MAXN=512;
vector <int> vals[MAXN];
int pos[MAXN];
bool guessed[MAXN];
int last;
vector <int> perm;
int query (vector <int>& perm) {
    return last=numMatches(perm);
}
void remove (vector <int>& v, int ind) {
    swap(v[ind],v.back());
    v.pop_back();
}
void swap (int ind1, int ind2, bool fin) {
    swap(perm[ind1],perm[ind2]);
    if (fin==true) swap(pos[perm[ind1]],pos[perm[ind2]]);
}
set <int> remaining;
void mark_guessed (vector <int> inds) {
    for (auto ind : inds) {
        guessed[perm[ind]]=true;
        remaining.erase(ind);
    }
}
int ans[MAXN];
vector <int> findPerm (int n) {
    mt19937 rnd(42);
    perm.resize(n);
    for (int i=0; i<n; i++) {
        perm[i]=i;
    }
    for (;;) {
        shuffle(perm.begin(),perm.end(),rnd);
        if (query(perm)==0) break;
    }
    for (int i=0; i<n; i++) {
        vals[i].reserve(n-1);
        vals[i].clear();
        for (int j=0; j<n; j++) {
            if (j==perm[i]) continue;
            vals[i].push_back(j);
        }
        shuffle(vals[i].begin(),vals[i].end(),rnd);
    }
    for (int i=0; i<n; i++) {
        guessed[i]=false;
        pos[perm[i]]=i;
        remaining.insert(i);
    }

    int cnt=0;
    vector <pair <int, int>> curr;
    curr.reserve(n);
    for (;;) {
        /*cout << cnt << endl ;
        for (int i=0; i<n; i++) {
            cout << perm[i] << " ";
        }
        cout << endl ;*/
        curr.clear();
        for (int i=0; i<n; i++) {
            int v=perm[i];
            if (guessed[v]==true) continue;
            /*for (int j=vals[i].size()-1; j>=0; j--) {
                int other=vals[i][j];
                if (guessed[other]==false) cout << i << " " << other << "  ";
            }*/
            for (int j=vals[i].size()-1; j>=0; j--) {
                int other=vals[i][j];
                if (guessed[other]==true) remove(vals[i],j);
                else {
                    remove(vals[i],j);
                    int ind=pos[other];
                    for (int h=0; h<vals[ind].size(); h++) {
                        if (vals[ind][h]==v) {
                            remove(vals[ind],h);
                            break;
                        }
                    }
                    curr.push_back({i, pos[other]});
                    break;
                }
            }
            break;
        }
        for (auto p : curr) {
            swap(p.first,p.second,false);
        }
        query(perm);
        assert(last>=cnt);
        if (last==n) break;
        for (auto p : curr) {
            swap(p.first,p.second,false);
        }

        if (last>cnt) {
            int p1=curr[0].first,p2=curr[0].second;
            swap(p1,p2,true);
            //cout << p1 << " " << p2 << " " << query(perm) << endl ;
            if (last==cnt+2) {
                mark_guessed({p1, p2});
                cnt+=2;
            }
            else {
                int p3=-1;
                for (auto p : remaining) {
                    if ((p!=p1)&&(p!=p2)) {
                        p3=p;
                        break;
                    }
                }
                swap(p1,p3,false);
                query(perm);
                swap(p1,p3,false);
                if (last==cnt) mark_guessed({p1});
                else mark_guessed({p2});
                cnt++;
            }
        }
    }
    return perm;
}
