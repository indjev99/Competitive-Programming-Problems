#include <iostream>
#include <stdlib.h>
#include "park.h"

const int __MAX_N=1e5+10;
int __n;
bool __direction[__MAX_N*4+1];

bool __invalid;
bool __incorrect;
bool __missing;
bool __stated[__MAX_N*4+1];
int __queries;

bool __adaptive;
bool __direction2[__MAX_N*4+1];
bool __known[__MAX_N*4+1];

const long long __HASH_BASE=133337;
const long long __HASH_MOD=1234567891;
long long __hash_input_with(long long beg)
{
    long long hsh=(beg+__HASH_MOD)%__HASH_MOD;
    hsh=(hsh*__HASH_BASE+__n)%__HASH_MOD;
    for (int i=0; i<4*__n+1; ++i)
    {
        hsh=(hsh*__HASH_BASE+__direction[i])%__HASH_MOD;
    }
    return hsh;
}
int __get_number(const std::pair<int, int>& edge)
{
    int f=edge.first;
    int t=edge.second;
    int num=f+t-3;
    if (t==f || t-f<-2 || t-f>2 || num<0 || num>4*__n) return -1;
    return num;
}
std::pair<int, int> __get_edge(int num)
{
    int f,t;
    f=num/2+1;
    t=f+1+num%2;
    return {f,t};
}
void __init_adaptive()
{
    srand(__hash_input_with(420));
    for (int i=0; i<4*__n+1; ++i)
    {
        if (i%2 || i==0 || i==4*__n)
        {
            __known[i]=true;
            __direction2[i]=true;
        }
    }
}
void __set_direction(int e, bool dir)
{
    if (dir)
    {
        __direction2[e]=true;
        __known[e]=true;
    }
    else
    {
        __direction2[e]=false;
        __known[e]=true;
        __direction2[e-2]=true;
        __known[e-2]=true;
        __direction2[e+2]=true;
        __known[e+2]=true;
    }
}
bool __are_adj(int e1, int e2)
{
    return e1-e2==-2 || e1-e2==2;
}
int __side_to_pick;
const int CHANCE_FOR_BETTER=50;
void __handle_two_adj(int e1, int e2)
{
    if (rand()%2) std::swap(e1,e2);
    __set_direction(e1,true);
    __set_direction(e2,true);
    if (__known[e1-2] && __known[e1+2])
    {
        if (rand()%CHANCE_FOR_BETTER) __set_direction(e1,false);
        else __set_direction(e2,false);
    }
    else if (__known[e2-2] && __known[e2+2])
    {
        if (rand()%CHANCE_FOR_BETTER) __set_direction(e2,false);
        else __set_direction(e1,false);
    }
    else
    {
        if (__side_to_pick==1)
        {
            __set_direction(std::min(e1,e2),false);
            __set_direction(std::max(e1,e2),false);
        }
        else if (__side_to_pick==2)
        {
            __set_direction(std::max(e1,e2),false);
            __set_direction(std::min(e1,e2),false);
        }
        else
        {
            if (rand()%2) __set_direction(e1,false);
            else __set_direction(e2,false);
        }
    }
}
bool __handle_random(const std::vector<int> &e)
{
    bool curr_xor=false;
    for (int i=0;i<e.size();++i)
    {
        __set_direction(e[i],__direction[e[i]]);
        curr_xor^=__direction[e[i]];
    }
    return curr_xor;
}
std::vector<int> __others;
std::vector<int> __others2;
int __occ[__MAX_N*4+1];
bool get_xor(const std::vector<std::pair<int, int>>& edges)
{
    ++__queries;
    if (!__adaptive)
    {
        bool curr_xor=false;
        int num;
        for (int i=0; i<edges.size(); ++i)
        {
            num=__get_number(edges[i]);
            if (num==-1)
            {
                if (!__invalid && !__incorrect) __invalid=true;
                return 0;
            }
            curr_xor^=__direction[num];
        }
        return curr_xor;
    }
    else
    {
        __others.resize(0);
        __others2.resize(0);
        bool known_xor=false,other_xor;
        int num;
        for (int i=0; i<edges.size(); ++i)
        {
            num=__get_number(edges[i]);
            if (num==-1)
            {
                if (!__invalid && !__incorrect) __invalid=true;
                return 0;
            }
            if (__known[num]) known_xor^=__direction2[num];
            else
            {
                if (__occ[num]==0)
                {
                    __others2.push_back(num);
                }
                ++__occ[num];
            }
        }
        for (int i=0;i<__others2.size();++i)
        {
            num=__others2[i];
            if (__occ[num]%2) __others.push_back(num);
            __occ[num]=0;
        }
        if (__others.size()==0)
        {
            other_xor=false;
        }
        else if (__others.size()==1)
        {
            num=__others[0];
            __set_direction(num,true);
            other_xor=true;
        }
        else if (__others.size()==2)
        {
            int num1=__others[0];
            int num2=__others[1];
            if (__are_adj(num1,num2))
            {
                __handle_two_adj(num1,num2);
                other_xor=true;
            }
            else
            {
                if (rand()%CHANCE_FOR_BETTER)
                {
                    __set_direction(num1,true);
                    __set_direction(num2,true);
                }
                else
                {
                    __set_direction(num1,false);
                    __set_direction(num2,false);
                }
                other_xor=false;
            }
        }
        else if (__others.size()==3)
        {
            int num1=__others[0];
            int num2=__others[1];
            int num3=__others[2];
            if (!__are_adj(num1,num2))
            {
                if (__are_adj(num1,num3)) std::swap(num2,num3);
                else if (__are_adj(num2,num3)) std::swap(num1,num3);
            }
            if (__are_adj(num1,num2))
            {
                if (__are_adj(num1,num3) || __are_adj(num2,num3))
                {
                    if (__are_adj(num2,num3)) std::swap(num1,num2);
                    __set_direction(num1,true);
                    __set_direction(num2,true);
                    __set_direction(num3,true);
                    if (rand()%CHANCE_FOR_BETTER) __set_direction(num1,false);
                    else __handle_two_adj(num2,num3);
                    other_xor=false;
                }
                else
                {
                    __handle_two_adj(num1,num2);
                    other_xor=true;
                    if (rand()%CHANCE_FOR_BETTER)
                    {
                        other_xor^=true;
                        __set_direction(num3,true);
                    }
                    else
                    {
                        other_xor^=false;
                        __set_direction(num3,false);
                    }
                }
            }
            else
            {
                other_xor=__handle_random(__others);
            }
        }
        else
        {
            other_xor=__handle_random(__others);
        }
        return known_xor^other_xor;
    }
}
void state_direction(const std::pair<int, int>& edge, bool direction)
{
    int num=__get_number(edge);
    if (num==-1)
    {
        if (!__invalid && !__incorrect) __invalid=true;
    }
    else
    {
        __stated[num]=true;
        if (!__adaptive)
        {
            if (__direction[num]!=direction)
            {
                if (!__invalid && !__incorrect) __incorrect=true;
            }
        }
        else
        {
            if (!__known[num] || __direction2[num]!=direction)
            {
                if (!__invalid && !__incorrect) __incorrect=true;
            }
        }
    }
}
long long __string_to_number(const std::string& s)
{
    long long num=0;
    for (int i=0;i<s.size();++i)
    {
        if (s[i]<'0' || s[i]>'9') return -1;
        num*=10;
        num+=s[i]-'0';
    }
    return num;
}
bool __get_input_data()
{
    /*std::string s;
    if (!std::cin) return false;
    std::cin>>s;
    __n=__string_to_number(s);
    if (__n>=__MAX_N || __n<=0) return false;
    int a;
    for (int i=0; i<4*__n+1; ++i)
    {
        if (!std::cin) return false;
        std::cin>>s;
        a=__string_to_number(s);
        if (a<0 || a>1) return false;
        __direction[i]=a;
    }
    __adaptive=false;
    if (std::cin)
    {
        std::cin>>s;
        if (__string_to_number(s)==__hash_input_with(0))
        {
            __adaptive=true;
            std::cin>>__side_to_pick;
        }
    }
    return true;*/

    __n=__MAX_N;
    for (int i=0; i<4*__n+1; ++i)
    {
        __direction[i]=1;
    }
    __adaptive=false;
    return true;
}
void __check_all()
{
    for (int i=0; i<4*__n+1; ++i)
    {
        if (!__stated[i])
        {
            if (!__invalid && !__incorrect) __missing=true;
        }
    }
}
void __verdict()
{
    if (__invalid)
    {
        std::cout<<__hash_input_with(-4)<<"\n";
        std::cout<<(-4)<<"\n";
    }
    else if (__incorrect)
    {
        std::cout<<__hash_input_with(-3)<<"\n";
        std::cout<<(-3)<<"\n";
    }
    else if (__missing)
    {
        std::cout<<__hash_input_with(-2)<<"\n";
        std::cout<<(-2)<<"\n";
    }
    else
    {
        int ST1=3*__n;
        int ST2=2*__n;
        int ST3=3*__n/2;
        if (3*__n%2) ++ST3;
        int ST4=16*__n/11;
        if (16*__n%11) ++ST4;
        if (__queries>ST1)
        {
            std::cout<<__hash_input_with(-1)<<"\n";
            std::cout<<(-1)<<"\n";
        }
        else if (__queries>ST2)
        {
            std::cout<<__hash_input_with(1)<<"\n";
            std::cout<<(1)<<"\n";
        }
        else if (__queries>ST3)
        {
            std::cout<<__hash_input_with(2)<<"\n";
            std::cout<<(2)<<"\n";
        }
        else if (__queries>ST4)
        {
            std::cout<<__hash_input_with(3)<<"\n";
            std::cout<<(3)<<"\n";
        }
        else
        {
            std::cout<<__hash_input_with(4)<<"\n";
            std::cout<<(4)<<"\n";
        }
    }
}
void __wrong_input()
{
    std::cout<<0<<"\n";
}
#include <time.h>
int main()
{
    time_t start=clock();
    if (!__get_input_data())
    {
        __wrong_input();
        return 0;
    }
    if (__adaptive) __init_adaptive();
    run(__n);
    __check_all();
    __verdict();
    time_t curr=clock();
    std::cerr<<(curr-start)*1.0/CLOCKS_PER_SEC<<std::endl;
    return 0;
}
