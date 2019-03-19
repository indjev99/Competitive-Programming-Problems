#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

string TASK_NAME="transmission";
const string INPUT_EXT=".in";
const string SOLUTION_EXT=".sol";

int TESTS_IN_BATCH=5;
int curr=1;
string getInputName()
{
    string name;
    name=TASK_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=INPUT_EXT;
    return name;
}
string getSolutionName()
{
    string name;
    name=TASK_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=SOLUTION_EXT;
    ++curr;
    return name;
}
void genBatch()
{
    string inName;
    string outName;
    string cmd;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        cerr<<i+1<<":"<<endl;
        inName=getInputName();
        outName=getSolutionName();

        cmd="grader.exe < "+inName+" > out ";
        system(cmd.c_str());
        cmd="checker.exe "+inName+" one out > "+outName;
        system(cmd.c_str());
    }
}
int main()
{
    cout<<"Tests: ";
    cin>>TESTS_IN_BATCH;
    genBatch();
    system("pause");
    return 0;
}
