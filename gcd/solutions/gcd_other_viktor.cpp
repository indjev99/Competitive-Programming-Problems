#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef double ld;
typedef unsigned long long ull;
template<class T> void check_min(T &a, const T &b){ a = (a < b) ? a : b; }
template<class T> void check_max(T &a, const T &b){ a = (a > b) ? a : b; }
#define all(x) (x).begin(), (x).end()

vector<int> primes{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43};

ull query(ull a, ull b);

ull lcm(ull a, ull b){
    return a / __gcd(a, b) * b;
}

ull play(ull MAX_X){
    ull curr = 1, add = 0, prev_goal = 1;
    while(true){
        ull goal = 1;
        vector<pair<ld, int>> v;
        for(auto prime: primes){
            ull cnt = 0, t = curr;
            while(t % prime == 0){
                t /= prime;
                ++cnt;
            }
            
            ld e = 0, pwr = prime;
            for(ull i = 1; pwr * curr < 2 * MAX_X; ++i, pwr *= prime){
                e = log2(prime) * (ld)1.0 / pwr;

                ld cost = 1 / (log2(prime) * (cnt + i));
                if(i != 1)
                    cost = 1 / (log2(prime));
                
                if(i != 1 || prime != 2) v.push_back({e * cost, prime});
            }
        }
        sort(all(v), greater<pair<ld, int>>());

        {
            ull copy_curr = curr;
            while(copy_curr % 2 == 0){
                copy_curr >>= 1;
                goal <<= 1;
            }
            goal <<= 1;
        }

        for(auto [score, prime]: v){
            // cout << score << "," << prime << " ";
            if(prime * goal < 2 * MAX_X){
                ull prev_goal = goal;
                if(goal % prime){
                    ull curr_copy = curr;
                    while(curr_copy % prime == 0){
                        if(prime * goal >= 2 * MAX_X){
                            goal = 2 * MAX_X;
                            break;
                        }
                        curr_copy /= prime;
                        goal *= prime;
                    }
                }

                if(prime * goal < 2 * MAX_X)
                    goal *= prime;
                else
                    goal = 2 * MAX_X;

                if(goal >= 2 * MAX_X)
                    goal = prev_goal;
            }
        }

        while(goal < MAX_X) goal <<= 1;

        ull g = query(add, goal);
        curr = lcm(g, curr);
        prev_goal = goal;

        if(curr >= MAX_X){
            if(!add) return 0;
            return (curr - (add % curr));
        }
    
        add += curr;
        add %= 2 * curr;
    }

    assert(-1);
    return 1;
}