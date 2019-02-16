#include<iostream>
#include<fstream>

using namespace std;

const string TASK_NAME="sorting";
const string INPUT_EXT=".in";
const string OUTPUT_EXT=".out";

const int MAX_N=400;
int n;
int perm[MAX_N];
int tp;
int cp[MAX_N][MAX_N];
int tv;
int cv[MAX_N][MAX_N];

struct Swap
{
    int f,t;
};
const int MAX_K=1e6;
int k;
Swap s[MAX_K];

bool getInput()
{
    ifstream inF((TASK_NAME+INPUT_EXT).c_str());
    if (inF.fail()) return 0;
    if (inF) inF>>n;
    else return 0;
    for (int i=0;i<n;++i)
    {
        if (inF) inF>>perm[i];
        else return 0;
    }
    if (inF) inF>>tp;
    else return 0;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            if (inF) inF>>cp[i][j];
            else return 0;
        }
    }
    if (inF) inF>>tv;
    else return 0;
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            if (inF) inF>>cv[i][j];
            else return 0;
        }
    }
    return 1;
}
bool getOutput()
{
    ifstream outF((TASK_NAME+OUTPUT_EXT).c_str());
    if (outF.fail()) return 0;
    if (outF) outF>>k;
    else return 0;
    if (k>MAX_K) return 0;
    if (k<0) return 0;
    for (int i=0;i<k;++i)
    {
        if (outF) outF>>s[i].f;
        else return 0;
        if (outF) outF>>s[i].t;
        else return 0;
        if (s[i].f<=0) return 0;
        if (s[i].f>n) return 0;
        if (s[i].t<=0) return 0;
        if (s[i].t>n) return 0;
    }
    return 1;
}
int evaluate()
{
    int result=0;
    int f,t;
    int fv,tv;
    for (int i=0;i<k;++i)
    {
        f=s[i].f-1;
        t=s[i].t-1;
        fv=perm[f]-1;
        tv=perm[t]-1;
        result+=cp[f][t];
        result+=cv[fv][tv];
        swap(perm[f],perm[t]);
    }
    for (int i=0;i<n;++i)
    {
        if (perm[i]!=i+1) return -1;
    }
    return result;
}
int main()
{
    if (!getInput())
    {
        cout<<"ERR\nBad input data, please inform the authors\n";
        return 0;
    }
    if (!getOutput())
    {
        cout<<"ERR\nBad contestant output file\n";
        return 0;
    }
    int result=evaluate();
    if (result>=0) cout<<"OK: "<<result<<"\n";
    else cout<<"WRONG ANSWER\nSequence not sorted\n";
    return 0;
}
