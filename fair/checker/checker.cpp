#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

const bool regen = false;

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream sol(argv[2]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cout << "0\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }

    if (!sol)
    {
        std::cout << "0\n";
        std::cerr << "Sol-file " << argv[2] << " not found.\n";
        return 0;
    }

    if (!out)
    {
        std::cout << "0\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    int n, m;
    double p;
    double penFair, penUnfair;
    int k;

    in >> n >> m >> p >> penFair >> penUnfair >> k;

    double totalPen = 0;
    std::vector<bool> sols;

    for (int i = 0; i < k; ++i)
    {
        bool currOut, currSol;
        out >> currOut;
        sol >> currSol;

        sols.push_back(currSol);

        if (currOut != currSol) totalPen += currSol ? penFair : penUnfair;
    }

    double avgPen = static_cast<double>(totalPen) / k;
    double baseAvgPen = std::min(p * penFair, (1 - p) * penUnfair);
    double outScore = (baseAvgPen - avgPen) / baseAvgPen;
    outScore = std::max(outScore, 0.0);

    double solScore;
    sol >> solScore;

    double ratio = outScore / solScore;

    ratio = std::min(ratio, 1.0);
    ratio = std::max(ratio, 0.0);

    const double SWITCH1 = 0.8;

    double res = 0;
    if (ratio > SWITCH1) res = 0.3 + 0.7 * (1 - pow(1 - (ratio - SWITCH1) / (1 - SWITCH1), 0.75));
    else res = 0.3 * ratio / SWITCH1;

    res = std::min(res, 1.0);
    res = std::max(res, 0.0);

    std::cerr << "AP:  " << avgPen << std::endl;
    std::cerr << "BAP: " << baseAvgPen << std::endl;
    std::cerr << "S:   " << outScore << std::endl;
    std::cerr << "R:   " << ratio << std::endl;

    std::cout << res << std::endl;

    if (regen)
    {
        sol.close();
    
        std::ofstream sol2(argv[2]);

        for (bool currSol : sols)
        {
            sol2 << currSol << "\n";
        }

        sol2 << outScore << "\n";
    }

    return 0;
}
