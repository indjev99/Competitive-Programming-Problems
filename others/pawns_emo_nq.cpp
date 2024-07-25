#include <iostream>
#include <vector>

const int MAX_N = 1e6;

int n, q;
bool isExit[MAX_N];
std::vector<int> adj[MAX_N];

void input()
{
    std::cin >> n >> q;

    for (int i = 0; i < n; ++i)
    {
        std::cin >> isExit[i];
    }

    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }
}

int ans;
int u, v;

bool onPath[MAX_N];
int nextOnPath[MAX_N];

bool DFSPath(int c, int par = -1)
{
    if (c == v)
    {
        onPath[c] = true;
        nextOnPath[c] = -1;
        return true;
    }

    for (int o : adj[c])
    {
        if (o == par) continue;

        if (DFSPath(o, c))
        {
            onPath[c] = true;
            nextOnPath[c] = o;
            return true;
        }
    }

    return false;
}

int DFSFreeDist(int c, int par = -1)
{
    if (isExit[c]) return 0;

    int exitDist = 10 * n;

    for (int o : adj[c])
    {
        if (o == par) continue;

        int d = DFSFreeDist(o, c);

        exitDist = std::min(exitDist, d + 1);
    }

    return exitDist;
}

int DFSSideDist(int c, int par = -1)
{
    // std::cerr << "  " << c << " " << par << std::endl;

    if (isExit[c]) return 0;

    int exitDist = 10 * n;

    for (int o : adj[c])
    {
        if (o == par) continue;
        if (onPath[o]) continue;

        int d = DFSSideDist(o, c);

        exitDist = std::min(exitDist, d + 1);
    }

    return exitDist;
}

void solve()
{
    // std::cerr << "Solving: " << u + 1 << " " << v + 1 << std::endl;

    int uFreeDist = DFSFreeDist(u);
    int vFreeDist = DFSFreeDist(v);

    // std::cerr << "Free dists: " << uFreeDist << " " << vFreeDist << std::endl;

    std::fill(onPath, onPath + n, false);
    std::fill(nextOnPath, nextOnPath + n, -2);
    DFSPath(u);

    std::vector<int> sideDists;

    // std::cerr << "Path:";
    // for (int c = u; c != -1; c = nextOnPath[c])
    // {
    //     std::cerr << " " << c + 1;
    // }
    // std::cerr << std::endl;

    for (int c = u; c != -1; c = nextOnPath[c])
    {
        sideDists.push_back(DFSSideDist(c));
    }

    // std::cerr << "Side dists:";
    // for (int sd : sideDists)
    // {
    //     std::cerr << " " << sd;
    // }
    // std::cerr << std::endl;

    ans = 10 * n;

    for (int i = 0; i < (int) sideDists.size(); ++i)
    {
        int cand = i + std::max(sideDists[i], vFreeDist);
        ans = std::min(ans, cand);

        int cand2 = i + std::max(sideDists[sideDists.size() - i - 1], uFreeDist);
        ans = std::min(ans, cand2);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();

    for (int i = 0; i < q; ++i)
    {
        std::cin >> u >> v;
        --u;
        --v;

        solve();
        std::cout << ans << "\n";
    }

    return 0;
}
