#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="sequences";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_Q=40000;
const long long MAX_BNS=1e9;
const long long MAX_C=1e12;
int q;
long long b1[MAX_Q],c1[MAX_Q],s1[MAX_Q],b2[MAX_Q],c2[MAX_Q],s2[MAX_Q];
long long n[MAX_Q];

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_C;
    r=(r*(RAND_MAX+1)+rand())%MAX_C;
    r=(r*(RAND_MAX+1)+rand())%MAX_C;
    r=(r*(RAND_MAX+1)+rand())%MAX_C;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<q<<'\n';
    for (int i=0;i<q;++i)
    {
        outF<<b1[i]<<' '<<c1[i]<<' '<<s1[i]<<' '<<b2[i]<<' '<<c2[i]<<' '<<s2[i]<<' '<<n[i]<<'\n';
    }
}
void genTest(ofstream &outF)
{
    q=MAX_Q;
    for (int i=0;i<q;++i)
    {
        b1[i]=randomNumber()%MAX_BNS;
        c1[i]=randomNumber()%(2*MAX_C)-MAX_C;
        s1[i]=randomNumber()%MAX_BNS;
        b2[i]=randomNumber()%MAX_BNS;
        c2[i]=randomNumber()%(2*MAX_C)-MAX_C;
        s2[i]=randomNumber()%MAX_BNS;
        n[i]=randomNumber()%MAX_BNS+1;
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
