#include <iostream>
#include <vector>

int base;

const std::string X = "X", X2 = "X'", posX2 = "+X'", negX2 = "-X'", Y = "Y", N = "N", major = "Major", minor = "Minor";

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

std::string S(int k)
{
    return std::to_string(k);
}

void leftShifts()
{
    transition({}, {X2, Y, N}, {X, X2}, X2);
    transition({}, {Y, N}, {Y}, Y);
    transition({}, {Y, N}, {N}, N);
    std::cout << std::endl;
}

void consumptions()
{
    for (int i = -base + 2; i <= base - 2; i += 2)
    {
        std::vector<std::string> lefts = {S(i)};
        if (i == 0) lefts = {X, X2, S(0), S(base), S(-base)};
        transition(lefts, {Y}, {Y}, S(i + 2));
        transition(lefts, {Y}, {N}, S(i));
        transition(lefts, {N}, {Y}, S(i));
        transition(lefts, {N}, {N}, S(i - 2));
        std::cout << std::endl;
    }
}

void borderConsumptions()
{
    std::vector<std::string> pos, zeroPos, neg, zeroNeg;

    zeroPos = zeroNeg = {X, X2, S(0), S(base), S(-base)};

    for (int i = 2; i <= base - 2; i += 2)
    {
        pos.push_back(S(i));
        zeroPos.push_back(S(i));
        neg.push_back(S(-i));
        zeroNeg.push_back(S(-i));
    }

    transition(zeroPos, {Y}, {X, X2}, posX2);
    if (base > 2)
    {
        transition(pos, {N}, {X, X2}, posX2);
        transition(neg, {Y}, {X, X2}, negX2);
    }
    transition(zeroNeg, {N}, {X, X2}, negX2);

    std::cout << std::endl;
}

void counter()
{
    std::vector<std::string> normal;
    for (int i = -base + 1; i <= base - 1; ++i)
    {
        normal.push_back(S(i));
    }
    for (int i = -base + 1; i <= base - 1; ++i)
    {
        std::vector<std::string> lefts = {S(i)};
        if (i == 0)
        {
            normal.push_back(X2);
            transition({X, X2}, normal, {}, X2);
            normal.pop_back();
            transition({S(0), S(base), S(-base)}, normal, {}, S(i));
            lefts = {X, X2, S(0), S(base), S(-base)};
        }
        else
        {
            transition(lefts, normal, {}, S(i));
        }
        transition(lefts, {S(base)}, {}, S(i + 1));
        transition(lefts, {S(-base)}, {}, S(i - 1));
        std::cout << std::endl;
    }
}

void border()
{
    std::vector<std::string> pos, neg;
    for (int i = 0; i <= base - 1; ++i)
    {
        pos.push_back(S(i));
        if (i > 0) neg.push_back(S(-i));
    }

    pos.push_back(posX2);
    transition({X, X2}, pos, {X, X2, posX2}, major);
    pos.pop_back();
    pos.erase(pos.begin());
    transition({X, X2}, pos, {negX2}, major);
    std::cout << std::endl;

    neg.push_back(negX2);
    transition({X, X2}, neg, {X, X2, posX2, negX2}, minor);
    transition({X, X2}, {S(0)}, {negX2}, minor);
    std::cout << std::endl;

    transition({S(0), S(base), S(-base)}, {X2, posX2}, {X, X2}, posX2);
    transition({S(0), S(base), S(-base)}, {negX2}, {X, X2}, negX2);
    std::cout << std::endl;

    neg.pop_back();
    transition(pos, {X2, posX2, negX2}, {X, X2}, posX2);
    transition(neg, {X2, posX2, negX2}, {X, X2}, negX2);
    std::cout << std::endl;
}

int main()
{
    std::cin >> base;

    consumptions();
    borderConsumptions();
    border();
    counter();
    leftShifts();

    return 0;
}
