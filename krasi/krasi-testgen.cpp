#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="krasi";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=100000;
const int MAX_K=100;
const int MAX_NUM=1000;
int n,k;
int arr[MAX_N];

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<n<<' '<<k<<'\n';
    for (int i=0;i<n;++i)
    {
        if (i) outF<<' ';
        outF<<arr[i];
    }
    outF<<'\n';
}
void genTest(ofstream &outF, int type)
{
    int pNeg;
    if (type==0) pNeg=60;
    if (type==1) pNeg=55;
    if (type==2) pNeg=50;
    if (type==3) pNeg=40;
    if (type==4) pNeg=25;
    n=MAX_N;
    k=MAX_K;
    for (int i=0;i<n;++i)
    {
        if (randomNumber()%75==0) arr[i]=0;
        else arr[i]=randomNumber()%MAX_NUM+1;
        if (randomNumber()%100<pNeg) arr[i]*=-1;
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
