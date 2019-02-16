#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<fstream>

using namespace std;

const string TASK_NAME="sorting";
const string INPUT_EXT=".in";

const int MAX_N=400;
int TESTS_IN_BATCH;
int SEED;
int n;
int perm[MAX_N];
int tp;
int cp[MAX_N][MAX_N];
int tv;
int cv[MAX_N][MAX_N];

void genPerm()
{
    for (int i=0;i<n;++i)
    {
        perm[i]=i+1;
    }
    for (int i=n-1;i>=0;--i)
    {
        swap(perm[i],perm[rand()%(i+1)]);
    }
}
void genArrZero(int arr[MAX_N][MAX_N], int &t)
{
    t=0;
    for (int i=0;i<n;++i)
    {
        arr[i][i]=0;
        for (int j=i+1;j<n;++j)
        {
            arr[i][j]=0;
            arr[j][i]=arr[i][j];
        }
    }
}
void genArrRand(int arr[MAX_N][MAX_N], int &t)
{
    t=1;
    for (int i=0;i<n;++i)
    {
        arr[i][i]=0;
        for (int j=i+1;j<n;++j)
        {
            arr[i][j]=(rand()*(RAND_MAX+1)+rand())%(n*4)+1;
            arr[j][i]=arr[i][j];
        }
    }
}
void genArrLin(int arr[MAX_N][MAX_N], int &t)
{
    t=2;
    for (int i=0;i<n;++i)
    {
        arr[i][i]=0;
        for (int j=i+1;j<n;++j)
        {
            arr[i][j]=(j-i)*6;
            arr[j][i]=arr[i][j];
        }
    }
}
void genArrSqrt(int arr[MAX_N][MAX_N], int &t)
{
    t=3;
    for (int i=0;i<n;++i)
    {
        arr[i][i]=0;
        for (int j=i+1;j<n;++j)
        {
            arr[i][j]=round(sqrt(j-i)*sqrt(n)*15/4);
            arr[j][i]=arr[i][j];
        }
    }
}
void genArrMax(int arr[MAX_N][MAX_N], int &t)
{
    t=4;
    for (int i=0;i<n;++i)
    {
        arr[i][i]=0;
        for (int j=i+1;j<n;++j)
        {
            arr[i][j]=round(max(i,j)+1)*3;
            arr[j][i]=arr[i][j];
        }
    }
}
void printTest(ofstream &outF)
{
    outF<<n<<'\n';
    for (int i=0;i<n;++i)
    {
        outF<<perm[i]<<' ';
    }
    outF<<'\n';
    outF<<tp<<'\n';
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            outF<<cp[i][j]<<' ';
        }
        outF<<'\n';
    }
    outF<<tv<<'\n';
    for (int i=0;i<n;++i)
    {
        for (int j=0;j<n;++j)
        {
            outF<<cv[i][j]<<' ';
        }
        outF<<'\n';
    }
}
void genTest(void (*genArrCP)(int[MAX_N][MAX_N], int &), void (*genArrCV)(int[MAX_N][MAX_N], int &), ofstream &outF)
{
    genPerm();
    genArrCP(cp,tp);
    genArrCV(cv,tv);
    printTest(outF);
}
int curr=1;
string getName()
{
    string name;
    name=TASK_NAME;
    name+='.';
    name+='0'+curr/10;
    name+='0'+curr%10;
    name+=INPUT_EXT;
    ++curr;
    return name;
}
void genSmallBatch(void (*genArrCP)(int[MAX_N][MAX_N], int &), void (*genArrCV)(int[MAX_N][MAX_N], int &))
{
    string name;
    ofstream currOutF;
    for (int i=0;i<TESTS_IN_BATCH;++i)
    {
        name=getName();
        cerr<<name<<endl;
        currOutF.open(name.c_str());
        genTest(genArrCP,genArrCV,currOutF);
        currOutF.close();
    }

}
void genBigBatch()
{
    void (*genArrCP)(int[MAX_N][MAX_N], int &);
    void (*genArrCV)(int[MAX_N][MAX_N], int &);
    for (int i=1;i<5;++i)
    {
        if (i==1) genArrCP=&genArrRand;
        else if (i==2) genArrCP=&genArrLin;
        else if (i==3) genArrCP=&genArrSqrt;
        else if (i==4) genArrCP=&genArrMax;
        for (int j=0;j<5;++j)
        {
            if (j==0) genArrCV=&genArrZero;
            else if (j==1) genArrCV=&genArrRand;
            else if (j==2) genArrCV=&genArrLin;
            else if (j==3) genArrCV=&genArrSqrt;
            else if (j==4) genArrCV=&genArrMax;
            genSmallBatch(genArrCP,genArrCV);
        }
    }
}
int main()
{
    cin>>SEED;
    cin>>TESTS_IN_BATCH;
    srand(SEED);
    n=400;
    genBigBatch();
    return 0;
}
