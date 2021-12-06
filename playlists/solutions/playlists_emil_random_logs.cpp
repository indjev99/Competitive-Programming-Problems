#include "playlists.h"

#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

std::mt19937 generator;

int n, k, s;
double logP;

std::vector<double> logs;

void precomp()
{
    logs.resize(std::max(n, std::max(s, k)) + 1);
    for (int i = 0; i < logs.size(); ++i)
    {
        logs[i] = log(i);
    }
}

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

    double sampleLogProbUpdate(int t, const std::vector<int>& others, int next)
    {
        return logs[t + 1] - logs[others[next] + 1] + logs[counts[next] - others[next]] - logs[k - t];
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
    logP = log(P);

    generator.seed(0);

    precomp();

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
std::vector<double> sampleLogProbs;

double guessLogProb(int t, const std::vector<int>& counts)
{
    double maxSampleLogProb = logs[0];
    for (int i = 0; i < n; ++i)
    {
        if (sampleLogProbs[i] > maxSampleLogProb)
        {
            maxSampleLogProb = sampleLogProbs[i];
            guess = i;
        }
    }

    double totalRelProb = 0;
    for (int i = 0; i < n; ++i)
    {
        if (i == guess) continue;
        totalRelProb += exp(sampleLogProbs[i] - maxSampleLogProb);
    }

    //double sampleLogProb = maxSampleLogProb + log1p(totalRelProb) - logs[n];
    //double guessLogProb = maxSampleLogProb - logs[n] - sampleLogProb;

    return -log1p(totalRelProb);
}

int guessPlaylist()
{
    int t = 0;
    std::vector<int> counts(s, 0);

    sampleLogProbs.resize(n);
    std::fill(sampleLogProbs.begin(), sampleLogProbs.end(), logs[1]);

    while (guessLogProb(t, counts) < logP && t < k)
    {
        int next = nextSong();

        for (int i = 0; i < n; ++i)
        {
            sampleLogProbs[i] += playlists[i].sampleLogProbUpdate(t, counts, next);
        }

        ++counts[next];
        ++t;
    }

    return guess;
}
