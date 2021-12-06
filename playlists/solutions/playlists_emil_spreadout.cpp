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

const int TRIALS = 1000;

std::vector<Playlist> playlists;

std::vector<std::vector<int>> makePlaylists(int N, int K, int S, double P)
{
    n = N;
    k = K;
    s = S;
    p = P;

    generator.seed(0);

    playlists.resize(n);

    for (int i = 0; i < n; ++i)
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

int guessPlaylist()
{
    int t = 0;
    std::vector<int> counts(s, 0);

    sampleProbs.resize(n);
    std::fill(sampleProbs.begin(), sampleProbs.end(), 1);

    while (guessProb(t, counts) < p && t < k)
    {
        int next = nextSong();

        for (int i = 0; i < n; ++i)
        {
            sampleProbs[i] *= playlists[i].sampleProbUpdate(t, counts, next);
        }

        ++counts[next];
        ++t;
    }

    return guess;
}
