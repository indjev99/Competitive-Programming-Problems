#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="chess";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=1000;
const long long MAX_K=1000000000;
int n,x1,x2,y1,y2;
long long k;
bool taken[MAX_N][MAX_N];

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_K;
    r=(r*(RAND_MAX+1)+rand())%MAX_K;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<n<<' '<<k<<' '<<x1+1<<' '<<y1+1<<' '<<x2+1<<' '<<y2+1<<'\n';
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            if (j) outF<<' ';
            outF<<taken[i][j];
        }
        outF<<'\n';
    }
}
void genTest(ofstream &outF,int prob)
{
    n=randomNumber()%(MAX_N-100)+101;
    x1=randomNumber()%n;
    x2=randomNumber()%n;
    y1=randomNumber()%n;
    y2=randomNumber()%n;
    if (randomNumber()%3==0) k=randomNumber()%(MAX_K-10000)+10001;
    else k=randomNumber()%(abs(x2-x1)+abs(y2-y1))+10;
    cerr<<abs(x2-x1)+abs(y2-y1)<<endl;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            if ((i==x1 && j==y1) || (i==x2 && j==y2)) taken[i][j]=0;
            else taken[i][j]=randomNumber()%100<prob;
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
        genTest(currOutF,67);
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
