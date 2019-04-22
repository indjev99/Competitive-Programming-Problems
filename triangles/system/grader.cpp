#include "triangles.h"
#include <iostream>
#include <stdio.h>
#include <locale>

static const int BUF_SIZE=1<<16;
static char buf[BUF_SIZE];
static int bufOut;
static int bufCnt;

static bool scan(char& c)
{
    if (bufOut==bufCnt)
    {
        bufOut=0;
        bufCnt=fread(buf,1,BUF_SIZE,stdin);
    }
    if (bufOut==bufCnt) return false;
    c=buf[bufOut++];
    return true;
}
static bool scan(int& a)
{
    char c;
    bool succ=true;
    do
    {
        succ=scan(c);
    }
    while (succ && isspace(c));
    if (!succ) return false;
    a=0;
    do
    {
        a=a*10+c-'0';
        succ=scan(c);
    }
    while (succ && !isspace(c));
    return true;
}

static const int MAX_N=34e4;
static int n;
static int lens[MAX_N];

static void input()
{
    scan(n);
    for (int i=0;i<n;++i)
    {
        scan(lens[i]);
    }
}
int main()
{
    input();
    std::cout<<triangles(lens,n)<<std::endl;
    return 0;
}
