#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>

const int MAX_COEFF = 10;

struct Shot
{
    int time;
    int pos;

    Shot(int time, int pos):
        time(time),
        pos(pos) {}

    Shot& operator=(const Shot& b)
    {
        time = b.time;
        pos = b.pos;
        return *this;
    }
};

int d, k;
std::vector<Shot> ans;

void input()
{
    std::cin >> d >> k;
}

void output()
{
    std::sort(ans.begin(), ans.end());
    std::cout << ans.size() << "\n";
    for (const Shot& shot : ans)
    {
        std::cout << shot.time << " " << shot.pos << "\n";
    }
}

bool operator<(const Shot& a, const Shot& b)
{
    return a.time < b.time;
}

bool operator==(const Shot& a, const Shot& b)
{
    return a.time == b.time && a.pos == b.pos;
}

namespace std
{
    template<>
    struct hash<Shot>
    {
        std::size_t operator()(const Shot& shot) const
        {
            return std::hash<int>()(shot.time) ^ std::hash<int>()(shot.pos);
        }
    };
}

struct Sub
{
    int idx;
    int pos;
    int speed;
    bool alive;
    std::vector<Shot> shots;

    Sub(int idx, int pos, int speed):
        idx(idx),
        pos(pos),
        speed(speed),
        alive(true) {}

    int posAt(int time) const
    {
        int newPos = pos + speed * time;
        newPos = (newPos % d + d) % d;
        return newPos;
    }
};

struct ScoredShot
{
    Shot shot;
    int numSubs;

    ScoredShot(const Shot& shot, int numSubs):
        shot(shot),
        numSubs(numSubs) {}
};

bool operator<(const ScoredShot& a, const ScoredShot& b)
{
    if (a.numSubs != b.numSubs) return a.numSubs < b.numSubs;
    if (a.shot.pos != b.shot.pos) return a.shot.pos > b.shot.pos;
    if (a.shot.time != b.shot.time) return a.shot.time > b.shot.time;
    return false;
}

std::vector<Sub> subs;
std::unordered_map<Shot, std::pair<int, std::unordered_set<int>>> clusters;

std::priority_queue<ScoredShot> priorityShots;
std::unordered_set<int> shotTimes;
std::vector<Shot> shots;

void generateSubs()
{
    subs.clear();
    k = std::min(k, d);
    for (int pos = 0; pos < d; ++pos)
    {
        for (int speed = 1; speed <= k; ++speed)
        {
            subs.emplace_back(subs.size(), pos, speed % d);
        }
        for (int speed = std::max(k + 1, d - k); speed < d; ++speed)
        {
            subs.emplace_back(subs.size(), pos, speed);
        }
    }
}

void findRelevantShots()
{
    clusters.clear();
    for (Sub& sub : subs)
    {
        for (int time = 0; time < d * MAX_COEFF; ++time)
        {
            Shot shot = Shot(time, sub.posAt(time));
            sub.shots.push_back(shot);
            clusters[shot].second.insert(sub.idx);
        }
    }
}

void initialize()
{
    priorityShots = std::priority_queue<ScoredShot>();
    for (auto& sub : subs)
    {
        sub.alive = true;
    }
    for (auto& cluster : clusters)
    {
        if (cluster.first.time > d * MAX_COEFF) continue; 
        cluster.second.first = cluster.second.second.size();
        priorityShots.emplace(cluster.first, cluster.second.first);
    }
    shotTimes.clear();
    shots.clear();
}

void getMainSolution()
{
    while (!priorityShots.empty())
    {
        ScoredShot scoredShot = priorityShots.top();
        priorityShots.pop();
        if (scoredShot.numSubs == 0) break;
        Shot shot = scoredShot.shot;

        assert(shot.time <= d * MAX_COEFF);
        if (shotTimes.find(shot.time) != shotTimes.end()) continue;
        auto& currCluster = clusters[shot];
        if (scoredShot.numSubs != currCluster.first) continue;

        shotTimes.insert(shot.time);
        shots.push_back(shot);

        //std::cerr << shot.time << " " << shot.pos << " " << currCluster.first << std::endl;

        std::unordered_map<Shot, int> newPriorityShots;
        for (int idx : currCluster.second)
        {
            Sub& sub = subs[idx];
            if (!sub.alive) continue;
            sub.alive = false;
            for (const Shot& remShot : sub.shots)
            {
                if (remShot == shot) continue;
                if (remShot.time > d * MAX_COEFF) continue;
                if (shotTimes.find(remShot.time) != shotTimes.end()) continue;
                auto& thatCluster = clusters[remShot];
                --thatCluster.first;
                newPriorityShots[remShot] = thatCluster.first;
            }
        }
        currCluster.first = 0;

        for (const auto& newPriorityShot : newPriorityShots)
        {
            priorityShots.emplace(newPriorityShot.first, newPriorityShot.second);
        }
    }
}

void trySolve()
{
    generateSubs();
    findRelevantShots();
    initialize();
    getMainSolution();
    if (ans.empty() || ans.size() > shots.size())
    {
        ans = shots;
    }
    
}

void solve()
{
    int origK = k;

    if (d % 2 == 0 && origK < d / 2)
    {
        k = d / 2 - 1;
        trySolve();
        if (ans.size() <= d) return;
    }

    if (d % 3 == 0 && origK < d / 3)
    {
        k = d / 3 - 1;
        trySolve();
        if (ans.size() <= d) return;
    }

    if (origK <= d)
    {
        k = d;
        trySolve();
        if (ans.size() <= d) return;
    }

    k = origK;
    trySolve();
}

int main()
{
    input();
    solve();
    output();

    return 0;
}