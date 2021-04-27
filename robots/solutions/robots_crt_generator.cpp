#include <iostream>
#include <assert.h>
#include <vector>

int modFront;
int modBack;

const std::string X = "X", Y = "Y", N = "N", major = "Major", minor = "Minor";

void listStates(std::vector<std::string> states)
{
    if (states.empty())
    {
        std::cout << "? ";
        return;
    }
    for (int i = 0; i < (int) states.size(); ++i)
    {
        if (i > 0) std::cout << "/";
        std::cout << states[i];
    }
    std::cout << " ";
}

void transition(std::vector<std::string> lefts, std::vector<std::string> mids, std::vector<std::string> rights, std::string end)
{
    listStates(lefts);
    listStates(mids);
    listStates(rights);
    std::cout << "-> " << end << std::endl;
}

std::string S(int k, std::string YN="")
{
    if (YN == Y) return std::to_string(k) + "_" + Y;
    else if (YN == N) return std::to_string(k) + "_" + N;
    else if (k == modBack - 2) return N;
    else if (k == modBack - 1) return Y;
    else return std::to_string(k) + "_" + Y;
}

void catchall()
{
    transition({}, {}, {}, Y);
    std::cout << std::endl;
}

void stays()
{
    transition({}, {Y}, {}, Y);
    transition({}, {N}, {}, N);
    std::cout << std::endl;
}

void frontCounter()
{
    for (int i = 0; i < modFront; ++i)
    {
        std::vector<std::string> lefts = {S(i, Y), S(i, N)};
        if (i == 0) lefts.push_back(X);
        transition(lefts, {Y}, {}, S((i + 1) % modFront, Y));
        transition(lefts, {N}, {}, S((i - 1 + modFront) % modFront, N));
        std::cout << std::endl;
    }
}

void backCounter()
{
    std::vector<std::string> yes, no;
    for (int i = 0; i < modFront; ++i)
    {
        yes.push_back(S(i, Y));
        no.push_back(S(i, N));
    }

    for (int i = 0; i < modBack; ++i)
    {
        std::vector<std::string> lefts = {S(i)};
        if (i == 0) lefts.push_back(X);
        transition(lefts, yes, {}, S((i + 1) % modBack));
        transition(lefts, no, {}, S((i - 1 + modBack) % modBack));
        std::cout << std::endl;
    }
}

int frontCoeff, backCoeff;
int product;

void findCoeffs()
{
    int a = modFront;
    int b = modBack;
    int ax = 1, ay = 0;
    int bx = 0, by = 1;
    while (b != 0)
    {
        int ratio = a / b;
        int temp = a - ratio * b;
        a = b;
        b = temp;
        temp = ax - ratio * bx;
        ax = bx;
        bx = temp;
        temp = ay - ratio * by;
        ay = by;
        by = temp;
    }
    assert(a == 1);
    frontCoeff = ax;
    backCoeff = ay;
    product = modFront * modBack;
    assert(frontCoeff * modFront + backCoeff * modBack == 1);
}

std::string isMajority(int a, int b)
{
    int diff = (a * backCoeff * modBack + b * frontCoeff * modFront);
    diff = (diff % product + product) % product;
    assert(diff % modFront == a);
    assert(diff % modBack == b);
    if (diff <= product / 2) return major;
    else return minor;
}

void border()
{
    for (int i = 0; i < modFront; ++i)
    {
        for (int j = 0; j < modBack; ++j)
        {
            std::vector<std::string> lefts = {S(j)};
            if (j == 0) lefts.push_back(X);
            transition(lefts, {S(i, Y)}, {X}, isMajority(i, (j + 1) % modBack));
            transition(lefts, {S(i, N)}, {X}, isMajority(i, (j - 1 + modBack) % modBack));  
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::cin >> modFront >> modBack;
    findCoeffs();

    border();
    frontCounter();
    backCounter();
    stays();
    catchall();

    return 0;
}
