#include <vector>
#include <utility>
#include <cassert>
#include <iostream>

static int _n;
static std::vector<bool> _a;
static long long num_queries;

static int longest_seq_of_1s(){
    int ans = 0, curr = 0;
    for(int i = 0; i < _n; ++i){
        if(_a[i]){
            ++curr;
        }
        else{
            curr = 0;
        }
        ans = std::max(ans, curr);
    }
    return ans;
}

int flip_bits(const std::vector<bool> &flips){
    assert((int)flips.size() == _n);
    ++num_queries;
    for(int i = 0; i < _n; ++i)
        _a[i] = flips[i] ^ _a[i];
    return longest_seq_of_1s();
}

std::pair<int, int> find_longest_subarray_of_ones(int n);

int main(){
    int t;
    std::cin >> t;

    long long max_qs = 0;

    while(t--){
        num_queries = 0;

        std::cin >> _n;
        _a.resize(_n);
        for(int i = 0; i < _n; ++i){
            bool val;
            std::cin >> val;
            _a[i] = val;
        }

        auto [l, r] = find_longest_subarray_of_ones(_n);
        if(longest_seq_of_1s() != r - l + 1){
            std::cout << "0\n";
            return 0;
        }
        if(l == 0 && r == -1){
            max_qs = std::max(num_queries, max_qs);
            continue;
        }
        if(r < l || l < 0 || l >= _n || r < 0 || r >= _n){
            std::cout << "0\n";
            return 0;
        }

        for(int i = l; i <= r; ++i){
            if(_a[i] != 1){
                std::cout << "0\n";
                return 0;
            }
        }

        max_qs = std::max(num_queries, max_qs);
    }

    std::cout << "1 " << max_qs << "\n";
    return 0;
}