#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>

using namespace std;

bool SPECIAL;

const int MAX_N=1e6;
const int MAX_Q=1e6;
const int MAX_T=1e7;

int n,q;
int a[MAX_N];
int qs[MAX_Q];
bool taken[MAX_T+1];

long long random_number()
{
    long long ans=rand()%MAX_T;
    ans=ans*(RAND_MAX+1ll)%MAX_T+rand();
    ans=ans*(RAND_MAX+1ll)%MAX_T+rand();
    ans=ans*(RAND_MAX+1ll)%MAX_T+rand();
    return ans;
}
void add_last(int& mx)
{
    a[n-7]=mx;
    a[n-6]=mx;
    a[n-5]=1;
    a[n-4]=1;
    a[n-3]=1;
    a[n-2]=mx;
    a[n-1]=++mx;
}

///Encho - type 6

long long presums[MAX_N];

long long eval(int t, int ind)
{
    long long ans = presums[ind];

    t -= ind;

    ans += (long long)(t/2) * (long long)(a[ind-1] + a[ind]);

    if (t % 2 == 1)
        ans += (long long)a[ind-1];

    return ans;
}

///

void gen_test(ofstream& outfile, int type)
{
    fill(taken,taken+MAX_T+1,false);
    srand(time(0));
    mt19937 mt(time(0));

    int mx=1;
    if (type==1) // random
    {
        for (int i=0;i<n;++i)
        {
            a[i]=random_number()%MAX_T+1;
        }
        int mxQ=n*5;
        for (int i=0;i<q;++i)
        {
            do
            {
                qs[i]=random_number()%mxQ+1;
            }
            while (taken[qs[i]]);
            taken[qs[i]]=true;
        }
    }
    else if (type==2) // almost increasing, slightly breaks ternary
    {
        for (int i=0;i<n-7;++i)
        {
            if (!rand()%3) a[i]=random_number()%mx+1;
            else a[i]=mx;
            mx+=random_number()%10;
            if (mx>=MAX_T) mx=MAX_T;
        }
        add_last(mx);
        int mxQ=n/2;
        for (int i=0;i<q;++i)
        {
            do
            {
                if (random_number()%4) qs[i]=random_number()%mxQ+1;
                else qs[i]=random_number()%MAX_T+1;
            }
            while (taken[qs[i]]);
            taken[qs[i]]=true;
        }
    }
    else if (type==3) // increasing with gaps, breaks ternary
    {
        for (int i=0;i<n-7;++i)
        {
            if (i<n-9 && i%1000==0)
            {
                a[i++]=1;
                a[i++]=1;
            }
            a[i]=mx;
            mx+=random_number()%10;
            if (mx>=MAX_T) mx=MAX_T;
        }
        add_last(mx);
        int mxQ=n/2;
        for (int i=0;i<q;++i)
        {
            do
            {
                if (random_number()%4) qs[i]=random_number()%mxQ+1;
                else qs[i]=random_number()%MAX_T+1;
            }
            while (taken[qs[i]]);
            taken[qs[i]]=true;
        }
    }
    else if (type==4) // increasing no gaps
    {
        for (int i=0;i<n-7;++i)
        {
            a[i]=mx;
            mx+=random_number()%10;
            if (mx>=MAX_T) mx=MAX_T;
        }
        add_last(mx);
        int mxQ=n/2;
        for (int i=0;i<q;++i)
        {
            do
            {
                if (random_number()%4) qs[i]=random_number()%mxQ+1;
                else qs[i]=random_number()%MAX_T+1;
            }
            while (taken[qs[i]]);
            taken[qs[i]]=true;
        }
    }
    else if (type==5) // T*N breaking
    {
        mx=4e6;
        for (int i=0;i<n;++i)
        {
            if (i%8==0) a[i]=mx;
            else if (i%8==1) a[i]=mx;
            else if (i%8==2) a[i]=1;
            else if (i%8==3) a[i]=1;
            else if (i%8==4) a[i]=mx;
            else if (i%8==5) a[i]=++mx;
            else if (i%8==6) a[i]=1;
            else if (i%8==7) a[i]=1;
            if (mx>=MAX_T) mx=MAX_T;
        }
        int range=6*(mx-4e6)+1;
        for (int i=0;i<q;++i)
        {
            do
            {
                if (random_number()%4) qs[i]=8e6+random_number()%range;
                else qs[i]=random_number()%MAX_T+1;
            }
            while (taken[qs[i]]);
            taken[qs[i]]=true;
        }
    }
    else if (type==6) // ternary breaking
    {
        for (int i=0;i<n;i++)
        {
            a[i] = 1;
        }

        int V = n;
        int total = 0;

        for (int i=1;i<n;i+=4)
        {
            total++;

            a[i] = V;

            if (i + 1 < n)
                a[i+1] = 1;
            if (i + 2 < n)
                a[i+2] = 1;
            if (i + 3 < n)
                a[i+3] = 1;

            V++;
        }

        presums[0] = a[0];
        for (int i=1;i<n;i++)
        {
            presums[i] = presums[i-1] + a[i];
        }

        long long initValue = eval(n, 1);
        int winner = mt() % (total - 100) + 1;
        int current = 0;
        int bad = 0;

        for (int i=1;i+4<n;i+=4)
        {
            current++;

            long long goalValue;

            if (current != winner)
                goalValue = initValue + mt() % 201 - 100;
            else
                goalValue = initValue + 101;

            if (current == winner)
                cerr<<"Optimal position is at "<<i<<endl;

            a[i+2] = goalValue - eval(n, i+4);

            if (a[i+2] > a[i])
            {
                bad++;

                if (bad > 200)
                    cerr<<"UNEXPECTED BEHAVIOUR PLEASE STOP"<<endl;

                a[i+2] = 1;
            }

            if (a[i+2] < 1)
                cerr<<"UNEXPECTED BEHAVIOUR PLEASE STOP!"<<endl;

            for (int j = i; j < n && j < i+20;j++)
            {
                presums[j] = presums[j-1] + a[j];
            }
        }

        for (int i=0;i<q;i++)
        {
            qs[i] = n+i;
        }
    }
    else std::cerr<<"UNKNOWN TYPE"<<std::endl;

    std::cerr<<"MAX: "<<mx<<std::endl;

    if (SPECIAL)
    {
        for (int i=0;i<q;++i)
        {
            qs[i]=i+1;
        }
    }

    std::random_shuffle(qs,qs+q);

    outfile<<n<<' '<<q<<'\n';
    for (int i=0;i<n;++i)
    {
        if (i) outfile<<' ';
        outfile<<a[i];
    }
    outfile<<'\n';
    for (int i=0;i<q;++i)
    {
        outfile<<qs[i]<<'\n';
    }
}
int main()
{
    ios::sync_with_stdio(false);
    //cin.tie(nullptr);
    int num,type;
    int on,oq;
    while (1)
    {
        cout<<"Test num? ";
        cin>>num;
        cout<<"N? ";
        cin>>n;
        cout<<"Q? ";
        cin>>q;
        cout<<"Type? ";
        cin>>type;
        cout<<"Special? ";
        cin>>SPECIAL;

        if (n==0) n=on;
        else on=n;
        if (q==0) q=oq;
        else oq=q;
        string nums="";
        nums+=(num/10+'0');
        nums+=(num%10+'0');
        ofstream outfile("goblins."+nums+".in");

        gen_test(outfile,type);
    }
    return 0;
}
