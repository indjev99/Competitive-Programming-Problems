#include "multisort.h"
#include <algorithm>
using namespace std;

void multisort(vector<int>& v, int l, int r, int k) {
    if (l >= r) return;
    if (r-l+1 <= k) {
        vector<int> c = vector<int>(v.begin()+l, v.begin()+r+1);
        compare(c);
        for (int i = l; i <= r; i++) v[i] = c[i-l];
        return;
    }

    random_shuffle(v.begin()+l, v.begin()+r+1);

    int pivot = v[l];
    vector<int> ranges[2];
    for (int i = l+1; i <= r; i+=k-1) {
        vector<int> t = vector<int>(v.begin()+i, v.begin()+min(i+k-2, r)+1);
        t.push_back(pivot);
        compare(t);
        int index = 0;
        for (int x: t) {
            if (x == pivot) index++;
            else ranges[index].push_back(x);
        }
    }

    int br = l;
    int index = l;
    for (int i = 0; i < 2; i++) {
        for (int x: ranges[i]) v[br++] = x;
        if (i == 0) {
            index = br;
            v[br++] = pivot;
        }
    }

    multisort(v, l, index-1, k);
    multisort(v, index+1, r, k);
}

vector<int> multisort(int n, int k) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;

    multisort(v, 0, v.size()-1, k);
    return v;
}
