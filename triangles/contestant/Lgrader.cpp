#include "triangles.h"
#include <iostream>

static const int MAX_N=34e4;
static int n;
static int lens[MAX_N];

static void input()
{
    std::cin>>n;
    for (int i=0;i<n;++i)
    {
        std::cin>>lens[i];
    }
}
int main()
{
    input();
    std::cout<<triangles(lens,n)<<std::endl;
    return 0;
}
