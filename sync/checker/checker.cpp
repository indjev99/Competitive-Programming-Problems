#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <string>
#include <cmath>
#include <map>

const int MAX_N = 2500;
const int MAX_STATES = 40;
const int MAX_ITERS_RATIO = 5;
int MAX_ITERS;

const int EXTRA_STATES = 2;
const int TOTAL_STATES = MAX_STATES + EXTRA_STATES;

const std::string ARROW = "->";
const std::string WILDCARD = "?";
const std::string EMPTY = "X";
const std::string FIRE = "Fire";
const std::string WAIT = "Wait";
const std::string INIT = "Init";

const int wildcard = -1;
int emptyState, waitState, initState, fireState;
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
    std::string arrow, orderStr, leftStr, midStr, rightStr, endStr, tailStr;
    std::vector<int> lefts, mids, rights, ends;
    int end;

    emptyState = toState(EMPTY);
    fireState = toState(FIRE);
    waitState = toState(WAIT);
    initState = toState(INIT);

    setTrans(waitState, waitState, waitState, waitState);
    setTrans(emptyState, waitState, waitState, waitState);
    setTrans(waitState, waitState, emptyState, waitState);
    setTrans(emptyState, waitState, emptyState, waitState);

    while (!in.eof())
    {
        std::string line;
        std::getline(in, line);

        std::stringstream lineStream(line);

        std::string noCommentLine;
        std::getline(lineStream, noCommentLine, '#');

        std::stringstream noCommentLineStream(noCommentLine);

        leftStr = midStr = rightStr = arrow = endStr = "";
        noCommentLineStream >> leftStr >> midStr >> rightStr >> arrow >> endStr >> tailStr;

        if (leftStr == "") continue;

        myAssert(leftStr != "" && midStr != "" && rightStr != "" && arrow == ARROW && endStr != "" && tailStr == "", "Invalid transition rule: " + line);

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
            myAssert(left != fireState, "Invalid left state: " + toString(left));
        }
        for (int mid : mids)
        {
            myAssert(mid != emptyState && mid != fireState, "Invalid mid state: " + toString(mid));
        }
        for (int right : rights)
        {
            myAssert(right != fireState, "Invalid right state: " + toString(right));
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

void readN(std::istream& in)
{
    in >> n;
    myAssert(1 <= n && n <= MAX_N, "Invalid N: " + std::to_string(n));

    MAX_ITERS = MAX_ITERS_RATIO * n;
}

int state[MAX_N + 2];
int newState[MAX_N + 2];

void printStates()
{
    int maxW = 0;
    for (int i = 0; i < freeState; i++)
    {
        maxW = std::max(maxW, (int) toString(i).size());
    }

    for (int i = 1; i <= n; ++i)
    {
        std::cerr << std::left << std::setw(maxW) << toString(state[i]);
        if (i < n) std::cerr << " ";
    }
    std::cerr << std::endl;
}

#define RES_NONE 0
#define RES_NOSYNC 1
#define RES_SYNC 2

std::pair<int, int> runRobots(bool debug)
{
    state[0] = state[n + 1] = emptyState;
    for (int i = 1; i <= n; ++i)
    {
        state[i] = i == 1 ? initState : waitState;
    }

    if (debug) std::cerr << std::endl;

    int iters = 0;
    bool noChange = false;
    int result = RES_NONE;

    while (iters < MAX_ITERS && !noChange && result == RES_NONE)
    {
        if (debug) printStates();

        for (int i = 1; i <= n; ++i)
        {
            if (!isSet[state[i - 1]][state[i]][state[i + 1]])
            {
                fail("No transition from: " + toString(state[i - 1]) + " " + toString(state[i]) + " " + toString(state[i + 1]));
            }
            newState[i] = trans[state[i - 1]][state[i]][state[i + 1]];
        }

        noChange = true;

        bool anyoneFire = false;
        bool anyoneNotFire = false;

        for (int i = 1; i <= n; ++i)
        {
            if (newState[i] != state[i]) noChange = false;
            if (newState[i] == fireState) anyoneFire = true;
            else anyoneNotFire = true;
            state[i] = newState[i];
        }

        if (anyoneFire)
        {
            if (anyoneNotFire) result = RES_NOSYNC;
            else result = RES_SYNC;
        }

        ++iters;
    }

    if (debug) printStates();

    if (result == RES_NONE && noChange) iters = MAX_ITERS;

    return {result, iters};
}

const int TARGET_STATES = 7;
const double POW = 0.6;

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    myAssert(in.is_open(), "In-file " + std::string(argv[1]) + " not found.");
    myAssert(out.is_open(), "Out-file " + std::string(argv[3]) + " not found.");

    bool partialScoring;
    in >> partialScoring;

    readN(in);
    readMachine(out);

    int numStates = freeState - EXTRA_STATES;

    std::cerr << "Num states: " << numStates << std::endl;

    std::pair<int, int> resIters = runRobots(false);

    myAssert(resIters.first != RES_NONE, "No robots fired");
    myAssert(resIters.first != RES_NOSYNC, "Only some robot(s) fired");

    std::cerr << "All robots fired" << std::endl;

    double score = partialScoring ? pow((double) TARGET_STATES / numStates, POW) : 1;

    std::cout << std::fixed << std::setprecision(6) << score << std::endl;

    return 0;
}
