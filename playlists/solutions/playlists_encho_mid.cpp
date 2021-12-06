#include <iostream>
#include <stdio.h>
#include <random>
#include <algorithm>
#include <set>
#include "playlists.h"
using namespace std;

int n, k, s;
double p;
mt19937 mt(1337);
vector< vector<int> > playlists;
int playlistCounts[111][111];
int orgPlaylistCounts[111][111];

int curCount[111];
set<int> seens[111];

int estimateSongs(double rat)
{
    int songs = 1;
    int cur = round(k / rat);
    int total = cur;

    while(total < k)
    {
        cur = max(1, (int)round((double)cur / rat));
        total += cur;
        songs++;
    }

    return songs;
}

int PUT_OFF = 0;
vector< vector<int> > makePlaylists(int N, int K, int S, double P)
{
    int i, j;

    n = N;
    k = K;
    s = S;
    p = P + 0.01;

    if (s > 15)
        PUT_OFF = s - 15;

    s -= PUT_OFF;
    n -= PUT_OFF;

    double rat = 2.0;

    while(estimateSongs(rat) > s)
        rat -= 0.05;

    int songCnt = estimateSongs(rat);
    int restarts = 0;
    set< vector<int> > shapeSet;

    for (i=0;i<n;i++)
    {
        vector<int> playlist;
        vector<int> playlistShape;

        int goal = round(k / rat);
        set<int> seen;

        while(playlist.size() < k)
        {
            int choice = mt() % s;
            if (seen.find(choice) != seen.end())
                continue;
            seen.insert(choice);

            for (j=0;j<goal;j++)
            {
                playlist.push_back(choice);
            }
            playlistShape.push_back(choice);

            goal = max((int)round(goal / rat), 1);
        }
        playlist.resize(k);

        if (shapeSet.find(playlistShape) != shapeSet.end() && restarts < 1000)
        {
            restarts++;
            i--;
            continue;
        }

        playlists.push_back(playlist);
    }

    for (i=n;i<n+PUT_OFF;i++)
    {
        vector<int> playlist;
        for (j=0;j<k;j++)
        {
            playlist.push_back(s + i - n);
        }
        playlists.push_back(playlist);
    }
    n += PUT_OFF;

    if (restarts >= 1000)
    {
        fprintf(stderr, "BAD ENDING\n");
    }

    for (i=0;i<n;i++)
    {
        for (j=0;j<k;j++)
        {
            playlistCounts[i][playlists[i][j]]++;
            orgPlaylistCounts[i][playlists[i][j]]++;
        }
    }

    return playlists;
}

double fixedProb[111];
double invProbs[111];

void newRun()
{
    int i, j;

    for (i=0;i<n;i++)
    {
        fixedProb[i] = 1.0;
    }

    for (i=0;i<n;i++)
    {
        for (j=0;j<s;j++)
        {
            playlistCounts[i][j] = orgPlaylistCounts[i][j];
        }
    }
}

void recomputeBayesians(int song, int songId)
{
    int i, j;
    double totalSongProb = 0.0;

    for (i=0;i<n;i++)
    {
        fixedProb[i] *= (double)playlistCounts[i][song] / (k - songId);
        playlistCounts[i][song]--;
        totalSongProb += fixedProb[i];
    }

    totalSongProb /= (double)n;

    for (i=0;i<n;i++)
    {
        invProbs[i] = fixedProb[i] / (double)n / totalSongProb;
    }
}

double actualAverage = 0.0;
double leftovers = 0.0;

int guessPlaylist()
{
    vector<int> curPlaylist;
    int i, j;

    newRun();

    for (i=0;i<k;i++)
    {
        curPlaylist.push_back(nextSong());
        if (curPlaylist.back() >= s)
        {
            return (n - PUT_OFF) + (curPlaylist.back() - s);
        }

        recomputeBayesians(curPlaylist.back(), i);

        for (j=0;j<n;j++)
        {
            if (invProbs[j] >= p - leftovers)
            {
                actualAverage += invProbs[j];
                leftovers += invProbs[j] - p;

                return j;
            }
        }
    }

    return -1;
}

