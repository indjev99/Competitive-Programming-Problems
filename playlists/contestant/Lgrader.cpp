#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include "playlists.h"

static int n, k, s;
static double p;
static std::mt19937 mt(42);

static const int GUESS_ROUNDS = 20000;

static void setResult(std::string message)
{
    std::cout << message << std::endl;
    exit(0);
}

static void input()
{
    std::cin >> n >> k >> s >> p;
}

static std::vector<std::vector<int>> playlists;

static void verifyPlaylists()
{
    if (playlists.size() != n)
        setResult("Wrong number of playlists: " + std::to_string(playlists.size()));

    for (std::vector<int> playlist : playlists)
    {
        if (playlist.size() != k)
            setResult("Wrong number of songs: " + std::to_string(playlist.size()));

        for (int song : playlist)
        {
            if (song < 0 || song >= s)
                setResult("Invalid song: " + std::to_string(song));
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
        setResult("Trying to play songs after playlist is over");

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
            setResult("Invalid guess: " + std::to_string(guess));

        if (guess == currentPlaylist)
        {
            correctSongsPlayed += songsPlayed;
            correctGuesses++;
        }
    }

    double correctRate = (double)correctGuesses / (double)GUESS_ROUNDS;

    if (correctRate < p)
        setResult("Accuracy too low: " + std::to_string(correctRate));

    double songsPlayedRate = (double)correctSongsPlayed / (double)correctGuesses;

    setResult("OK " + std::to_string(songsPlayedRate));

    return 0;
}
