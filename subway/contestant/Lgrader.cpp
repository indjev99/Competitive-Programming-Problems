#include "subway.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

static int n, m, d;

static std::vector<int> a;
static std::vector<int> b;

static std::vector<int> assigned_company;

static bool no_answers = true;

static void myAssert(bool cond, const std::string& msg)
{
    if (cond) return;
    std::cout << msg << std::endl;
    exit(0);
}

static void input()
{
    std::cout << "N:" << std::endl;
    std::cin >> n;
    std::cout << "M:" << std::endl;
    std::cin >> m;
    d = m/n;
    a.resize(m);
    b.resize(m);
    assigned_company.resize(m);

    for(int i=0;i<m;i++)assigned_company[i]=-1;

    std::cout << "Edges:" << std::endl;
    for (int i = 0; i < m; i++)
    {
        std::cin>>a[i]>>b[i];
    }
}
std::vector<std::pair<int, std::vector<int> > >answers;
void answer(int company_id, std::vector<int> roads)
{
    no_answers = false;
    for(auto road_id: roads) myAssert(road_id >= 0 && road_id < m, "Invalid tunnel id.");
    myAssert(company_id >= 0 && company_id < d, "Invalid tunnel id.");
    for(auto road_id: roads)
    {
        myAssert(assigned_company[road_id] == -1, "Assigned the same tunnel to a company twice.");
        assigned_company[road_id] = company_id;
    }
    answers.push_back({company_id, roads});
}

int main()
{
    input();
    bool ans = assign_roads(n, m, a, b);
    if(ans==false)
    {
        std::cout<<0<<std::endl;
        return 0;
    }
    else
    {
        std::cout<<1<<std::endl;
    }
    for(int i=0;i<m;i++)
    {
        myAssert(assigned_company[i] != -1, "Did not assign all tunnels to companies.");
    }

    for(int i=0;i<answers.size();i++)
    {
        std::cout<<"Ids of tunnels assigned to company "<<answers[i].first<<":\n";
        for(int j=0;j<answers[i].second.size();j++)
        {
            std::cout<<answers[i].second[j]<<" ";
        }
        std::cout<<std::endl;
    }

    return 0;
}
