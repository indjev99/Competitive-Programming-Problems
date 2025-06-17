#include "subway.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

static const std::string PASSWORD_TRALALEROTRALALA69 = "PASSWORD_26354";

static int n, m, d;

static std::vector<int> a;
static std::vector<int> b;

static std::vector<bool> company_used;
static std::vector<int> assigned_company;

static std::vector<bool> cnt_in, cnt_out;

static bool no_answers = true;

static void myAssert(bool cond, const std::string& msg)
{
    if (cond) return;
    std::cout << -1 <<std::endl;
    std::cout << msg << std::endl;
    exit(0);
}

static void input()
{
    //std::cout << "N:" << std::endl;
    std::cin >> n;
    //std::cout << "M:" << std::endl;
    std::cin >> m;
    d = m/n;
    a.resize(m);
    b.resize(m);
    company_used.resize(d);
    assigned_company.resize(m);

    for(int i=0;i<d;i++)company_used[i]=false;
    for(int i=0;i<m;i++)assigned_company[i]=-1;

    //std::cout << "Edges:" << std::endl;
    for (int i = 0; i < m; i++)
    {
        std::cin>>a[i]>>b[i];
    }
}
void answer(int company_id, std::vector<int> roads)
{
    no_answers = false;
    for(auto road_id: roads) myAssert(road_id >= 0 && road_id < m, "Invalid tunnel id");
    myAssert(company_id >= 0 && company_id < d, "Invalid company id");
    myAssert(!company_used[company_id], "Answered for the same company twice.");
    company_used[company_id] = true;
    for(auto road_id: roads)
    {
        myAssert(assigned_company[road_id] == -1, "Assigned the same tunnel to a company twice");
        assigned_company[road_id] = company_id;
    }
}

int main()
{
    input();
    bool ans = assign_roads(n, m, a, b);
    if(ans==false)
    {
        std::cout<<PASSWORD_TRALALEROTRALALA69<<std::endl;
        std::cout<<0<<std::endl;
        return 0;
    }
    else
    {
        std::cout<<PASSWORD_TRALALEROTRALALA69<<std::endl;
        std::cout<<1<<std::endl;
    }
    for(int i=0;i<m;i++)
    {
        myAssert(assigned_company[i] != -1, "Did not assign all tunnels to companies");
    }
    cnt_in.resize(m);
    cnt_out.resize(m);
    for(int i=0;i<m;i++)
    {
        myAssert(cnt_out[a[i]*d + assigned_company[i]] == 0, "Invalid assignment");
        myAssert(cnt_in[b[i]*d + assigned_company[i]] == 0, "Invalid assignment");
        cnt_out[a[i]*d + assigned_company[i]] = 1;
        cnt_in[b[i]*d + assigned_company[i]] = 1;
    }
    for(int i=0;i<m;i++)
    {
        myAssert(cnt_in[i] == 1, "Invalid assignment");
        myAssert(cnt_out[i] == 1, "Invalid assignment");
    }

    std::cout<<1<<std::endl;

    return 0;
}
