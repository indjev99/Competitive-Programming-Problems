#include<iostream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="yen";
const string INPUT_EXT=".in";

int SEED;
int TESTS_IN_BATCH;

const long long MAX_NUM=1000;
long long a,b,c,n;

long long randomNumber()
{
    long long r=rand();
    r=(r*(RAND_MAX+1)+rand())%MAX_NUM;
    r=(r*(RAND_MAX+1)+rand())%MAX_NUM;
    r=(r*(RAND_MAX+1)+rand())%MAX_NUM;
    r=(r*(RAND_MAX+1)+rand());
    return r;
}
void printTest(ofstream &outF)
{
    outF<<a<<' '<<b<<' '<<c<<' '<<n<<'\n';
}
void genTest(ofstream &outF)
{
    a=randomNumber()%MAX_NUM+1;
    b=randomNumber()%MAX_NUM+1;
    c=randomNumber()%MAX_NUM+1;
    n=randomNumber()%MAX_NUM+1;
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
