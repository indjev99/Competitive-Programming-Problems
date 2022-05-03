#include "shoes.h"

#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <random>
#include <iomanip>
#include <numeric>

static const int N = 5000 + 3;

static int n, subtask, n_tests, seed;
static int other_shoe[2 * N];

static long long number_of_queries;

static std::mt19937 generator;

static const long long HASH_BASE = 853;
static const long long HASH_MOD = 7246427;

static long long hashWithInput()
{
    long long hsh = (n + HASH_MOD) % HASH_MOD;
    hsh = (hsh * HASH_BASE + subtask) % HASH_MOD;
    hsh = (hsh * HASH_BASE + n_tests) % HASH_MOD;
    hsh = (hsh * HASH_BASE + seed) % HASH_MOD;
    return hsh;
}

static std::vector<std::pair<int, int>> generatePairs(){
    std::vector<std::pair<int, int>> pairs;
    
    std::vector<int> v(2 * n);
    std::iota(v.begin(), v.end(), 1);
    if(subtask == 1){
        std::shuffle(v.begin(), v.begin() + n, generator);
        std::shuffle(v.begin() + n, v.begin() + 2 * n, generator);
    }   
    else{
        std::shuffle(v.begin(), v.begin() + 2 * n, generator);
    }

    for(int i = 0; i < n; ++i)
        pairs.push_back({v[i], v[i + n]});
    return pairs;
}

static long long inside_v[2 * N];
static long long timer = 0;
bool pairInSet(const std::vector<int> &v){
    ++timer;
    ++number_of_queries;

    for(int x: v){
        if(x < 1 || 2 * n < x || inside_v[x] == timer){
            // Your program used pairInSet() on a set that contained duplicates/out of range numbers.
            std::cout << "-1 " << hashWithInput() << std::endl;
            exit(0); 
        }
        inside_v[x] = timer;
    }

    int cnt = 0;
    for(int x: v)
        cnt += inside_v[other_shoe[x]] == timer;

    return cnt;
}

static long long runOneTest(){
    number_of_queries = 0;

    std::vector<std::pair<int, int>> pairs = generatePairs();
    for(auto &[a, b]: pairs){
        if(a > b){
            std::swap(a, b);
        }
        other_shoe[a] = b;
        other_shoe[b] = a;
    }
    std::sort(pairs.begin(), pairs.end());

    std::vector<std::pair<int, int>> pairs_guess = guessPairs(n);
    if((int) pairs_guess.size() != n){
        // Your program returned a vector that contains too many or too few pairs.
        std::cout << "-2 " << hashWithInput() << std::endl;
        exit(0);
    }

    for(auto [a, b]: pairs_guess){
        if(a < 1 || 2 * n < a || b < 1 || 2 * n < b){
            // Your program returned a pair that contains an out of bound number.
            std::cout << "-3 " << hashWithInput() << std::endl;
            exit(0); 
        }
    }

    static bool mentioned[2 * N];
    std::fill(mentioned + 1, mentioned + 1 + 2 * n, false);
    for(auto [a, b]: pairs_guess){
        mentioned[a] = true;
        mentioned[b] = true;
    }

    for(int i = 1; i <= 2 * n; ++i)
        if(!mentioned[i]){
            // Your program returned pairs that contain the same shoe.
            std::cout << "-4 " << hashWithInput() << std::endl;
            exit(0); 
        }

    for(int i = 0; i < n; ++i){
        if(pairs_guess[i].first > pairs_guess[i].second)
            std::swap(pairs_guess[i].first, pairs_guess[i].second);
    }
    sort(pairs_guess.begin(), pairs_guess.end());

    bool correct = true;
    for(int i = 0; i < n; ++i)
        correct &= pairs_guess[i] == pairs[i];

    if(!correct){
        // Your program didn't return the correct pairs.
        std::cout << "-5 " << hashWithInput() << std::endl;
        exit(0);
    }

    return number_of_queries;
}

int main(){
    std::cin >> n >> subtask >> n_tests >> seed;
    generator.seed(seed);

    long double average_queries = 0;
    for(int i = 0; i < n_tests; ++i)
        average_queries += runOneTest();
    average_queries /= n_tests;

    std::cout << std::fixed << std::setprecision(6) << average_queries << " ";
    std::cout << std::fixed << std::setprecision(0) << hashWithInput() << std::endl;
}