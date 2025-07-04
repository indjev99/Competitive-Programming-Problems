#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <utility>

using namespace std;

double linear_interp(double x, double x1, double x2, double y1, double y2){
    return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
}

int main(int argc, char *argv[])
{
    std::ifstream out(argv[3]);

    if (!out)
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }
    
    std::ifstream in(argv[1]);
    int t, random_seed;
    in >> t >> random_seed;
    int n;
    in >> n;

    int bits;
    out >> bits;

    if (bits < 0)
    {
        std::string msg;
        std::getline(out, msg);
        if (msg == "") std::getline(out, msg);
        std::cerr << msg << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    double score = 0;
    double MAX_SCORE = 205;
    // double r = std::min(bits / MAX_SCORE, 1.0);

    vector<pair<int, double>> fixed_points{
        {1, 5},
        {5, 10},
        {10, 20},
        {85, 45},
        {100, 55},
        {170, 75},
        {MAX_SCORE, 100}
    };

    if (bits == 0) {
        score = 0;
    }
    else if (bits >= MAX_SCORE) {
        score = 1;
    }
    else {
        for (int i = 0; i + 1 < fixed_points.size(); ++i){
            if (fixed_points[i].first <= bits && bits < fixed_points[i + 1].first) {
                score = linear_interp(bits, fixed_points[i].first, fixed_points[i + 1].first, fixed_points[i].second, fixed_points[i + 1].second);
                score /= 100;
                break;
            }
        }
    }

    std::cerr << "Minimum number of decoded bits: " << bits << std::endl;
    std::cout << score << std::endl;

    return 0;
}
