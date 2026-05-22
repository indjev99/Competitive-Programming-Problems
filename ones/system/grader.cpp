#include <bits/stdc++.h>

using namespace std;

static int _n;
static vector<bool> _a;
static long long num_queries;

const long long HASH_BASE = 713;
const long long HASH_MOD = 7246427;

static long long hsh = 0;
static void update_hash(long long x){
    hsh *= HASH_BASE;
    hsh += x;
    hsh %= HASH_MOD;
}

static int longest_seq_of_1s(){
    int ans = 0, curr = 0;
    for(int i = 0; i < _n; ++i){
        if(_a[i]){
            ++curr;
        }
        else{
            curr = 0;
        }
        ans = max(ans, curr);
    }
    return ans;
}

int flip_bits(const vector<bool> &flips){
    assert((int)flips.size() == _n);
    ++num_queries;
    for(int i = 0; i < _n; ++i)
        _a[i] = flips[i] ^ _a[i];
    return longest_seq_of_1s();
}

pair<int, int> find_longest_subarray_of_ones(int n);

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;

    long long max_qs = 0;

    while(t--){
        num_queries = 0;

        cin >> _n;
        _a.resize(_n);
        for(int i = 0; i < _n; ++i){
            bool val;
            cin >> val;
            _a[i] = val;
            update_hash(val);
        }

        auto [l, r] = find_longest_subarray_of_ones(_n);
        if(longest_seq_of_1s() != r - l + 1){
            cout << "0\n";
            return 0;
        }
        if(l == 0 && r == -1){
            max_qs = max(num_queries, max_qs);
            continue;
        }
        if(r < l || l < 0 || l >= _n || r < 0 || r >= _n){
            cout << "0\n";
            return 0;
        }

        for(int i = l; i <= r; ++i){
            if(_a[i] != 1){
                cout << "0\n";
                return 0;
            }
        }

        max_qs = max(num_queries, max_qs);
    }

    cout << "1 " << max_qs << " " << hsh << "\n";
    return 0;
}