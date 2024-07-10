#include <iostream>
#include <vector>
#include <cassert>

const int MAX_N = 4e6;

int n;
int vals[MAX_N];

long long ans;

void input()
{
    std::string s;

    std::cin >> n;
    std::cin >> s;

    for (int i = 0; i < n; ++i)
    {
        vals[i] = s[i] == '(' ? 1 : -1;
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int archiveCounts[MAX_N];

struct Chunk
{
    int prefSum;
    int size = 1;
    bool archived = false;
    std::vector<int> counts = {1};

    void archive()
    {
        assert(!archived);

        archived = true;
        for (int v = 0; v < (int) counts.size(); ++v)
        {
            archiveCounts[counts.size() - v - 1] += counts[v];
        }
    }

    void unarchive()
    {
        assert(archived);

        archived = false;
        for (int v = 0; v < (int) counts.size(); ++v)
        {
            archiveCounts[counts.size() - v - 1] -= counts[v];
        }
    }
};

int prefSums[MAX_N + 1];
int distModify[MAX_N + 1];

std::vector<Chunk> chunks;
std::vector<int> unarchived;

void solve()
{
    ans = 0;

    prefSums[0] = 0;
    for (int i = 0; i <= n; ++i)
    {
        prefSums[i] = vals[i - 1] + prefSums[i - 1];
    }

    std::vector<int> stack;
    for (int i = n; i >= 0; --i)
    {
        while (!stack.empty() && prefSums[i] >= prefSums[stack.back()])
        {
            distModify[stack.back()] = stack.back() - i;
            stack.pop_back();
        }
        stack.push_back(i);
    }

    while (!stack.empty())
    {
        distModify[stack.back()] = n;
        stack.pop_back();
    }

    long long rCountNgtLps = 0;

    chunks.push_back({prefSums[n]});
    unarchived.push_back(0);

    for (int i = n - 1; i >= 0; --i)
    {
        if (vals[i] == 1)
        {
            chunks.push_back({prefSums[i]});

            rCountNgtLps += 1;

            unarchived.push_back(chunks.size() - 1);
        }
        else
        {
            rCountNgtLps += 1;
            int v = prefSums[i + 1];
            int numCounts = chunks.back().counts.size();
            if (v < numCounts)
            {
                rCountNgtLps -= chunks.back().counts[numCounts - v - 1];
            }

            if (chunks.back().archived)
            {
                chunks.back().unarchive();
                unarchived.push_back(chunks.size() - 1);
            }

            chunks.back().prefSum += 1;
            chunks.back().size += 1;
            chunks.back().counts.push_back(1);

            if (chunks.size() >= 2 && chunks.back().prefSum == chunks[chunks.size() - 2].prefSum)
            {
                Chunk& last = chunks.back();
                Chunk& prev = chunks[chunks.size() - 2];

                assert(unarchived.back() == (int) chunks.size() - 1);
                unarchived.pop_back();

                if (prev.archived)
                {
                    prev.unarchive();
                    unarchived.push_back(chunks.size() - 2);
                }

                if (last.counts.size() > prev.counts.size())
                {
                    std::swap(last.counts, prev.counts);
                }

                prev.size = last.size + prev.size;
                for (int v = 0; v < (int) last.counts.size(); ++v)
                {
                    prev.counts[prev.counts.size() - v - 1] += last.counts[last.counts.size() - v - 1];
                }

                chunks.pop_back();
            }
        }

        if (chunks.back().size < distModify[i])
        {
            chunks.back().archive();
            assert(unarchived.back() == (int) chunks.size() - 1);
            unarchived.pop_back();
        }

        if (vals[i] == 1)
        {
            int v = prefSums[i + 1];
            rCountNgtLps -= archiveCounts[v];
            for (int idx : unarchived)
            {
                int numCounts = chunks[idx].counts.size();
                if (v < numCounts)
                {
                    rCountNgtLps -= chunks[idx].counts[numCounts - v - 1];
                }
            }
        }
        else
        {
            int v = prefSums[i];
            rCountNgtLps += archiveCounts[v];
            for (int idx : unarchived)
            {
                int numCounts = chunks[idx].counts.size();
                if (v < numCounts)
                {
                    rCountNgtLps += chunks[idx].counts[numCounts - v - 1];
                }
            }
        }

        ans += rCountNgtLps;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
