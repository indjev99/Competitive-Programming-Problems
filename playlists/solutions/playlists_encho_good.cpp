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

int estimatePossibilities(int x)
{
    int i;
    int cnt = 1;

    for (i=s;i>=s-x+1;i--)
    {
        cnt *= i;
    }

    for (i=1;i<=x;i++)
    {
        cnt /= i;
    }

    return cnt;

}

int PUT_OFF = 0;
bool GG_EZ = false;

vector< vector<int> > all;
vector<int> cur;

void generateAll(int total, int curVal)
{
    if (cur.size() > total)
        return;

    if (curVal == s)
    {
        if (cur.size() == total)
            all.push_back(cur);

        return;
    }

    generateAll(total, curVal + 1);

    cur.push_back(curVal);
    generateAll(total, curVal + 1);
    cur.pop_back();
}

vector< vector<int> > makePlaylists(int N, int K, int S, double P)
{
    int i, j;

    n = N;
    k = K;
    s = S;
    p = P + 0.01;

    //if (s > 10 && k > 3)
    //    PUT_OFF = s - 10;

    if ( (double)(s - 1) / (double)n >= p )
    {
        GG_EZ = true;
        for (i=0;i<n;i++)
        {
            vector<int> playlist;
            for (j=0;j<k;j++)
            {
                if (i < s - 1)
                    playlist.push_back(i);
                else
                    playlist.push_back(s - 1);
            }
            playlists.push_back(playlist);
        }

        return playlists;
    }

    s -= PUT_OFF;
    n -= PUT_OFF;

    int perPlay = 1;
    while(estimatePossibilities(perPlay) < n && perPlay < k && perPlay < s)
        perPlay++;

    generateAll(perPlay, 0);

    shuffle(all.begin(), all.end(), mt);

    for (i=0;i<n;i++)
    {
        int ch = i % (int)all.size();
        vector<int> playlist;
        int cntLeft = perPlay;
        int ind = 0;

        while(playlist.size() < k)
        {
            int chunkSize = (int)round((double)(k - playlist.size()) / (double)cntLeft);

            for (j=0;j<chunkSize;j++)
            {
                playlist.push_back(all[ch][ind]);
            }

            ind++;
            cntLeft--;
        }

        playlists.push_back(playlist);
    }
    ///

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

    ///

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
int rounds = 0;
double score = 0;

int guessPlaylist()
{
    vector<int> curPlaylist;
    int i, j;

    if (GG_EZ)
    {
        return nextSong();
    }

    rounds++;
    newRun();

    for (i=0;i<k;i++)
    {
        curPlaylist.push_back(nextSong());
        if (curPlaylist.back() >= s)
        {
            return (n - PUT_OFF) + (curPlaylist.back() - s);
        }

        recomputeBayesians(curPlaylist.back(), i);

        if (rounds > 1000 && leftovers > 1.5)
        {
            double avgScore = (double)score / rounds;

            if ( (i + 1) > avgScore )
            {
                score += avgScore;
                leftovers -= p;
                return 0;
            }
        }

        for (j=0;j<n;j++)
        {
            if (invProbs[j] >= p)
            {
                actualAverage += invProbs[j];
                leftovers += invProbs[j] - p;

                score += (i + 1);

                return j;
            }
        }
    }

    return -1;
}

