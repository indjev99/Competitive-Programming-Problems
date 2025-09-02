#include<iostream>
#include<assert.h>
#include<vector>
using namespace std;
std::pair <int, int> find_parent (int c, const std::vector <int>& adj) {
    if (c==1) return {-1, -1};
    int newc=3;
    std::vector <int> prv;
    for (int i=0; i<adj.size(); i++) {
        int c=adj[i];
        if ((c==1)||(c==2)) prv.push_back(i);
    }
    assert((prv.size()==1)||(prv.size()==2));
    if (prv.size()==1) return {newc, prv[0]};
    if (adj[prv[0]]==1) return {newc, prv[1]};
    if (adj[prv[1]]==1) return {newc, prv[0]};

    newc=4;
    return {newc, prv[0]};
}
std::pair <int, int> navigate (int c, const std::vector <int>& adj) {
    /*used[vr]=true;
    cout << vr+1 << ", " << colors[vr] << ": ";
    for (auto to : a[vr]) {
        cout << "(" << to+1 << ", " << colors[to] << ") ";
    }
    cout << endl ;*/

    int newc=c;
    if (c==0) {
        bool first=true;
        for (auto c : adj) {
            if (c!=0) first=false;
        }
        if (first==true) newc=1;
        else newc=2;

        for (int i=0; i<adj.size(); i++) {
            int c=adj[i];
            if (c==0) return {newc, i};
        }

        return find_parent(c,adj);
    }

    bool rem=false;
    for (auto c : adj) {
        if (c==0) {
            rem=true;
            break;
        }
    }
    if ((c==4)&&(rem==false)) {
        newc=3;
        for (int i=0; i<adj.size(); i++) {
            int c=adj[i];
            if (c==4) return {newc, i};
        }
        int cnt=0;
        for (int i=0; i<adj.size(); i++) {
            int c=adj[i];
            if ((c==1)||(c==2)) {
                cnt++;
                if (cnt==2) return {newc, i};
            }
        }

        return find_parent(c,adj);
    }
    else if ((c==1)||(c==2)||((c==4)&&(rem==true))) {
        if (c==4) newc=2;
        else {
            int test=-1;
            for (int i=0; i<adj.size(); i++) {
                int c=adj[i];
                if (c==4) {
                    test=i;
                    break;
                }
            }
            if (test!=-1) {
                int cnt=0;
                for (auto c : adj) {
                    if ((c==1)||(c==2)) cnt++;
                }
                if (cnt==1) newc=4;
                return {newc, test};
            }
        }

        for (int i=0; i<adj.size(); i++) {
            int c=adj[i];
            if (c==0) return {newc, i};
        }

        return find_parent(c,adj);
    }
    assert(0);
}
/*int main () {
    int n,m;
    cin >> n >> m ;
    for (int i=0; i<m; i++) {
        int x,y;
        cin >> x >> y ;
        x--; y--;
        a[x].push_back(y);
        a[y].push_back(x);
    }
    dfs(0);
    for (int i=0; i<n; i++) {
        assert(used[i]==true);
    }
    cout << endl ;
    return 0;
}*/
