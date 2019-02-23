#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="crypto";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_N=1000;
string s;
int n;

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand())%MAX_N;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<s<<'\n';
}
void genTest(ofstream &outF)
{
    int p=randomNumber()%MAX_N+1;
    n=randomNumber()%MAX_N+1;
    if (n<p) n=p;
    s.resize(n);
    for (int i=0;i<p;++i)
    {
        s[i]='a'+randomNumber()%26;
    }
    for (int i=p;i<n;++i)
    {
        s[i]=s[i-p];
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
        genTest(currOutF);
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
