#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<chrono>
using namespace std;
using namespace chrono;
string toString(int n, int digits)//-1 for no formating
{
    string ans="",ans2="";
    do
    {
        ans2+=n%10+'0';
        n/=10;
    }
    while(n!=0);
    if (digits!=-1)
    {
        while (ans2.size()<digits)
        {
            ans2+='0';
        }
    }
    for (int i=ans2.size()-1;i>=0;--i)
    {
        ans+=ans2[i];
    }
    return ans;
}
int main(int argc, char* args[])
{
    string task_name;
    string file_name;
    int digits=2;
    int first_test;
    int last_test;
    double t,t_max=0;
    double res, sum=0;
    string cmd;
    if (argc<5)
    {
        cout<<"Task name: ";
        cin>>task_name;
        cout<<"Executable's name: ";
        cin>>file_name;
        cout<<"Number of first test: ";
        cin>>first_test;
        cout<<"Number of last test: ";
        cin>>last_test;
        cout<<"Number of digits in numbers (-1 for no formating): ";
        cin>>digits;
    }
    else
    {
        task_name=args[1];
        file_name=args[2];
        first_test=atoi(args[3]);
        last_test=atoi(args[4]);
        if (argc>=6) digits=atoi(args[5]);
    }
    high_resolution_clock::time_point start_time_all,start_time,curr_time;
    start_time_all=high_resolution_clock::now();
    for(int i=first_test;i<=last_test;++i)
    {
        cout<<toString(i,digits)<<":"<<endl;

        cmd=file_name+'<'+task_name+'.'+toString(i,digits)+'.'+"in"+'>'+"output";
        start_time=high_resolution_clock::now();
        system(cmd.c_str());
        curr_time=high_resolution_clock::now();

        cmd="checker "+task_name+'.'+toString(i,digits)+'.'+"in"+' '+task_name+'.'+toString(i,digits)+'.'+"sol"+' '+"output"+'>'+"result";
        system(cmd.c_str());

        t=duration_cast<duration<double>>(curr_time-start_time).count();
        if (t>t_max) t_max=t;

        ifstream res_file("result");
        res_file>>res;
        res *= 100.0/(last_test-first_test+1);
        sum+=res;

        cerr<<"Result: "<<res<<'\n';
        cout<<"Time: "<<t<<" seconds"<<'\n';
        cout<<'\n';
    }
    curr_time=high_resolution_clock::now();
    t=duration_cast<duration<double>>(curr_time-start_time_all).count();
    cout<<"Total score: "<<sum<<'\n';
    cout<<"Overall: "<<t<<" seconds"<<'\n';
    cout<<"Maximum: "<<t_max<<" seconds"<<'\n';
    return 0;
}
