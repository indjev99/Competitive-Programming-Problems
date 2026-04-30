#include "infection.h"

#include <cassert>
#include <cmath>
#include <queue>
#include <set>

using ll = long long;

struct Vector {
    int id;
    ll startPos;
};

bool operator<(const Vector& a, const Vector& b) {
    if (a.startPos != b.startPos)
        return a.startPos < b.startPos;
    return a.id < b.id;
}

struct Line {
    std::set<Vector> sets[2][2]; // [infected][rightDir]
};

const int ET_DISAPPEAR = 0;
const int ET_APPEAR = 1;
const int ET_INFECTION = 2;

struct Event {
    ll time;

    int type;

    int id = -1;
    int id2 = -1;
};

bool operator<(const Event& a, const Event& b) {
    if (a.time != b.time)
        return a.time > b.time;
    return a.type > b.type;
}

struct Parity {
    std::priority_queue<Event> pq;

    std::vector<Line> lines;
};

struct Segment {
    int line;
    int start;
    int end;
};

struct Person {
    int id;

    bool infected;

    std::vector<Segment> segments;
    int currSegment = 0;

    ll infTime = -1;

    bool rightDir = false;
    ll startPos = -1;
    ll endPos = -1;
    ll endTime = -1;

    Parity* par = nullptr;

    void infect(ll time) {
        infected = true;
        infTime = time;
    }
};

int n;
int m;
std::vector<Person> people;

void checkInfection(ll minTime, std::set<Vector>::iterator it) {
    Person& p = people[it->id];
    std::priority_queue<Event>& pq = p.par->pq;
    Line& line = p.par->lines[p.segments[p.currSegment].line];

    auto& opp = line.sets[!p.infected][!p.rightDir];
    if (p.rightDir) {
        auto oppIt = opp.lower_bound({0, p.startPos + 2 * minTime});
        if (oppIt != opp.end()) {
            ll nextTime = (oppIt->startPos - p.startPos) / 2;
            if (nextTime < p.endTime && nextTime < people[oppIt->id].endTime) {
                pq.push({nextTime, ET_INFECTION, p.id, oppIt->id});
            }
        }
    } else {
        auto oppIt = opp.upper_bound({m, p.startPos - 2 * minTime});
        if (oppIt != opp.begin()) {
            oppIt--;
            ll nextTime = (p.startPos - oppIt->startPos) / 2;
            if (nextTime < p.endTime && nextTime < people[oppIt->id].endTime) {
                pq.push({nextTime, ET_INFECTION, p.id, oppIt->id});
            }
        }
    }
};

std::set<Vector>::iterator remove(ll time, std::set<Vector>::iterator it) {
    Person& p = people[it->id];
    Line& line = p.par->lines[p.segments[p.currSegment].line];

    auto& curr = line.sets[p.infected][p.rightDir];
    it = curr.erase(it);

    auto it2 = it;

    if (p.rightDir) {
        if (it != curr.begin()) {
            it--;
            checkInfection(time, it);
        }
    } else {
        if (it != curr.end()) {
            checkInfection(time, it);
        }
    }

    return it2;
};

void add(ll time, int id);

std::set<Vector>::iterator infect(ll time, std::set<Vector>::iterator it) {
    Person& p = people[it->id];

    it = remove(time, it);

    p.infect(time);

    add(time, p.id);

    return it;
};

void add(ll time, int id) {
    Person& p = people[id];
    Line& line = p.par->lines[p.segments[p.currSegment].line];

    auto& other = line.sets[!p.infected][p.rightDir];
    auto otherIt = other.lower_bound({0, p.startPos});
    if (!p.infected) {
        if (otherIt != other.end() && otherIt->startPos == p.startPos)
            p.infect(time);
    } else {
        if (otherIt != other.end() && otherIt->startPos == p.startPos) {
            otherIt = infect(time, otherIt);
        }
    }

    auto& curr = line.sets[p.infected][p.rightDir];
    auto [it, isNew] = curr.insert({p.id, p.startPos});
    assert(isNew);

    checkInfection(time, it);
};

void solvePar(Parity& par) {
    std::priority_queue<Event>& pq = par.pq;

    while (!pq.empty()) {
        Event e = pq.top();
        pq.pop();

        if (e.type == ET_APPEAR) {
            Person& p = people[e.id];

            Segment seg = p.segments[p.currSegment];

            p.startPos = seg.start;
            p.endPos = seg.end;

            p.rightDir = p.endPos >= p.startPos;

            if (p.rightDir)
                p.startPos -= e.time;
            else
                p.startPos += e.time;

            p.endTime = std::abs(p.endPos - p.startPos) + 1;

            pq.push({p.endTime, ET_DISAPPEAR, p.id});

            add(e.time, p.id);
        } else if (e.type == ET_DISAPPEAR) {
            Person& p = people[e.id];
            Line& line = par.lines[p.segments[p.currSegment].line];

            auto& curr = line.sets[p.infected][p.rightDir];
            auto it = curr.find({p.id, p.startPos});

            assert(it != curr.end());

            remove(e.time, it);

            p.currSegment++;

            if (p.currSegment < (int)p.segments.size()) {
                Segment nextSeg = p.segments[p.currSegment];
                pq.push({e.time, ET_APPEAR, p.id});
            }
        } else if (e.type == ET_INFECTION) {
            Person& p1 = people[e.id];
            Person& p2 = people[e.id2];

            if (p1.infected == p2.infected)
                continue;

            assert(p1.segments[p1.currSegment].line == p2.segments[p2.currSegment].line);

            Line& line = par.lines[p1.segments[p1.currSegment].line];

            auto& curr1 = line.sets[p1.infected][p1.rightDir];
            auto& curr2 = line.sets[p2.infected][p2.rightDir];

            auto it1 = curr1.find({p1.id, p1.startPos});
            auto it2 = curr2.find({p2.id, p2.startPos});

            assert(it1 != curr1.end());
            assert(it2 != curr2.end());

            if (!p1.infected) {
                infect(e.time, it1);
            } else {
                infect(e.time, it2);
            }
        }
    }
}

Parity pars[2];

std::vector<std::vector<int>> adj;
std::vector<int> parent;
std::vector<int> depth;
std::vector<int> heavyChild;

int hldDfs(int v, int p) {
    parent[v] = p;
    if (p != -1)
        depth[v] = depth[p] + 1;
    else
        depth[v] = 0;

    int size = 1;
    int maxChildSize = 0;
    for (int c : adj[v]) {
        if (c == p)
            continue;

        int childSize = hldDfs(c, v);
        size += childSize;
        if (childSize > maxChildSize) {
            maxChildSize = childSize;
            heavyChild[v] = c;
        }
    }
    return size;
}

std::vector<int> lineHead;
std::vector<int> lineIdx;
std::vector<int> posOnLine;

void hldDecompose(int v, int li, int pos) {
    lineIdx[v] = li;
    posOnLine[v] = pos;
    if (heavyChild[v] != -1) {
        hldDecompose(heavyChild[v], li, pos + 1);
    }

    for (int c : adj[v]) {
        if (c == parent[v] || c == heavyChild[v])
            continue;

        lineHead.push_back(c);
        hldDecompose(c, lineHead.size() - 1, 0);
    }
}

std::vector<Segment> getSegments(int a, int b, ll startTime) {
    std::vector<Segment> pref, suff;
    while (lineIdx[a] != lineIdx[b]) {
        int lia = lineIdx[a];
        int lib = lineIdx[b];

        if (depth[lineHead[lia]] > depth[lineHead[lib]]) {
            pref.push_back({lia, posOnLine[a], 0});
            a = parent[lineHead[lia]];
        } else {
            suff.push_back({lib, 0, posOnLine[b]});
            b = parent[lineHead[lib]];
        }
    }
    pref.push_back({lineIdx[a], posOnLine[a], posOnLine[b]});
    pref.insert(pref.end(), suff.rbegin(), suff.rend());
    return pref;
}

std::vector<int> solve(std::vector<std::pair<int, int>> R, std::vector<long long> D, std::vector<int> S, std::vector<int> T, std::vector<bool> I) {
    n = R.size() + 1;
    m = D.size();

    adj.resize(n);
    for (auto [u, v] : R) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    parent.resize(n, -1);
    depth.resize(n, -1);
    heavyChild.resize(n, -1);
    lineIdx.resize(n, -1);
    posOnLine.resize(n, -1);

    hldDfs(0, -1);

    lineHead.push_back(0);
    hldDecompose(0, 0, 0);

    for (int parId : {0, 1}) {
        pars[parId].lines.resize(lineHead.size());
    }

    for (int i = 0; i < m; i++) {
        people.push_back({i, I[i], getSegments(S[i], T[i], D[i])});
        if (I[i])
            people.back().infTime = D[i];
        people.back().par = &pars[(depth[S[i]] + D[i]) % 2];
        people.back().par->pq.push({D[i], ET_APPEAR, i});
    }

    for (int parId : {0, 1}) {
        solvePar(pars[parId]);
    }

    std::vector<int> ans(m);
    for (Person& p : people) {
        ans[p.id] = 0;
        if (p.infected)
            ans[p.id] = p.endTime - p.infTime;
    }

    return ans;
}
