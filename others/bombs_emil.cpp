#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

struct Object
{
    bool isBox;
    int x, y;
};
bool operator<(const Object& a, const Object& b)
{
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

vector<Object> objects;

void input()
{
    int n, x, y;
    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        cin >> x >> y;
        objects.push_back({true, x, y});
    }
    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        cin >> x >> y;
        objects.push_back({false, x, y});
    }
}

struct Box
{
    int x, y;
    int left, top, right, bot;
};
vector<Box> boxes;

unordered_map<int, int> lastInCol;
void buildGraph()
{
    sort(objects.begin(), objects.end());
    int last = 0;
    int row = -1;
    for (const Object& o : objects)
    {
        if (o.y > row)
        {
            last = 0;
            row = o.y;
        }
        if (!o.isBox)
        {
            last = 0;
            lastInCol[o.x] = 0;
            continue;
        }
        int lastT = lastInCol[o.x];
        if (last != 0) boxes[last - 1].right = boxes.size();
        if (lastT != 0) boxes[lastT - 1].bot = boxes.size();
        boxes.push_back({o.x, o.y, last - 1, lastT - 1, -1, -1});
        last = boxes.size();
        lastInCol[o.x] = boxes.size();
    }
}

struct Out
{
    bool h;
    int x, y;
};

std::vector<Out> toOut;
std::vector<Out> toOutRev;

void blow(int a, int b)
{
    int x = (boxes[a].x + boxes[b].x) / 2;
    int y = (boxes[a].y + boxes[b].y) / 2;
    toOut.push_back({boxes[a].x == boxes[b].x, x, y});
}
void blow(int a)
{
    int x = boxes[a].x + 1;
    int y = boxes[a].y;
    toOut.push_back({false, x, y});
}
void postBlow(int a, int b)
{
    int x = (boxes[a].x + boxes[b].x) / 2;
    int y = (boxes[a].y + boxes[b].y) / 2;
    toOutRev.push_back({boxes[a].x == boxes[b].x, x, y});
}

#define L 1
#define T 2
#define R 4
#define B 8

vector<bool> vis;

bool solveT(int a, int par)
{
    vis[a] = true;
    Box& box = boxes[a];
    int children = 0;
    if (box.left != -1 && !vis[box.left])
        if (!solveT(box.left, R)) children += L;
    if (box.top != -1 && !vis[box.top])
        if (!solveT(box.top, B)) children += T;
    if (box.right != -1 && !vis[box.right])
        if (!solveT(box.right, L)) children += R;
    if (box.bot != -1 && !vis[box.bot])
        if (!solveT(box.bot, T)) children += B;

    switch (children)
    {
    case 0:
        return false;
    case L:
        blow(a, box.left);
        return true;
    case T:
        blow(a, box.top);
        return true;
    case R:
        blow(a, box.right);
        return true;
    case B:
        blow(a, box.bot);
        return true;
    case L+T+R:
        blow(a, box.top);
        blow(box.left, box.right);
        return true;
    case L+B+R:
        blow(a, box.bot);
        blow(box.left, box.right);
        return true;
    case B+L+T:
        blow(a, box.left);
        blow(box.top, box.bot);
        return true;
    case B+R+T:
        blow(a, box.right);
        blow(box.top, box.bot);
        return true;
    case L+R+B+T:
        blow(a, box.left);
        blow(box.top, box.bot);
        blow(box.right);
        return true;
    case L+T:
        if (par == B) blow(a, box.left);
        else blow(a, box.top);
        return false;
    case R+T:
        if (par == B) blow(a, box.right);
        else blow(a, box.top);
        return false;
    case L+B:
        if (par == T) blow(a, box.left);
        else blow(a, box.bot);
        return false;
    case R+B:
        if (par == T) blow(a, box.right);
        else blow(a, box.bot);
        return false;
    case L+R:
        postBlow(box.left, box.right);
        return false;
    case T+B:
        postBlow(box.top, box.bot);
        return false;
    }
}

void solve()
{
    buildGraph();
    vis.resize(boxes.size());
    fill(vis.begin(), vis.end(), false);
    for (int i = 0; i < boxes.size(); ++i)
    {
        if (vis[i]) continue;
        bool succ = solveT(i, 0);
        if (!succ) blow(i);
    }
}

void output()
{
    cout << toOut.size() + toOutRev.size() << "\n";
    for (const Out& out : toOut)
    {
        cout << out.h << " " << out.x << " " << out.y << "\n";
    }
    reverse(toOutRev.begin(), toOutRev.end());
    for (const Out& out : toOutRev)
    {
        cout << out.h << " " << out.x << " " << out.y << "\n";
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
