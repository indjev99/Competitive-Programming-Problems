#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
#include<algorithm>
#include<time.h>
#include<chrono>
#define PI 3.14159265359
using namespace std;
using namespace chrono;
const int MAX_N=60000;
const int MAX_M=20;
const double EPS=0.01;
const int MAX_COORD=10000;
int n,m;
double x,y;
int random()
{
    return rand()*RAND_MAX+rand();
}
void randomise(double &x, double &y, int m)
{
    if (m==0)
    {
        x=random()%int(2*MAX_COORD/EPS+1)*EPS-MAX_COORD;
        y=random()%int(2*MAX_COORD/EPS+1)*EPS-MAX_COORD;
    }
    else if (m==1)
    {
        x=random()%int(2*MAX_COORD/EPS+1)*EPS-MAX_COORD;
        y=MAX_COORD;
        if (random()%2)
        {
            y=-y;
        }
        if (random()%2)
        {
            swap(x,y);
        }
    }
    else
    {
        double alpha;
        alpha=random()*1.0/(RAND_MAX*RAND_MAX+RAND_MAX)*2*PI;
        x=cos(alpha)*MAX_COORD;
        y=sin(alpha)*MAX_COORD;
    }
}
int main()
{
    srand(time(0));
    string filename="";
    ofstream test;
    ifstream ans;
    string ans1,ans2;
    string cmd;
    high_resolution_clock::time_point start_time,curr_time;
    while (filename!="end")
    {
        cin>>filename;
        filename="TTS."+filename;
        test.open(filename+".in");
        n=random()%MAX_N;
        m=random()%MAX_M;
        n=MAX_N;
        m=MAX_M;
        test<<n<<' '<<m<<'\n';
        for (int i=0;i<n;++i)
        {
            randomise(x,y,1);
            test<<x<<' '<<y<<'\n';
        }
        for (int i=0;i<m;++i)
        {
            randomise(x,y,0);
            test<<x<<' '<<y<<'\n';
        }
        test.close();

        cmd="author<"+filename+".in>"+filename+".out";
        start_time=high_resolution_clock::now();
        system(cmd.c_str());
        curr_time=high_resolution_clock::now();
        ans.open(filename+".out");
        ans>>ans1;
        ans.close();
        cout<<"author: "<<ans1<<" for "<<duration_cast<duration<double>>(curr_time-start_time).count()<<endl;

        /*cmd="sol2<"+filename+".in>"+filename+".sol2.out";
        start_time=high_resolution_clock::now();
        system(cmd.c_str());
        ans.open(filename+".sol2.out");
        curr_time=high_resolution_clock::now();
        ans>>ans2;
        ans.close();
        cout<<"sol2: "<<ans2<<" for "<<duration_cast<duration<double>>(curr_time-start_time).count()<<endl;*/
    }
    return 0;
}
