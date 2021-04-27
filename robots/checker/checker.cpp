#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <math.h>
#include <vector>
#include <string>
#include <map>

const int MAX_N = 1500;
const int MAX_STATES = 30;
const int MAX_ITERS = 7500;

const int TARGET_ITERS = 753;
const int TARGET2_ITERS = 760;

const int EXTRA_STATES = 3;
const int TOTAL_STATES = MAX_STATES + EXTRA_STATES;

const std::string ARROW = "->";
const std::string WILDCARD = "?";
const std::string EMPTY = "X";
const std::string YES = "Y";
const std::string NO = "N";
const std::string MAJORITY = "Major";
const std::string MINORITY = "Minor";

const int wildcard = -1;
int emptyState, yesState, noState, majorityState, minorityState;
int trans[TOTAL_STATES][TOTAL_STATES][TOTAL_STATES];
bool isSet[TOTAL_STATES][TOTAL_STATES][TOTAL_STATES];

void fail(const std::string& message)
{
    std::cerr << message << std::endl;
    std::cout << 0 << std::endl;
    exit(0);
}

void myAssert(bool cond, const std::string& message)
{
    if (!cond) fail(message);
}

int freeState = 0;
std::map<std::string, int> stringToState;
std::map<int, std::string> stateToString;

int toState(const std::string& word)
{
    if (word == WILDCARD) return wildcard;
    if (stringToState.find(word) == stringToState.end())
    {
        myAssert(freeState < TOTAL_STATES, "Too many states: " + std::to_string(freeState + 1 - EXTRA_STATES));
        stringToState[word] = freeState;
        stateToString[freeState] = word;
        ++freeState;
    }
    return stringToState[word];
}

std::string toString(int state)
{
    if (state == wildcard) return WILDCARD;
    return stateToString[state];
}

std::vector<int> toStates(const std::string& wordList)
{
    std::string word;
    std::vector<int> states;
    std::stringstream wordStream(wordList);
    while (!wordStream.eof())
    {
        getline(wordStream, word, '/');
        if (word == "") continue;
        states.push_back(toState(word));
    }
    return states;
}

std::vector<int> allStates(const std::vector<int>& states)
{
    bool noWildcard = true;
    for (int state : states)
    {
        if (state == wildcard) noWildcard = false;
    }
    if (noWildcard) return states;
    else
    {
        std::vector<int> everything(TOTAL_STATES);
        std::iota(everything.begin(), everything.end(), 0);
        return everything;
    }
}

void setTrans(int left, int mid, int right, int end)
{
    if (isSet[left][mid][right]) return;
    isSet[left][mid][right] = true;
    trans[left][mid][right] = end;
}

void readMachine(std::istream& in)
{
    std::string arrow, orderStr, leftStr, midStr, rightStr, endStr;
    std::vector<int> lefts, mids, rights, ends;
    int end;

    emptyState = toState(EMPTY);
    yesState = toState(YES);
    noState = toState(NO);
    majorityState = toState(MAJORITY);
    minorityState = toState(MINORITY);

    while (!in.eof())
    {
        leftStr = midStr = rightStr = arrow = endStr = "";
        in >> leftStr >> midStr >> rightStr >> arrow >> endStr;
        if (leftStr == "" || midStr == "" || rightStr == "" || arrow == "" || endStr == "") break;

        myAssert(arrow == ARROW, "Not a valid arrow separator: " + arrow);

        lefts = toStates(leftStr);
        mids = toStates(midStr);
        rights = toStates(rightStr);
        ends = toStates(endStr);

        myAssert(!lefts.empty(), "No left states: " + leftStr);
        myAssert(!mids.empty(), "No mid states: " + midStr);
        myAssert(!rights.empty(), "No right states: " + rightStr);
        myAssert(!ends.empty(), "No end state: " + endStr);
        myAssert(ends.size() <= 1, "More than one end state: " + endStr);

        end = ends.front();

        for (int left : lefts)
        {
            myAssert(left != majorityState && left != minorityState, "Invalid left state: " + toString(left));
        }
        for (int mid : mids)
        {
            myAssert(mid != emptyState && mid != majorityState && mid != minorityState, "Invalid mid state: " + toString(mid));
        }
        for (int right : rights)
        {
            myAssert(right != majorityState && right != minorityState, "Invalid right state: " + toString(right));
        }

        myAssert(end != wildcard && end != emptyState, "Invalid end state: " + toString(end));

        for (int left : allStates(lefts))
        {
            for (int mid : allStates(mids))
            {
                for (int right : allStates(rights))
                {
                    setTrans(left, mid, right, end);
                }
            }
        }
    }

    myAssert(freeState <= TOTAL_STATES, "Too many states: " + std::to_string(freeState - EXTRA_STATES));
}

int n;
bool vote[MAX_N];

void readVotes(std::istream& in)
{
    char v;

    in >> n;
    myAssert(1 <= n && n <= MAX_N, "Invalid N: " + std::to_string(n));

    for (int i = 0; i < n; ++i)
    {
        in >> v;
        vote[i] = v == 'Y';
        myAssert(v == 'Y' || v == 'N', "Invalid vote: " + std::string(1, v));
    }
}

bool findAns()
{
    int numVotes = 0;
    for (int i = 0; i < n; ++i)
    {
        if (vote[i]) ++numVotes;
    }
    return numVotes >= (n + 1) / 2;
}

int state[MAX_N + 2];
int newState[MAX_N + 2];

#define NO_RES 0
#define MAJORITY_RES 1
#define MINORITY_RES 2

std::pair<int, int> runMachine()
{
    state[0] = state[n + 1] = emptyState;
    for (int i = 1; i <= n; ++i)
    {
        state[i] = vote[i  - 1] ? yesState : noState;
    }

    int iters = 0;
    bool noChange = false;
    int result = NO_RES;
    while (iters < MAX_ITERS && !noChange && result == NO_RES)
    {
        for (int i = 1; i <= n; ++i)
        {
            if (!isSet[state[i - 1]][state[i]][state[i + 1]])
            {
                fail("No transition from: " + toString(state[i - 1]) + " " + toString(state[i]) + " " + toString(state[i + 1]));
            }
            newState[i] = trans[state[i - 1]][state[i]][state[i + 1]];
        }

        noChange = true;
        result = NO_RES;
        for (int i = 1; i <= n; ++i)
        {
            if (newState[i] != state[i]) noChange = false;
            if (newState[i] == majorityState && result == NO_RES) result = MAJORITY_RES;
            if (newState[i] == minorityState && result == NO_RES) result = MINORITY_RES;
            state[i] = newState[i];
        }

        ++iters;
    }

    return {result, iters};
}

std::pair<double, int> oneTest(std::ifstream& in, int targetType)
{
    readVotes(in);
    bool ansMajority = findAns();
    std::pair<int, int> resIters = runMachine();

    myAssert(resIters.first != NO_RES, "Robots did not decide");
    myAssert((resIters.first == MAJORITY_RES) == ansMajority, "Robots decided incorrectly");

    int target = targetType == 0 ? n / 2 + 3 : n + 1;

    double score;
    if (resIters.second <= target) score = 1.0;
    else if (resIters.second <= target + 7) score = 0.85;
    else score = std::max(0.75 * pow((double) (target + 8) / resIters.second, 0.85), 0.2);

    return {score, resIters.second};
}

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    myAssert(in.is_open(), "In-file " + std::string(argv[1]) + " not found.");
    myAssert(out.is_open(), "Out-file " + std::string(argv[3]) + " not found.");

    readMachine(out);

    int targetType;
    in >> targetType;

    int maxIters = 0;
    double score = 1.0;
    for (int i = 0; i < 10; ++i)
    {
        std::pair<double, int> res = oneTest(in, targetType);
        score = std::min(score, res.first);
        maxIters = std::max(maxIters, res.second);
    }

    std::cerr << "Num states: " << freeState - EXTRA_STATES << std::endl;
    std::cerr << "Maximum iterations: " << maxIters << std::endl;
    std::cout << score << std::endl;

    return 0;
}
