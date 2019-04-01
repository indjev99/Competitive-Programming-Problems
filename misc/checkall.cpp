#include<iostream>
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
int main()
{
    string task_name;
    string file_name;
    int digits;
    int first_test;
    int last_test;
    double t,t_max=0;
    string cmd;
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
    high_resolution_clock::time_point start_time_all,start_time,curr_time;
    start_time_all=high_resolution_clock::now();
    for(int i=first_test;i<=last_test;++i)
    {
        cout<<toString(i,digits)<<":"<<endl;

        cmd=file_name+'<'+task_name+'.'+toString(i,digits)+'.'+"in"+'>'+"output";
        start_time=high_resolution_clock::now();
        system(cmd.c_str());
        curr_time=high_resolution_clock::now();

        cmd="checker "+task_name+'.'+toString(i,digits)+'.'+"in"+' '+task_name+'.'+toString(i,digits)+'.'+"out"+' '+"output";
        system(cmd.c_str());

        t=duration_cast<duration<double>>(curr_time-start_time).count();
        if (t>t_max) t_max=t;
        cout<<t<<" seconds"<<'\n'<<'\n';
    }
    curr_time=high_resolution_clock::now();
    t=duration_cast<duration<double>>(curr_time-start_time_all).count();
    cout<<"Overall: "<<t<<" seconds"<<'\n';
    cout<<"Maximum: "<<t_max<<" seconds"<<'\n';
    return 0;
}
