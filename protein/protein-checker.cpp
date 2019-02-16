#include<iostream>
#include<fstream>

using namespace std;

const string TASK_NAME="protein";
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
const string SOLUTION_EXT=".author.out";

const long long MAX_K=1e9;
const int MAX_S=10;
const int MAX_M=10;
bool nums[MAX_S+1];
int n, m;
int lnum;
long long k;
long long _ans;
long long ans;

bool getInput()
{
    ifstream inF((TASK_NAME+INPUT_EXT).c_str());
    if (inF.fail()) return 0;
    if (inF) inF>>k;
    else return 0;
    if (inF) inF>>n;
    else return 0;
    int a;
    for (int i=0;i<n;++i)
    {
        if (inF) inF>>a;
        else return 0;
        a/=10;
        nums[a]=1;
    }
    if (inF) inF>>lnum;
    else return 0;
    if (inF) inF>>m;
    else return 0;
    lnum/=10;
    nums[lnum]=0;
    return 1;
}
bool getSolution()
{
    ifstream solF((TASK_NAME+SOLUTION_EXT).c_str());
    if (solF.fail()) return 0;
    if (solF) solF>>_ans;
    else return 0;
    return 1;
}
bool getOutput()
{
    ifstream outF((TASK_NAME+OUTPUT_EXT).c_str());
    if (outF.fail()) return 0;
    if (outF) outF>>ans;
    else return 0;
    return 1;
}
int evaluate()
{
    if (ans==_ans) return 1;
    else return 0;
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
