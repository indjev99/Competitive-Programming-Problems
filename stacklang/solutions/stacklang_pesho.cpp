#include<bits/stdc++.h>
using namespace std;

int n, m;
int startVertex, endVertex;
vector<pair<int, int>> edges[100000];

void read() {
    cin >> n >> m;
    cin >> startVertex >> endVertex;
    for (int i = 0; i < m; i++) {
        int u, v, w; cin >> u >> v >> w;
        edges[u].push_back({w, v});
    }
    for (int i = 0; i < n; i++) {
        sort(edges[i].begin(), edges[i].end());
    }
}

void getEdges(int u, stack<int>& st) {
    for (auto& p: edges[u]) {
        st.push(p.second);
        st.push(p.first);
    }
    edges[u].clear();
}

int main() {
    read();

    stack<int> st1, st2, st3;
    st1.push(-1); st2.push(-1);
    st2.push(startVertex);
    st2.push(0);
    st3.push(endVertex);

    while (st2.top() != -1) {
        st1.push(st2.top()); st2.pop();
        if (st2.top() == st3.top()) {
            cout << st1.top() << endl;
            return 0;
        }
        st3.push(st1.top()); st1.pop();
        st3.push(st2.top()); st2.pop();

        st2.push(-1);
        getEdges(st3.top(), st2); st3.pop();
        while (st2.top() != -1) {
            st1.push(st2.top()+st3.top()); st2.pop();
            st1.push(st2.top()); st2.pop();
        }
        st2.pop();
        while (st1.top() != -1) {
            st2.push(st1.top()); st1.pop();
            if (st1.top() != st3.top()) {
                st1.push(st2.top()); st2.pop();
                break;
            }
            st2.push(st1.top()); st1.pop();
        }

        st3.pop();

        if (st2.top() == -1) {
            while (st1.top() != -1) {
                st2.push(st1.top()); st1.pop();
                st2.push(st1.top()); st1.pop();
            }
        }
    }
    cout << -1 << endl;

    return 0;
}
