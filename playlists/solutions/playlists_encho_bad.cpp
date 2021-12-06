#include <iostream>
#include <stdio.h>
#include <random>
#include <algorithm>
#include "playlists.h"
using namespace std;

int n, k, s;
double p;
mt19937 mt(1337);
vector< vector<int> > playlists;

vector< vector<int> > makePlaylists(int N, int K, int S, double P)
{
    int i, j;

    n = N;
    k = K;
    s = S;
    p = P;

    for (i=1;i<=n;i++)
    {
        vector<int> playlist;
        for (j=1;j<=k;j++)
        {
            playlist.push_back(mt() % s);
        }
        sort(playlist.begin(), playlist.end());
        playlists.push_back(playlist);
    }

    return playlists;
}

int guessPlaylist()
{
    vector<int> curPlaylist;
    int i, j;

    for (i=1;i<=k;i++)
    {
        curPlaylist.push_back(nextSong());
    }

    sort(curPlaylist.begin(), curPlaylist.end());

    for (i=0;i<n;i++)
    {
        bool matches = true;

        for (j=0;j<k;j++)
        {
            if (curPlaylist[j] != playlists[i][j])
            {
                matches = false;
                break;
            }
        }

        if (matches)
            return i;
    }

    return -1;
}
