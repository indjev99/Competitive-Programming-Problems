#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

#define ADD 1
#define REMOVE 2

const int NUM_LETS = 26;
const int MAX_N = 1e5;
const int MAX_Q = 1e5;
const int MAX_LEN = MAX_N + MAX_Q;
const int MAX_LOG = 21;

struct Query
{
    int type;
    char c;
};

std::string s;
std::vector<Query> qs;
int n;

void input()
{
    int q;
    std::cin >> s;
    std::cin >> q;
    for (int i = 0; i < q; ++i)
    {
        int type;
        char c;
        std::cin >> type;
        if (type == ADD) std::cin >> c;
        qs.push_back({type, c});
    }
}

int seq[MAX_LEN];

//fills seq with all characters (initial + queries) in reverse order
void buildSeq()
{
    n = 0;
    for (auto rit = qs.rbegin(); rit != qs.rend(); ++rit)
    {
        if (rit->type == ADD) seq[n++] = rit->c - 'a';
    }
    for (auto rit = s.rbegin(); rit != s.rend(); ++rit)
    {
        seq[n++] = *rit - 'a';
    }
}

int maxLev;
int pows[MAX_LOG];
int ranks[MAX_LOG][MAX_LEN];
int suffOrd[MAX_LEN];

//sorts a single group of suffixes (can be made linear with radix sort)
void process(int i, int l, int r)
{
    //compare two suffixes on the ranks of the second halves
    auto cmp = [i](int a, int b) {
        a += pows[i - 1];;
        b += pows[i - 1];
        int av = a < n ? ranks[i - 1][a] : -1;
        int bv = b < n ? ranks[i - 1][b] : -1;
        return av < bv;
    };

    std::sort(suffOrd + l, suffOrd + r, cmp);

    //assign the new ranks
    for (int j = l; j < r; ++j)
    {
        if (j > l && !cmp(suffOrd[j - 1], suffOrd[j]))
            ranks[i][suffOrd[j]] = ranks[i][suffOrd[j - 1]];
        else ranks[i][suffOrd[j]] = j;
    }
}

std::vector<int> buckets[NUM_LETS];

//builds the suffix array
void buildSuffixArray()
{
    //find the max level
    maxLev = 1;
    pows[0] = 1;
    while (pows[maxLev - 1] < n)
    {
        pows[maxLev] = pows[maxLev - 1] * 2;
        ++maxLev;
    }

    //initial phase based on single characters
    for (int j = 0; j < n; ++j)
    {
        ranks[0][j] = seq[j];
        buckets[seq[j]].push_back(j);
    }
    int pos = 0;
    for (int i = 0; i < NUM_LETS; ++i)
    {
        for (int j : buckets[i])
        {
            suffOrd[pos++] = j;
        }
    }

    //merge suffixes into larger ones iteratively
    for (int i = 1; i < maxLev; ++i)
    {
        int start = 0;
        for (int j = 1; j < n; ++j)
        {
            if (ranks[i - 1][suffOrd[j - 1]] < ranks[i - 1][suffOrd[j]])
            {
                process(i, start, j);
                start = j;
            }
        }
        process(i, start, n);
    }
}

//finds the longest common prefix of two suffixes
int findLcp(int a, int b)
{
    int lcp = 0;
    for (int i = maxLev - 1; i >= 0 && a < n && b < n; --i)
    {
        if (ranks[i][a] == ranks[i][b])
        {
            lcp += pows[i];
            a += pows[i];
            b += pows[i];
        }
    }
    return lcp;
}

struct Suffix
{
    int idx;
    int r;
};

bool operator<(const Suffix& a, const Suffix& b)
{
    return a.r < b.r;
}

//current number of different substrings
long long cnt;

//positions in seq for adding and removing respectively
int firstPos, lastPos;

//all the relevant suffixes ordered lexicographically
std::set<Suffix> suffs;

//whether the suffix has been deleted, used for efficiency
bool deleted[MAX_LEN];

//time (value of lastPost) when we plan to remove the suffix
int removeTime[MAX_LEN];

//the suffixes we want to remove at some time
std::vector<std::pair<std::set<Suffix>::iterator, int>> removeAtTime[MAX_LEN];

void updateLcp(std::set<Suffix>::iterator currIt, int curr, int lcp);

//deletes a suffix from the set
void deleteSuff(std::set<Suffix>::iterator currIt, int curr)
{
    //if it is already deleted, do nothing
    if (deleted[curr]) return;
    deleted[curr] = true;

    //if suffix has both neighbours
    //we need to update the shortest one
    //we might need to move up its deletion time
    //we delete the suffix after getting its neighbours
    auto rightIt = std::next(currIt);
    if (currIt != suffs.begin() && rightIt != suffs.end())
    {
        auto leftIt = std::prev(currIt);
        suffs.erase(currIt);
        int left = leftIt->idx;
        int right = rightIt->idx;
        int lcp = findLcp(left, right);
        if (left < right) updateLcp(rightIt, right, lcp);
        else updateLcp(leftIt, left, lcp);
    }
    else suffs.erase(currIt);
}

//updates a suffix with a new potential match length
void updateLcp(std::set<Suffix>::iterator currIt, int curr, int lcp)
{
    //this is the point when curr will become a prefix of the suffix updating it
    int toRem = curr + lcp;
    if (toRem <= removeTime[curr]) return;
    removeTime[curr] = toRem;

    //if the new removal time has already passed directly delete it
    //otherwise (re)schedule it for deletion
    if (toRem >= lastPos) deleteSuff(currIt, curr);
    else removeAtTime[toRem].push_back({currIt, curr});
}

//adds the rightmost not yet added character
void addChar()
{
    //move the insertion postion
    --firstPos;

    //insert the suffix and keep its iterator for efficiency
    auto ret = suffs.insert({firstPos, ranks[maxLev - 1][firstPos]});
    auto it = ret.first;
    int lcpL = 0, lcpR = 0;

    //update its left neighbour and get the LCP with it
    if (it != suffs.begin())
    {
        auto leftIt = std::prev(it);
        int left = leftIt->idx;
        lcpL = findLcp(firstPos, left);
        updateLcp(leftIt, left, lcpL);
        //ensure we are not matching over deleted characters
        lcpL = std::min(lcpL, lastPos - left);
    }

    //update its right neighbour and get the LCP with it
    auto rightIt = std::next(it);
    if (rightIt != suffs.end())
    {
        int right = rightIt->idx;
        lcpR = findLcp(firstPos, right);
        updateLcp(rightIt, right, lcpR);
        //ensure we are not matching over deleted characters
        lcpR = std::min(lcpR, lastPos - right);
    }

    //this suffix adds len - lcp new substrings
    cnt += lastPos - firstPos - std::max(lcpL, lcpR);

    //schedule for deletion when its length becomes zero
    updateLcp(it, firstPos, 0);
}

//deleted the rightmost not yet deleted character
void removeChar()
{
    //all different suffixes lose one of their substrings
    cnt -= suffs.size();

    //move the deletion pointer
    --lastPos;

    //remove all suffixes that are now a prefix of another and update their neighbours
    for (auto pr : removeAtTime[lastPos])
    {
        deleteSuff(pr.first, pr.second);
    }
    removeAtTime[lastPos].clear();
}

//finds the count after inserting the string
void firstCount()
{
    //initialize the insertion and deletion pointers and the deletion times
    firstPos = n;
    lastPos = n;
    std::fill(removeTime, removeTime + n, -1);

    //add all characters from the string
    for (int i = 0; i < s.size(); ++i)
    {
        addChar();
    }
}

//the main solve function
void solve()
{
    //build the sequence
    buildSeq();

    //build the suffix array
    buildSuffixArray();

    //find the answer after inserting the string
    firstCount();
    std::cout << cnt << " ";

    //process all the queries
    for (const Query& q : qs)
    {
        if (q.type == ADD) addChar();
        else removeChar();
        std::cout << cnt << " ";
    }
    std::cout << "\n";
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();

    return 0;
}
