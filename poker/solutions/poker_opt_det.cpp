#include <iostream>
#include <iomanip>
#include <random>

std::mt19937 generator;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

double randReal(double lb, double ub)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

const int MAX_N = 50;

int n;
double ante;
double betSize;

double openBetProbs[MAX_N];
double checkBetProbs[MAX_N];
double betCallProbs[MAX_N];
double checkBetCallProbs[MAX_N];

bool oppOpenBet[MAX_N];
bool oppCheckBet[MAX_N];
bool oppBetCall[MAX_N];
bool oppCheckBetCall[MAX_N];

double firstExpProfits[MAX_N];
double secondExpProfits[MAX_N];
double expProfit;

void input()
{
    std::cin >> n >> ante >> betSize;
}

void outputOne(int card, const std::string& sitatuion, const std::string& neg, const std::string& pos, double prob)
{
    std::cout << card + 1 << " " << sitatuion << ": ";
    if (prob == 0) std::cout << neg;
    else if (prob == 1) std::cout << pos;
    else std::cout << pos << " " << prob;
    std::cout << std::endl;
}

void output()
{
    for (int i = 0; i < n; ++i)
    {
        outputOne(i, "open", "check", "bet", openBetProbs[i]);
        outputOne(i, "check", "check", "bet", checkBetProbs[i]);
        outputOne(i, "bet", "fold", "call", betCallProbs[i]);
        outputOne(i, "check-bet", "fold", "call", checkBetCallProbs[i]);
    }
}

double checkCheck(int self, int opponent)
{
    if (self < opponent) return 0;
    else return ante; 
}

double betCall(int self, int opponent)
{
    if (self < opponent) return -betSize;
    else return (ante + betSize); 
}

void findExpProfit()
{
    for (int i = 0; i < n; ++i)
    {
        double oppCheckBetCallExp = 0;
        double oppCheckBetFoldExp = 0;

        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;

           oppCheckBetCallExp += checkBetProbs[j] * betCall(i, j);
        }

        oppCheckBetCall[i] = oppCheckBetCallExp > oppCheckBetFoldExp;

        double oppOpenBetExp = 0;
        double oppOpenCheckExp = 0;

        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;

            oppOpenBetExp += (1 - betCallProbs[j]) * ante;
            oppOpenBetExp += betCallProbs[j] * betCall(i, j);

            oppOpenCheckExp += (1 - checkBetProbs[j]) * checkCheck(i, j);
            oppOpenCheckExp += checkBetProbs[j] * (1 - oppCheckBetCall[i]) * 0;
            oppOpenCheckExp += checkBetProbs[j] * oppCheckBetCall[i] * betCall(i, j);
        }

        oppOpenBet[i] = oppOpenBetExp > oppOpenCheckExp;

        double oppCheckBetExp = 0;
        double oppCheckCheckExp = 0;

        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;

            oppCheckBetExp += (1 - openBetProbs[j]) * (1 - checkBetCallProbs[j]) * ante;
            oppCheckBetExp += (1 - openBetProbs[j]) * checkBetCallProbs[j] * betCall(i, j);

            oppCheckCheckExp += (1 - openBetProbs[j]) * checkCheck(i, j);
        }

        oppCheckBet[i] = oppCheckBetExp > oppCheckCheckExp;

        double oppBetCallExp = 0;
        double oppBetFoldExp = 0;

        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;

            oppBetCallExp += openBetProbs[j] * betCall(i, j);
        }

        oppBetCall[i] = oppBetCallExp > oppBetFoldExp;
    }

    expProfit = 0;

    for (int i = 0; i < n; ++i)
    {
        firstExpProfits[i] = 0;
        secondExpProfits[i] = 0;

        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;

            firstExpProfits[i] += openBetProbs[i] * (1 - oppBetCall[j]) * ante;
            firstExpProfits[i] += openBetProbs[i] * oppBetCall[j] * betCall(i, j);
            firstExpProfits[i] += (1 - openBetProbs[i]) * (1 - oppCheckBet[j]) * checkCheck(i, j);
            firstExpProfits[i] += (1 - openBetProbs[i]) * oppCheckBet[j] * (1 - checkBetCallProbs[i]) * 0;
            firstExpProfits[i] += (1 - openBetProbs[i]) * oppCheckBet[j] * checkBetCallProbs[i] * betCall(i, j);

            secondExpProfits[i] += oppOpenBet[j] * (1 - betCallProbs[i]) * 0;
            secondExpProfits[i] += oppOpenBet[j] * betCallProbs[i] * betCall(i, j);
            secondExpProfits[i] += (1 - oppOpenBet[j]) * (1 - checkBetProbs[i]) * checkCheck(i, j);
            secondExpProfits[i] += (1 - oppOpenBet[j]) * checkBetProbs[i] * (1 - oppCheckBetCall[j]) * ante;
            secondExpProfits[i] += (1 - oppOpenBet[j]) * checkBetProbs[i] * oppCheckBetCall[j] * betCall(i, j);
        }

        firstExpProfits[i] /= (n - 1);
        secondExpProfits[i] /= (n - 1);

        expProfit += firstExpProfits[i];
        expProfit += secondExpProfits[i];
    }

    expProfit /= 2 * n;
}

void info()
{
    findExpProfit();

    std::cerr << "Normalized profit: " << std::fixed << std::setprecision(4) << 2 * expProfit / ante << std::endl;
    std::cerr << "Average profit: " << std::fixed << std::setprecision(4) << expProfit << std::endl;

    double avgOpenBetProb = 0;
    double avgBetCallProb = 0;
    double avgCheckBetProb = 0;
    double avgCheckBetCallProb = 0;

    for (int i = 0; i < n; ++i)
    {
        avgOpenBetProb += openBetProbs[i] / n;
        avgBetCallProb += betCallProbs[i] / n;
        avgCheckBetProb += checkBetProbs[i] / n;
        avgCheckBetCallProb += checkBetCallProbs[i] / n;
    }

    std::cerr << std::endl;
    std::cerr << "Average bet/call probabilities breakdown:" << std::endl;
    std::cerr << "open: " << std::fixed << std::setprecision(4) << avgOpenBetProb << std::endl;
    std::cerr << "check: " << std::fixed << std::setprecision(4) << avgCheckBetProb << std::endl;
    std::cerr << "bet: " << std::fixed << std::setprecision(4) << avgBetCallProb << std::endl;
    std::cerr << "check-bet: " << std::fixed << std::setprecision(4) << avgCheckBetCallProb << std::endl;

    std::cerr << std::endl;
    std::cerr << "Profit breakdown:" << std::endl;

    for (int i = 0; i < n; ++i)
    {
        std::cerr << i + 1;
        if (i + 1 < 10) std::cerr << " ";
        std::cerr << "  " << std::fixed << std::setprecision(4) << firstExpProfits[i];
        std::cerr << "  " << std::fixed << std::setprecision(4) << secondExpProfits[i];
        std::cerr << std::endl;
    }

    std::cerr << std::endl;
    std::cerr << "Opponent strategy:" << std::endl;
    std::cerr << "card  open   check  bet    check-bet" << std::endl;

    for (int i = 0; i < n; ++i)
    {
        std::cerr << i + 1 << "  ";
        if (i + 1 < 10) std::cerr << " ";
        std::cerr << "  " << (oppOpenBet[i] ? "bet  " : "check");
        std::cerr << "  " << (oppCheckBet[i] ? "bet  " : "check");
        std::cerr << "  " << (oppBetCall[i] ? "call " : "fold ");
        std::cerr << "  " << (oppCheckBetCall[i] ? "call " : "fold ");
        std::cerr << std::endl;
    }
}

void init()
{
    for (int i = 0; i < n; ++i)
    {
        openBetProbs[i] = randNum(0, 2);
        betCallProbs[i] = randNum(0, 2);
        checkBetProbs[i] = randNum(0, 2);
        checkBetCallProbs[i] = randNum(0, 2);
    }
}

void localOpt()
{
    findExpProfit();

    double* sitProbs[4] = {openBetProbs, betCallProbs, checkBetProbs, checkBetCallProbs};

    for (int i = 0; i < 1e5; ++i)
    {
        int situation = randNum(0, 4);
        int card = randNum(0, n);

        double& prob = sitProbs[situation][card];

        double oldExpProfit = expProfit;
        double oldProb = prob;

        prob = randNum(0, 2);

        findExpProfit();

        if (expProfit < oldExpProfit)
        {
            expProfit = oldExpProfit;
            prob = oldProb;
        }
    }
}

double bestFirstBetProbs[MAX_N];
double bestBetCallProbs[MAX_N];
double bestCheckBetProbs[MAX_N];
double bestCheckBetCallProbs[MAX_N];

void solve()
{
    double bestExpProfit = 0;

    for (int i = 0; i < 10; ++i)
    {
        std::cerr << ".";

        init();
        localOpt();

        if (expProfit > bestExpProfit)
        {
            bestExpProfit = expProfit;

            std::copy(openBetProbs, openBetProbs + n, bestFirstBetProbs);
            std::copy(betCallProbs, betCallProbs + n, bestBetCallProbs);
            std::copy(checkBetProbs, checkBetProbs + n, bestCheckBetProbs);
            std::copy(checkBetCallProbs, checkBetCallProbs + n, bestCheckBetCallProbs);
        }
    }

    std::cerr << std::endl;

    std::copy(bestFirstBetProbs, bestFirstBetProbs + n, openBetProbs);
    std::copy(bestBetCallProbs, bestBetCallProbs + n, betCallProbs);
    std::copy(bestCheckBetProbs, bestCheckBetProbs + n, checkBetProbs);
    std::copy(bestCheckBetCallProbs, bestCheckBetCallProbs + n, checkBetCallProbs);
}

int main(int argc, char *argv[])
{
    generator.seed(0);

    input();
    solve();
    info();
    output();

    return 0;
}
