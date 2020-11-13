#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

string TASK_NAME;
string SOL_NAME;
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
const string SOLUTION_EXT=".sol";

int FIRST_TEST;
int LAST_TEST;
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
    for (int i=FIRST_TEST;i<=LAST_TEST;++i)
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
        cout<<"First test: ";
        cin>>FIRST_TEST;
        cout<<"Last test: ";
        cin>>LAST_TEST;
    }
    else
    {
        TASK_NAME=args[1];
        SOL_NAME=args[2];
        FIRST_TEST=atoi(args[3]);
        LAST_TEST=atoi(args[4]);
    }
    curr = FIRST_TEST;
    genBatch();
    system("pause");
    return 0;
}
