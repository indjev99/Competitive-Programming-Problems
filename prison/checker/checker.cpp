#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <map>

const std::string PASSWORD = "djk398fjk29skg05sjdfud";

const std::map<int, int> TARGETS = {
    {700, 82017},
    {1100, 202217},
    {1500, 375751},
    {1900, 602617},
    {2300, 882817},
    {2700, 1216351},
    {3100, 1603217},
    {3500, 2043417},
    {3900, 2536951},
    {4300, 3083817}
};

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cerr << "In-file " << argv[1] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out)
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int m;
    in >> m;

    std::string password;
    out >> password;

    if (password != PASSWORD)
    {
        std::cerr << "IO issues." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int n;
    out >> n;

    if (n < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    int target;
    if (!TARGETS.count(m))
    {
        target = m + m * (m - 1) / 6;
    }
    else
    {
        target = TARGETS.at(m);
    }

    double logBase = 0.985 * log(m);
    double logScore = std::max((log(n) - logBase) / (log(target) - logBase), 0.0);
    double linScore = (double) n / target;
    double score = 0.35 * pow(logScore, 0.3) + 0.65 * pow(linScore, 2.4);

    score = std::min(std::max(score, 0.01), 1.0);

    std::cerr << "N: " << n << std::endl;

    std::cout << std::setprecision(6) << std::fixed << score << std::endl;

    return 0;
}
