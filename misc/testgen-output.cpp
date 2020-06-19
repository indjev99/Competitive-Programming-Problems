#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

string TASK_NAME;
string SOL_NAME;
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
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
string getBaseOutputName()
{
    string name;
    name=TASK_NAME;
    name+=OUTPUT_EXT;
    return name;
}
void genBatch()
{
    string name;
    string inName;
    string outName;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        inName=getInputName();
        outName=getSolutionName();
        string cmd=SOL_NAME+".exe"+" < "+inName+" > "+outName;
        cerr<<cmd<<endl;
        system(cmd.c_str());
    }

}
int main(int argc, char* args[])
{
    if (argc<4)
    {
        cout<<"Task name: ";
        cin>>TASK_NAME;
        cout<<"Solution name: ";
        cin>>SOL_NAME;
        cout<<"Tests: ";
        cin>>TESTS_IN_BATCH;
    }
    else
    {
        TASK_NAME=args[1];
        SOL_NAME=args[2];
        TESTS_IN_BATCH=atoi(args[3]);
    }
    genBatch();
    system("pause");
    return 0;
}
