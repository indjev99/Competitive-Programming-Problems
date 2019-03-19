#include<iostream>
#include<stdlib.h>
#include<fstream>

using namespace std;

string TASK_NAME="transmission";
const string INPUT_EXT=".in";
const string SOLUTION_EXT=".sol";

int TESTS_IN_BATCH=40;
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
    ifstream inFile;
    ifstream outFile;
    ofstream newInFile;
    int n,d,k,seed;
    double authorScore;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        cerr<<i+1<<":"<<endl;
        inName=getInputName();
        outName=getSolutionName();

        cmd="./grader < "+inName+" > out ";
        system(cmd.c_str());
        cmd="./checker "+inName+" one out > "+outName;
        system(cmd.c_str());

        inFile.open(inName.c_str());
        outFile.open(outName.c_str());
        newInFile.open(("./new/"+inName).c_str());
        inFile>>n>>d>>k>>seed;
        outFile>>authorScore;
        newInFile<<n<<' '<<d<<' '<<k<<' '<<seed<<' '<<authorScore<<'\n';
        inFile.close();
        outFile.close();
        newInFile.close();
    }
}
int main()
{
    genBatch();
    system("pause");
    return 0;
}
