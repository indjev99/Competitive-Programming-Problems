#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

#define PI 3.1415926535897932384626433

using namespace std;

const string TASK_NAME="farm";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

struct Point
{
    int x;
    int y;
};

const int MAX_N=50;
const long long MAX_XY=10000;
int n,k;
Point p[MAX_N];

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_XY;
    r=(r*(RAND_MAX+1)+rand())%MAX_XY;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<n<<' '<<k<<'\n';
    for (int i=0;i<n;++i)
    {
        if (p[i].x<0 || p[i].y<0 || p[i].x>MAX_XY || p[i].y>MAX_XY) cerr<<"ERROR"<<endl;
        outF<<p[i].x<<'\t'<<p[i].y<<'\n';
    }
}
void genTest(ofstream &outF, int type)
{
    n=MAX_N;
    k=randomNumber()%(n-3)+3;
    if (type==0)
    {
        for (int i=0;i<n;++i)
        {
            p[i].x=randomNumber()%MAX_XY;
            p[i].y=randomNumber()%MAX_XY;
        }
    }
    else if (type==1)
    {
        for (int i=0;i<n;++i)
        {
            double angle=randomNumber()%MAX_XY*1.0/MAX_XY*2*PI;
            p[i].x=sin(angle)*MAX_XY/2.0+MAX_XY/2.0;
            p[i].y=cos(angle)*MAX_XY/2.0+MAX_XY/2.0;
        }
    }
    else
    {
        for (int i=0;i<6;++i)
        {
            double angle=randomNumber()%MAX_XY*1.0/MAX_XY*2*PI;
            p[i].x=-fabs(sin(angle))*1000+1000;
            p[i].y=cos(angle)*1000+1000;
        }
        for (int i=6;i<25;++i)
        {
            double angle=randomNumber()%MAX_XY*1.0/MAX_XY*2*PI;
            p[i].x=fabs(sin(angle))*1000+1000;
            p[i].y=cos(angle)*1000+1000;
        }

        for (int i=25;i<44;++i)
        {
            double angle=randomNumber()%MAX_XY*1.0/MAX_XY*2*PI;
            if (type<4) p[i].x=-fabs(sin(angle))*1000+5000;
            else p[i].x=-fabs(sin(angle))*1000+1500;
            p[i].y=cos(angle)*1000+1000;
        }
        for (int i=44;i<50;++i)
        {
            double angle=randomNumber()%MAX_XY*1.0/MAX_XY*2*PI;
            if (type<4) p[i].x=fabs(sin(angle))*1000+5000;
            else p[i].x=fabs(sin(angle))*1000+1500;
            p[i].y=cos(angle)*1000+1000;
        }
    }
    printTest(outF);
}
int curr=1;
string getName()
{
    string name;
    name=TASK_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=INPUT_EXT;
    ++curr;
    return name;
}
void genBatch()
{
    string name;
    ofstream currOutF;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        name=getName();
        cerr<<name<<endl;
        currOutF.open(name.c_str());
        genTest(currOutF,i%5);
        currOutF.close();
    }
}
int main()
{
    cin>>SEED;
    cin>>TESTS_IN_BATCH;
    srand(SEED);
    genBatch();
    return 0;
}
