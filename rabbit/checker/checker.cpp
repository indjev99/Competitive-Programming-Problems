#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

static int n, S, L;
static std::vector<int> aliveRabbits, survivors;

int makeMove(int pos, int shot, int state)
{
    if ( (pos < shot && state < S) || (pos > shot && state >= S) )
        pos = std::max(pos - 1, 1);
    else
        pos = std::min(pos + 1, n);

    return pos;
}

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cout << "0\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }
    if (!out)
    {
        std::cout << "0\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    in >> n >> S >> L;

    int shL;
    int state = 0;

    out >> shL;

    if (shL > 2 * n)
    {
        std::cout << "0\n";
        std::cerr << "Wrong Answer. Check sequence length exceeds N\n";
        return 0;
    }

    for (int i = 1; i <= n; i++)
    {
        aliveRabbits.push_back(i);
    }

    for (int i = 1; i <= shL; i++)
    {
        int shot;

        out >> shot;

        if (shot < 1 || shot > n)
        {
            std::cout << "0\n";
            std::cerr << "Wrong Answer\n";
            return 0;
        }

        survivors.clear();
        for (int j = 0; j < aliveRabbits.size(); j++)
        {
            if (aliveRabbits[j] != shot)
                survivors.push_back(makeMove(aliveRabbits[j], shot, state));
        }

        aliveRabbits = survivors;
        state = (state + 1) % (S + L);
    }

    if (!aliveRabbits.empty())
    {
        std::cout << "0\n";
        std::cerr << "Wrong Answer.\n";
        return 0;
    }

    double threshold = (double)n * (double)(S + L) / (double)std::max(3*S, 2*L + S) + 3.0*std::max(S, L);

    if ((double)shL <= threshold)
        std::cout << "1.0\n";
    else
    {

        double rat = (double)threshold / (double)shL;
        double score = pow(rat, 2);
        std::cout << std::min(score, 1.0) * 0.3 << "\n";
    }
    return 0;
}
