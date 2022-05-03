#include "shoes.h"

#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <random>
#include <iomanip>
#include <numeric>

static const int N = 5000 + 3;

static int n, n_tests;
static int other_shoe[2 * N];

static long long number_of_queries;

static bool inside_v[2 * N];
bool pairInSet(const std::vector<int> &v){
    ++number_of_queries;

    for(int x: v){
        if(x < 1 || 2 * n < x || inside_v[x]){
            std::cout << "Your program used pairInSet() on a set that contained duplicates/out of range numbers." << std::endl;
            exit(0); 
        }
        inside_v[x] = true;
    }

    int cnt = 0;
    for(int x: v)
        cnt += inside_v[other_shoe[x]];

    for(int x: v)
        inside_v[x] = false;

    return cnt;
}

static long long runOneTest(){
    number_of_queries = 0;

    std::vector<std::pair<int, int>> pairs;
    for(int i = 0; i < n; ++i){
        int a, b;
        std::cin >> a >> b;
        if(a > b){
            std::swap(a, b);
        }
        pairs.push_back({a, b});
        other_shoe[a] = b;
        other_shoe[b] = a;
    }
    std::sort(pairs.begin(), pairs.end());

    std::vector<std::pair<int, int>> pairs_guess = guessPairs(n);
    if((int) pairs_guess.size() != n){
        std::cout << "Your program returned a vector that contains too many or too few pairs." << std::endl;
        exit(0);
    }

    for(auto [a, b]: pairs_guess){
        if(a < 1 || 2 * n < a || b < 1 || 2 * n < b){
            std::cout << "Your program returned a pair that contains an out of bound number." << std::endl;
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
            std::cout << "Your program returned pairs that contain the same shoe." << std::endl;
            exit(0); 
        }

    for(int i = 0; i < n; ++i){
        if(pairs_guess[i].first > pairs_guess[i].second)
            std::swap(pairs_guess[i].first, pairs_guess[i].second);
    }
    sort(pairs_guess.begin(), pairs_guess.end());

    bool correct = true;
    for(int i = 0; i < n; ++i){
        correct &= pairs_guess[i] == pairs[i];
    }

    if(!correct){
        std::cout << "Your program didn't return the correct pairs." << std::endl;
        exit(0);
    }

    return number_of_queries;
}

int main(){
    std::cin >> n >> n_tests;

    long double average_queries = 0;
    for(int i = 0; i < n_tests; ++i)
        average_queries += runOneTest();
    average_queries /= n_tests;

    std::cout << std::fixed << std::setprecision(6) << average_queries << std::endl;
}