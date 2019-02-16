#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="ant";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const int MAX_ABC=10000;
int a,b,c;

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_ABC;
    r=(r*(RAND_MAX+1)+rand())%MAX_ABC;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<a<<' '<<b<<' '<<c<<'\n';
}
void genTest(ofstream &outF)
{
    a=randomNumber()%MAX_ABC+1;
    b=randomNumber()%MAX_ABC+1;
    c=randomNumber()%MAX_ABC+1;
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
