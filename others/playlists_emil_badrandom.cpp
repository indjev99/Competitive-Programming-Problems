#include "playlists.h"

#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

std::mt19937 generator;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}

int n, k, s;
double p;

struct Playlist
{
    std::vector<int> counts;

    void randomize()
    {
        counts.resize(s);
        int currSong = 0;
        std::fill(counts.begin(), counts.end(), 0);
        for (int i = 0; i < k; ++i)
        {
            ++counts[randNum(0, s - 1)];
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

bool operator==(const Playlist& left, const Playlist& right)
{
    return left.counts == right.counts;
}

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
        do
        {
            playlists[i].randomize();
        }
        while (std::find(playlists.begin(), playlists.begin() + i, playlists[i]) != playlists.begin() + i);
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
    double maxSampleProb = 0;
    for (int i = 0; i < n; ++i)
    {
        if (sampleProbs[i] > maxSampleProb)
        {
            maxSampleProb = sampleProbs[i];
            guess = i;
        }
    }

    double totalProb = 0;
    for (int i = 0; i < n; ++i)
    {
        totalProb += sampleProbs[i];
    }

    //double sampleLogProb = totalProb / n;
    //double guessLogProb = maxSampleProb / n / sampleLogProb;

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
