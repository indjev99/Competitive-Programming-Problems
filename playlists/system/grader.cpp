#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include "playlists.h"

static int n, k, s;
static double p;
static int seed;
static std::mt19937 mt;

static const std::string WRONG_NUMBER_OF_PLAYLISTS = "WNP";
static const std::string WRONG_NUMBER_OF_SONGS = "WNS";
static const std::string INVALID_SONG = "IS";
static const std::string INVALID_GUESS = "IG";
static const std::string PLAYED_ALL_SONGS = "PAS";
static const std::string LOW_ACCURACY = "LA";
static const std::string OK = "OK";

static const int GUESS_ROUNDS = 20000;

static const long long BASE = 792753497ll;
static const long long MOD = 1185914819ll;
static long long hashWithInput(std::string res)
{
    long long hsh = 0;
    for (char elem : res)
    {
        hsh = (hsh * BASE + elem) % MOD;
    }
    return hsh;
}

static void setResult(std::string resType, std::string res)
{
    std::cout << resType << " " << res << " " << hashWithInput(resType + " " + res) << std::endl;
    exit(0);
}

static void input()
{
    std::cin >> n >> k >> s >> p >> seed;
    mt = std::mt19937(seed);
}

static std::vector<std::vector<int>> playlists;

static void verifyPlaylists()
{
    if (playlists.size() != n)
        setResult(WRONG_NUMBER_OF_PLAYLISTS, std::to_string(playlists.size()));

    for (std::vector<int> playlist : playlists)
    {
        if (playlist.size() != k)
            setResult(WRONG_NUMBER_OF_SONGS, std::to_string(playlist.size()));

        for (int song : playlist)
        {
            if (song < 0 || song >= s)
                setResult(INVALID_SONG, std::to_string(song));
        }
    }
}

static int currentPlaylist;
static int currentSong;
static int songsPlayed = 0;
static int correctSongsPlayed = 0;

static void startNewPlaylist()
{
    currentPlaylist = mt() % n;
    currentSong = 0;
    shuffle(playlists[currentPlaylist].begin(), playlists[currentPlaylist].end(), mt);
    songsPlayed = 0;
}

int nextSong()
{
    songsPlayed++;

    if (currentSong >= k)
        setResult(PLAYED_ALL_SONGS, "x");

    int song = playlists[currentPlaylist][currentSong];
    currentSong++;

    return song;
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    playlists = makePlaylists(n, k, s, p);
    verifyPlaylists();

    int correctGuesses = 0;
    for (int i = 0; i < GUESS_ROUNDS; i++)
    {
        startNewPlaylist();
        int guess = guessPlaylist();

        if (guess < 0 || guess >= n)
            setResult(INVALID_GUESS, std::to_string(guess));

        if (guess == currentPlaylist)
        {
            correctSongsPlayed += songsPlayed;
            correctGuesses++;
        }
    }

    double correctRate = (double)correctGuesses / (double)GUESS_ROUNDS;

    if (correctRate < p)
        setResult(LOW_ACCURACY, std::to_string(correctRate));

    double songsPlayedRate = (double)correctSongsPlayed / (double)correctGuesses;

    setResult(OK, std::to_string(songsPlayedRate));

    return 0;
}
