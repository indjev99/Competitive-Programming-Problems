#include<iostream>
#include<fstream>

using namespace std;

const string TASK_NAME="sequences";
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
const string SOLUTION_EXT=".author.out";

const int MAX_Q=40000;
int q;
long long ans[MAX_Q];
long long _ans[MAX_Q];

bool getInput()
{
    ifstream inF((TASK_NAME+INPUT_EXT).c_str());
    if (inF.fail()) return 0;
    if (inF) inF>>q;
    else return 0;
    long long b1,c1,s1,b2,c2,s2,n;
    for (int i=0;i<q;++i)
    {
        if (inF) inF>>b1;
        else return 0;
        if (inF) inF>>c1;
        else return 0;
        if (inF) inF>>s1;
        else return 0;
        if (inF) inF>>b2;
        else return 0;
        if (inF) inF>>c2;
        else return 0;
        if (inF) inF>>s2;
        else return 0;
        if (inF) inF>>n;
        else return 0;
    }
    return 1;
}
bool getSolution()
{
    ifstream solF((TASK_NAME+SOLUTION_EXT).c_str());
    if (solF.fail()) return 0;
    for (int i=0;i<q;++i)
    {
        if (solF) solF>>_ans[i];
        else return 0;
    }
    return 1;
}
bool getOutput()
{
    ifstream outF((TASK_NAME+OUTPUT_EXT).c_str());
    if (outF.fail()) return 0;
    for (int i=0;i<q;++i)
    {
        if (outF) outF>>ans[i];
        else return 0;
    }
    return 1;
}
int evaluate()
{
    for (int i=0;i<q;++i)
    {
        if (ans[i]!=_ans[i]) return 0;
    }
    return 1;
}
int main()
{
    if (!getInput())
    {
        cout<<"ERR\nBad input data, please inform the authors\n";
        return 0;
    }
    if (!getSolution())
    {
        cout<<"ERR\nBad solution data, please inform the authors\n";
        return 0;
    }
    if (!getOutput())
    {
        cout<<"ERR\nBad contestant output file\n";
        return 0;
    }
    int result=evaluate();
    if (result>0) cout<<"OK 1\n";
    else cout<<"WRONG ANSWER\n";
    return 0;
}
