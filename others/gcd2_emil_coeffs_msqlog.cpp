#include <iostream>
#include <vector>

const int MAX_N = 2e5;
const int MAX_Q = 2e5;
const int MAX_VAL = 3e3;
const long long MOD = 1e9 + 7;

int n, q;

int smallestPrime[MAX_VAL + 1];

long long coeffs[MAX_VAL + 1];
long long partialCoeffs[MAX_VAL + 1][MAX_VAL + 1];

long long cnt[MAX_VAL + 1];

long long numSubsets[MAX_N + MAX_Q];

long long ans;

void precompPows2()
{
    numSubsets[0] = 1;

    for (int i = 1; i <= MAX_N + MAX_Q; ++i)
    {
        numSubsets[i] = numSubsets[i - 1] * 2 % MOD;
    }

    for (int i = 0; i <= MAX_N + MAX_Q; ++i)
    {
        numSubsets[i] = (numSubsets[i] + MOD - 1) % MOD;
    }
}

void sieve()
{
    for (int i = 2; i <= MAX_VAL; ++i)
    {
        if (smallestPrime[i] > 0) continue;
        for (int j = i; j <= MAX_VAL; j += i)
        {
            if (smallestPrime[j] == 0) smallestPrime[j] = i;
        }
    }
}

void findCoeffs()
{
    for (int i = MAX_VAL; i >= 1; --i)
    {
        partialCoeffs[i][i] = 1;
        for (int j = i * 2; j <= MAX_VAL; j += i)
        {
            for (int k = 1; k <= MAX_VAL; ++k)
            {
                partialCoeffs[i][k] -= partialCoeffs[j][k];
            }
        }
    }

    for (int i = 1; i <= MAX_VAL; ++i)
    {
        for (int j = 1; j <= MAX_VAL; ++j)
        {
            coeffs[j] += i * partialCoeffs[i][j];
        }
    }
}

void addNumber(int num, int sign)
{
    std::vector<int> divisors = {1};
    int oldDivisors = 0;
    int prevPrime = 0;
    int currPrimePow = 0;

    while (num > 1)
    {
        int prime = smallestPrime[num];
        if (prime != prevPrime)
        {
            prevPrime = prime;
            currPrimePow = prime;
            oldDivisors = divisors.size();
        }
        else currPrimePow *= prime;

        num /= prime;

        for (int i = 0; i < oldDivisors; ++i)
        {
            divisors.push_back(divisors[i] * currPrimePow);
        }
    }

    for (int div : divisors)
    {
        ans -= coeffs[div] * numSubsets[cnt[div]];
        cnt[div] += sign;
        ans += coeffs[div] * numSubsets[cnt[div]];
        ans %= MOD;
    }
    
    ans = (ans + MOD) % MOD;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int type, num;

    std::cin >> n >> q;

    precompPows2();
    sieve();
    findCoeffs();

    for (int i = 0; i < n; ++i)
    {
        std::cin >> num;
        addNumber(num, 1);
    }

    std::cout << ans << "\n";

    for (int i = 0; i < q; ++i)
    {
        std::cin >> type >> num;
        addNumber(num, type == 1 ? 1 : -1);

        std::cout << ans << "\n";
    }

    return 0;
}
