#include <iostream>
#include <queue>
#include <unordered_map>

typedef unsigned long long hashType;

const hashType BASE = 29;

std::string s;
int q;

std::deque<int> seq;
std::unordered_map<hashType, int> substrings;

std::vector<hashType> pows = {1};
hashType powBase(int n)
{
    while (pows.size() <= n)
    {
        pows.push_back(BASE * pows.back());
    }
    return pows[n];
}

hashType newHash()
{
    return 0;
}

void addToHash(hashType& h, int num)
{
    h = h * BASE + num;
}

void removeFromHash(hashType& h, int num, int length)
{
    h -= num * powBase(length - 1);
}

void addChar(char c)
{
    seq.push_front(c + 1 - 'a');
    hashType h = newHash();
    for (auto it = seq.begin(); it!= seq.end(); ++it)
    {
        addToHash(h, *it);
        auto mit = substrings.find(h);
        if (mit == substrings.end()) substrings.insert({h, 1});
        else ++(mit->second);
    }
}

void removeChar()
{
    int len = 0;
    hashType h = newHash();
    for (auto it = seq.begin(); it!= seq.end(); ++it)
    {
        addToHash(h, *it);
        ++len;
    }
    for (auto it = seq.begin(); it!= seq.end(); ++it)
    {
        auto mit = substrings.find(h);
        if (mit->second == 1) substrings.erase(mit);
        else --(mit->second);
        removeFromHash(h, *it, len);
        --len;
    }
    seq.pop_back();
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> s;

    for (char c : s)
    {
        addChar(c);
    }

    std::cout << substrings.size() << " ";

    std::cin >> q;

    for (int i = 0; i < q; ++i)
    {
        int type;
        std::cin >> type;

        if (type == 1)
        {
            char c;
            std::cin >> c;
            addChar(c);
        }
        else if (type == 2)
        {
            removeChar();
        }

        std::cout << substrings.size() << " ";
    }

    std::cout << "\n";

    return 0;
}
