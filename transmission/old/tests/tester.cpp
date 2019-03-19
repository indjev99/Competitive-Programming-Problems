#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<fstream>

using namespace std;

string TASK_NAME="transmission";
const string INPUT_EXT=".in";
const string SOLUTION_EXT=".sol";

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
double processTest()
{
    string inName;
    string solName;
    string cmd;
    ifstream temp;
    double score;
    inName=getInputName();
    solName=getSolutionName();
    cmd="grader.exe < "+inName+" > out ";
    system(cmd.c_str());
    cmd="checker.exe "+inName+" "+solName+" out > temp";
    system(cmd.c_str());
    temp.open("temp");
    temp>>score;
    cerr<<curr-1<<": "<<setprecision(15)<<score<<endl;
    temp.close();
    return score;
}
void genBatch()
{
    double s1=0,s2=0,s3=0,s4=0;
    int i=1;
    for (;i<=6;++i)
    {
        s1+=processTest()*2.5;
    }
    for (;i<=16;++i)
    {
        s2+=processTest()*2.5;
    }
    for (;i<=26;++i)
    {
        s3+=processTest()*2.5;
    }
    for (;i<=40;++i)
    {
        s4+=processTest()*2.5;
    }
    cout<<setprecision(15)<<"Subtask 1: "<<s1<<endl;
    cout<<setprecision(15)<<"Subtask 2: "<<s2<<endl;
    cout<<setprecision(15)<<"Subtask 3: "<<s3<<endl;
    cout<<setprecision(15)<<"Subtask 4: "<<s4<<endl;
    cout<<setprecision(15)<<"Total: "<<s1+s2+s3+s4<<endl;
}
int main()
{
    genBatch();
    system("pause");
    return 0;
}
