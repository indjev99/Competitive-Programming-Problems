#include <iostream>
#include <vector>

int base;

const std::string X = "X", X2 = "X'", Y = "Y", N = "N", neg0 = "-0", major = "Major", minor = "Minor";

void listStates(std::vector<std::string> states)
{
    if (states.empty())
    {
        std::cout << "? ";
        return;
    }
    for (int i = 0; i < states.size(); ++i)
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
    transition({}, {X2, Y, N}, {Y}, Y);
    transition({}, {X2, Y, N}, {N}, N);
    std::cout << std::endl;
}


void consumptions()
{
    for (int i = -base + 2; i <= base - 2; i += 2)
    {
        std::vector<std::string> lefts = {S(i)};
        if (i == 0) lefts = {X, X2, S(0), S(base), S(-base)};
        transition(lefts, {Y}, {Y}, S(i + 2));
        transition(lefts, {Y}, {X, X2}, S(i + 1));
        transition(lefts, {Y}, {N}, S(i));
        transition(lefts, {N}, {Y}, S(i));
        transition(lefts, {N}, {X, X2}, S(i - 1));
        transition(lefts, {N}, {N}, S(i - 2));
        std::cout << std::endl;
    }
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
            normal.push_back(neg0);
            transition(lefts, normal, {}, S(i));
            normal.pop_back();
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
        neg.push_back(i > 0 ? S(-i) : neg0);
    }
    std::vector<std::string> walls({X, X2});

    transition(walls, pos, walls, major);
    transition(walls, neg, walls, minor);
    transition({S(0), S(base), S(-base)}, neg, walls, neg0);
    std::cout << std::endl;
}

int main()
{
    std::cin >> base;

    consumptions();
    border();
    counter();
    leftShifts();

    return 0;
}
