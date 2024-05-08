#include <algorithm>
#include <iostream>
#include <vector>

inline std::ostream& operator<<(std::ostream& out, __int128 num)
{
    bool flip = false;

    if (num < 0)
    {
        flip = true;
        num = -num;
    }

    std::string str = "";

    do
    {
        str += '0' + num % 10;
        num /= 10;
    }
    while (num > 0);

    std::reverse(str.begin(), str.end());

    if (flip) str = "-" + str;

    out << str;

    return out;
}

__int128 solveGpus(std::vector<int>& gpuCosts, std::vector<int>& reqTimes);
