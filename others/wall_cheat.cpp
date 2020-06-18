#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct Point
{
    int x;
    int y;
    int type; // 1 - wall edge, 2 - exit
    int num;
    int source;
};
bool operator<(const Point& a, const Point& b)
{
    return a.y<b.y;
}
struct WallEnd
{
    int sourceX;
    int v;
};
map<pair<int,int>,WallEnd> wes;
map<int,int> paths;
vector<Point> ps;
int y;
int n,m;
vector<int> ans;

void input()
{
    int x;
    cin>>x;
    paths[x]=0;
    y=1;
    cin>>n>>m;
    ans.resize(m);
    int sx,fx,y,v;
    for (int i=0;i<n;++i)
    {
        cin>>sx>>fx>>y>>v;
        wes[{y,fx}]={sx,v-1};
        ps.push_back({sx-1,y,1,i});
        ps.push_back({fx+1,y,1,i});
    }
    for (int i=0;i<m;++i)
    {
        cin>>x>>y;
        ps.push_back({x,y,2,i});
    }
    sort(ps.begin(),ps.end());
}
void output()
{
    for (int i=0;i<m;++i)
    {
        cout<<ans[i]<<'\n';
    }
}
int dist(int x)
{
    int d=INT_MAX;
    auto n=paths.upper_bound(x);
    if (n!=paths.end())
    {
        d=min(d,n->second+n->first-x);
    }
    if (n!=paths.begin())
    {
        auto p=prev(n);
        d=min(d,p->second+x-p->first);
    }
    return d;
}
auto wt=wes.begin();
int mp=0;
void update(int h)
{
    auto pt=paths.begin();
    while (pt!=paths.end() && wt!=wes.end() && wt->first.first==h)
    {
        if (wt->first.second<pt->first) ++wt;
        else
        {
            if (wt->second.sourceX<=pt->first) pt->second+=wt->second.v;
            ++pt;
        }
    }
    for (auto pt=paths.begin();pt!=paths.end();++pt)
    {
        auto pt2=next(pt);
        while (pt2!=paths.end() && pt2->second-pt2->first>=pt->second-pt->first) pt2=paths.erase(pt2);
        if (pt!=paths.begin())
        {
            pt2=prev(pt);
            while (pt2->second+pt2->first>=pt->second+pt->first)
            {
                if (pt2==paths.begin())
                {
                    paths.erase(pt2);
                    break;
                }
                else pt2=prev(paths.erase(pt2));
            }
        }
    }
}
void solve()
{
    wt=wes.begin();
    int toUpdate=0;
    int d;
    for (int i=0;i<ps.size();++i)
    {
        if (toUpdate && toUpdate<ps[i].y)
        {
            update(toUpdate);
            toUpdate=0;
        }
        d=dist(ps[i].x);
        if (ps[i].type==1)
        {
            toUpdate=ps[i].y;
            paths[ps[i].x]=d;
        }
        else
        {
            ans[ps[i].num]=d+ps[i].y-1;
        }
    }
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
