#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

string OLD_NAME;
string NEW_NAME;
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
const string SOLUTION_EXT=".sol";

int TESTS_IN_BATCH=5;
int curr=1;
string getOldInputName()
{
    string name;
    name=OLD_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=INPUT_EXT;
    return name;
}
string getInputName()
{
    string name;
    name=NEW_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=INPUT_EXT;
    return name;
}
string getOldSolutionName()
{
    string name;
    name=OLD_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=SOLUTION_EXT;
    return name;
}
string getSolutionName()
{
    string name;
    name=NEW_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=SOLUTION_EXT;
    ++curr;
    return name;
}
void genBatch()
{
    string oldInName;
    string inName;
    string oldOutName;
    string outName;
    string cmd;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        oldInName=getOldInputName();
        inName=getInputName();

        cmd="ren "+oldInName+" "+inName;
        cerr<<cmd<<endl;
        system(cmd.c_str());

        oldOutName=getOldSolutionName();
        outName=getSolutionName();

        cmd="ren "+oldOutName+" "+outName;
        cerr<<cmd<<endl;
        system(cmd.c_str());
    }

}
int main(int argc, char* args[])
{
    if (argc<3)
    {
        cout<<"Old name: ";
        cin>>OLD_NAME;
        cout<<"New name: ";
        cin>>NEW_NAME;
        cout<<"Tests: ";
        cin>>TESTS_IN_BATCH;
    }
    else
    {
        OLD_NAME=args[1];
        NEW_NAME=args[2];
        TESTS_IN_BATCH=atoi(args[3]);
    }
    genBatch();
    system("pause");
    return 0;
}
