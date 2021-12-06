#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <string>

static const std::string WRONG_NUMBER_OF_PLAYLISTS = "WNP";
static const std::string WRONG_NUMBER_OF_SONGS = "WNS";
static const std::string INVALID_SONG = "IS";
static const std::string INVALID_GUESS = "IG";
static const std::string PLAYED_ALL_SONGS = "PAS";
static const std::string LOW_ACCURACY = "LA";
static const std::string OK = "OK";

const long long BASE = 792753497ll;
const long long MOD = 1185914819ll;
long long hashWithInput(std::string res)
{
    long long hsh = 0;
    for (char elem : res)
    {
        hsh = (hsh * BASE + elem) % MOD;
    }
    return hsh;
}

int main(int argc, char *argv[])
{
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    if (!sol.is_open())
    {
        std::cerr << "Sol-file " << argv[2] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out.is_open())
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    std::string targetRes;
    sol >> targetRes;

    std::string resType, res;
    long long hsh;
    out >> resType >> res >> hsh;

    if (hsh != hashWithInput(resType + " " + res))
    {
        std::cerr<<"Your program printed to stdout. It shouldn't.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (resType == WRONG_NUMBER_OF_PLAYLISTS)
    {
        std::cerr << "Wrong number of playlists.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (resType == WRONG_NUMBER_OF_SONGS)
    {
        std::cerr << "Playlist contains wrong number of songs.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (resType == INVALID_SONG)
    {
        std::cerr << "Playlist contains an invalid song.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (resType == INVALID_GUESS)
    {
        std::cerr << "Your function returned an invalid guess.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (resType == PLAYED_ALL_SONGS)
    {
        std::cerr << "Your function tried to play more songs than the playlist contains.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    if (resType == LOW_ACCURACY)
    {
        std::cerr << "Guess accuracy lower than requested threshold.\n";
        std::cout << 0 << "\n";
        return 0;
    }

    double score = std::stod(targetRes) / std::stod(res);
    score = std::min(1.0, score);

    std::cout << std::setprecision(4) << score << "\n";

    return 0;
}
