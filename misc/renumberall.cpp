#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

string NAME;
int FIRST_TEST;
int LAST_TEST;
int NEW_NUM;

const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
const string SOLUTION_EXT=".sol";

int currOld;
int currNew;
string getOldInputName()
{
    string name;
    name=NAME;
    name+='.';
    name+='0'+currOld/10;
    name+='0'+currOld%10;
    ++currOld;
    name+=INPUT_EXT;
    return name;
}
string getNewInputName()
{
    string name;
    name=NAME;
    name+='.';
    name+='0'+currNew/10;
    name+='0'+currNew%10;
    ++currNew;
    name+=INPUT_EXT;
    return name;
}
bool fileExists(const string& fname)
{
    ifstream f(fname.c_str());
    return f.good();
}
void genBatch()
{
    string oldInName;
    string newInName;
    string cmd;
    currOld = FIRST_TEST;
    currNew = NEW_NUM;
    for (int i=0;i<LAST_TEST-FIRST_TEST+1;++i)
    {
        oldInName=getOldInputName();
        if (!fileExists(oldInName)) continue;
        newInName=getNewInputName();

        cmd="ren "+oldInName+" "+newInName;
        cerr<<cmd<<endl;
        system(cmd.c_str());
    }

}
int main(int argc, char* args[])
{
    if (argc<5)
    {
        cout<<"Name: ";
        cin>>NAME;
        cout<<"First test: ";
        cin>>FIRST_TEST;
        cout<<"Last test: ";
        cin>>LAST_TEST;
        cout<<"New first test: ";
        cin>>NEW_NUM;
    }
    else
    {
        NAME=args[1];
        FIRST_TEST=atoi(args[2]);
        LAST_TEST=atoi(args[3]);
        NEW_NUM=atoi(args[4]);
    }
    genBatch();
    system("pause");
    return 0;
}
