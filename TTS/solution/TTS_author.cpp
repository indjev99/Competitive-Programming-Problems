#include<stdio.h>
#include<vector>
#include<math.h>
#include<algorithm>
using namespace std;
const int MAX_N=60100;
const int MAX_M=1100;
const double EPS=0.000001;
struct point
{
    double x,y;
};
bool operator<(point a, point b)
{
    if (a.x<b.x) return 1;
    if (a.x>b.x) return 0;
    return a.y<b.y;
}
int n,m;
int ch_size=0;
double answer=0;
point h[MAX_N];
point tts[MAX_M];
point convex_hull[MAX_N];
void input()
{
    scanf("%d%d",&n,&m);
    for (int i=0;i<n;++i)
    {
        scanf("%lf%lf",&h[i].x,&h[i].y);
    }
    for (int i=0;i<m;++i)
    {
        scanf("%lf%lf",&tts[i].x,&tts[i].y);
    }
}
double crossProduct(point o, point a, point b)
{
    return (a.x-o.x)*(b.y-o.y)-(a.y-o.y)*(b.x-o.x);
}
void findConvexHull()
{
    sort(h,h+n);
    ch_size=0;
	for (int i=0;i<n;++i)
    {
		while (ch_size>=2 && crossProduct(convex_hull[ch_size-2],convex_hull[ch_size-1],h[i])<=0) ch_size--;
		convex_hull[ch_size]=h[i];
		++ch_size;
	}
	int upper_hull_size=ch_size;
	for (int i=n-2; i>=0;--i)
    {
		while (ch_size>=upper_hull_size+1 && crossProduct(convex_hull[ch_size-2],convex_hull[ch_size-1],h[i])<=0) ch_size--;
		convex_hull[ch_size]=h[i];
		++ch_size;
	}
	--ch_size;
}
long double wholeArea;
long double findArea(point a, point b, point c)
{
    return ((a.x-b.x)*(a.y+b.y)+(b.x-c.x)*(b.y+c.y)+(c.x-a.x)*(c.y+a.y))/2;
}
long double findWholeArea(point a)
{
    long double area=0;
    for (int i=0;i<ch_size;++i)
    {
        area+=fabs(findArea(a,convex_hull[i],convex_hull[(i+1)%ch_size]));
    }
    return area;
}
bool isInside(point a)
{
    if (findWholeArea(a)>wholeArea+EPS || wholeArea<0+EPS) return 0;
    return 1;
}
double findDist(point a, point b, point c)
{
    if (fabs(b.x-c.x)>EPS || fabs(b.y-c.y)>EPS) return fabs((c.y-b.y)*a.x-(c.x-b.x)*a.y+c.x*b.y-c.y*b.x)/sqrt((c.y-b.y)*(c.y-b.y)+(c.x-b.x)*(c.x-b.x));
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double compute(point a)
{
    if (!isInside(a)) return 0;
    double ans,curr_ans;
    bool first=1;
    for (int i=0;i<ch_size;++i)
    {
        curr_ans=findDist(a,convex_hull[i],convex_hull[(i+1)%ch_size]);
        if (first || curr_ans<ans) ans=curr_ans;
        first=0;
    }
    return ans;
}
void solve()
{
    answer=0;
    if (n<3)
    {
        return;
    }
    findConvexHull();
    /*printf("\n");
    for (int i=0;i<ch_size;++i)
    {
        printf("%.2f %.2f\n",convex_hull[i].x,convex_hull[i].y);
    }*/
    wholeArea=findWholeArea(convex_hull[0]);
    for (int i=0;i<m;++i)
    {
        answer+=compute(tts[i]);
    }
}
void output()
{
    printf("%.6f\n",answer);
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
