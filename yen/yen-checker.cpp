#include<iostream>
#include<fstream>

using namespace std;

const string TASK_NAME="yen";
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";
const string SOLUTION_EXT=".author.out";

string ans;
long long ans1,ans2,ans3;
string _ans;
long long a,b,c,n;

bool getInput()
{
    ifstream inF((TASK_NAME+INPUT_EXT).c_str());
    if (inF.fail()) return 0;
    if (inF) inF>>a;
    else return 0;
    if (inF) inF>>b;
    else return 0;
    if (inF) inF>>c;
    else return 0;
    if (inF) inF>>n;
    else return 0;
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
    if (ans=="Yes")
    {
        if (outF) outF>>ans1;
        else return 0;
        if (outF) outF>>ans2;
        else return 0;
        if (outF) outF>>ans3;
        else return 0;
    }
    return 1;
}
int evaluate()
{
    if (ans!=_ans) return 0;
    if (ans=="Yes" && ans1*a+ans2*b+ans3*c!=n) return 0;
    else return 1;
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
