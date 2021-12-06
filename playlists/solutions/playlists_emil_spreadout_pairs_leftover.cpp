#include "playlists.h"

#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

std::mt19937 generator;

int n, k, s;
double p;

struct Playlist
{
    std::vector<int> counts;

    void randomize()
    {
        std::vector<bool> tokens(k + s - 1, false);
        std::fill(tokens.begin(), tokens.begin() + s - 1, true);
        std::shuffle(tokens.begin(), tokens.end(), generator);

        counts.resize(s);
        int currSong = 0;
        std::fill(counts.begin(), counts.end(), 0);
        for (bool token : tokens)
        {
            if (token) ++currSong;
            else ++counts[currSong];
        }
    }

    std::vector<int> songs()
    {
        std::vector<int> list;
        for (int i = 0; i < s; ++i)
        {
            for (int j = 0; j < counts[i]; ++j)
            {
                list.push_back(i);
            }
        }
        return list;
    }

    double sampleProbUpdate(int t, const std::vector<int>& others, int next)
    {
        return (double) (t + 1) / (others[next] + 1) * (counts[next] - others[next]) / (k - t);
    }
};

int maxDist()
{
    return k * k * 2;
}

int dist(const Playlist& left, const Playlist& right)
{
    int d = 0;
    for (int i = 0; i < s; ++i)
    {
        int currD = std::abs(left.counts[i] - right.counts[i]);
        d += currD * currD;
    }
    return d;
}

const int TESTS = 2e4;
const double STDEVS = 3;
const int TRIALS = 3000;

std::vector<Playlist> playlists;

double threshold;

std::vector<std::vector<int>> makePlaylists(int N, int K, int S, double P)
{
    n = N;
    k = K;
    s = S;
    p = P;

    double left = 0;
    double right = 1;
    for (int i = 0; i < 64; ++i)
    {
        double mid = (left + right) / 2;
        if (mid - STDEVS * sqrt(mid * (1 - mid) / TESTS) >= p) right = mid;
        else left = mid;
    }
    threshold = left;

    generator.seed(0);

    playlists.resize(n);

    int curr = 0;

    for (int i = 0; i < s && curr < n; ++i)
    {
        playlists[curr].counts.resize(s, 0);
        playlists[curr].counts[i] = k;
        ++curr;
    }

    std::vector<std::pair<int, int>> pairs;
    for (int i = 0; i < s; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            pairs.push_back({i, j});
        }
    }

    std::shuffle(pairs.begin(), pairs.end(), generator);
    for (int i = 0; i < pairs.size() && curr < n; ++i)
    {
        playlists[curr].counts.resize(s, 0);
        playlists[curr].counts[pairs[i].first] = k / 2;
        playlists[curr].counts[pairs[i].second] = (k + 1) / 2;
        ++curr;
    }

    for (int i = curr; i < n; ++i)
    {
        Playlist best;
        int bestMinDist = -1;
        for (int j = 0; j < TRIALS; ++j)
        {
            int minDist = maxDist();
            playlists[i].randomize();
            for (int j = 0; j < i; ++j)
            {
                minDist = std::min(minDist, dist(playlists[i], playlists[j]));
            }
            if (minDist > bestMinDist)
            {
                bestMinDist = minDist;
                best = playlists[i];
            }
        }
        playlists[i] = best;
    }

    std::vector<std::vector<int>> playlistsSongs;
    for (int i = 0; i < n; ++i)
    {
        playlistsSongs.push_back(playlists[i].songs());
    }
    return playlistsSongs;
}

int guess;
std::vector<double> sampleProbs;

double guessProb(int t, const std::vector<int>& counts)
{
    double totalProb = 0;
    double maxSampleProb = 0;
    for (int i = 0; i < n; ++i)
    {
        totalProb += sampleProbs[i];
        if (sampleProbs[i] > maxSampleProb)
        {
            maxSampleProb = sampleProbs[i];
            guess = i;
        }
    }
    return maxSampleProb / totalProb;
}

double leftover;

int guessPlaylist()
{
    int t = 0;
    std::vector<int> counts(s, 0);

    sampleProbs.resize(n);
    std::fill(sampleProbs.begin(), sampleProbs.end(), 1);

    while (guessProb(t, counts) < threshold - leftover && t < k)
    {
        int next = nextSong();

        for (int i = 0; i < n; ++i)
        {
            sampleProbs[i] *= playlists[i].sampleProbUpdate(t, counts, next);
        }

        ++counts[next];
        ++t;
    }

    leftover += guessProb(t, counts) - threshold;

    return guess;
}
