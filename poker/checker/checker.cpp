#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

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

void error(const std::string& message)
{
    std::cerr << message << std::endl;
    std::cout << 0 << std::endl;
    exit(0);
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

void parseStrategy(std::ifstream& stratFile)
{
    std::string line;
    std::string word;

    int card;
    std::string situation;
    std::string action;
    double prob;

    std::fill(openBetProbs, openBetProbs + n, -1);
    std::fill(checkBetProbs, checkBetProbs + n, -1);
    std::fill(betCallProbs, betCallProbs + n, -1);
    std::fill(checkBetCallProbs, checkBetCallProbs + n, -1);

    while (!stratFile.eof())
    {
        std::getline(stratFile, line);
        std::stringstream lineStream(line);

        word = "";
        lineStream >> word;
        if (word == "") continue;

        card = std::stoi(word) - 1;
        
        word = "";
        lineStream >> word;
        if (word == "") error("Unexpected line end in.");
    
        situation = word;
        if (situation.back() != ':') error("Expected ':' after situation.");
        situation = situation.substr(0, situation.size() - 1);
        
        word = "";
        lineStream >> word;
        if (word == "") error("Unexpected line end.");
    
        action = word;

        word = "";
        lineStream >> word;

        if (word == "") prob = 1;
        else prob = std::stod(word);

        word = "";
        lineStream >> word;
        if (word != "") error("Expected line end.");

        if (card < 0 || card > n) error("Invalid card number.");

        if (situation != "open" && situation != "check" && situation != "bet" && situation != "check-bet") error("Invalid situation.");

        double* toSet = nullptr;

        if (situation == "open" && (action == "check" || action == "bet")) toSet = &openBetProbs[card];
        else if (situation == "check" && (action == "check" || action == "bet")) toSet = &checkBetProbs[card];
        else if (situation == "bet" && (action == "fold" || action == "call")) toSet = &betCallProbs[card];
        else if (situation == "check-bet" && (action == "fold" || action == "call")) toSet = &checkBetCallProbs[card];
        else error("Invalid action.");

        if (prob < 0 || prob > 1) error("Invalid probability.");

        if (*toSet >= 0) error("Overspecified card-situation pair.");

        if (action == "check" || action == "fold") *toSet = 1 - prob;
        else if (action == "bet" || action == "call") *toSet = prob;
        else error("Internal error.");
    }

    for (int i = 0; i < n; ++i)
    {
        if (openBetProbs[i] < 0 || checkBetProbs[i] < 0 || betCallProbs[i] < 0 || checkBetCallProbs[i] < 0) error("Unspecified card-situation pair.");
    }
}

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in.is_open())
    {
        std::cerr << "In-file " << argv[2] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    if (!out.is_open())
    {
        std::cerr << "Out-file " << argv[3] << " not found." << std::endl;
        std::cout << 0 << std::endl;
        return 0;
    }

    in >> n >> ante >> betSize;

    parseStrategy(out);

    findExpProfit();

    double score = std::min(std::max(2 * expProfit / (0.99 * ante), 0.0), 1.0);
    score = score * score * score * score * score;

    std::cout << std::fixed << std::setprecision(4) << score << std::endl;
    std::cerr << "Average profit: " << std::fixed << std::setprecision(4) << expProfit << std::endl;

    //info();

    return 0;
}
