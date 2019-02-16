#include <iostream>
#include "park.h"

const int __MAX_N=1e5+10;
int __n;
bool __direction[__MAX_N*4+1];

bool __invalid;
bool __incorrect;
bool __missing;
bool __stated[__MAX_N*4+1];
int __queries;

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
bool get_xor(const std::vector<std::pair<int, int>>& edges)
{
    ++__queries;
    bool curr_xor=false;
    int num;
    for (int i=0; i<edges.size(); ++i)
    {
        num=__get_number(edges[i]);
        if (num==-1)
        {
            std::cout<<"Your program asked for a non NON-EXISTENT edge "<<edges[i].first<<" - "<<edges[i].second<<".\n";
            if (!__invalid && !__incorrect) __invalid=true;
            return 0;
        }
        curr_xor^=__direction[num];
    }
    std::cout<<"Your program asked for a XOR and the answer is "<<curr_xor<<".\n";
    return curr_xor;
}
void state_direction(const std::pair<int, int>& edge, bool direction)
{
    int num=__get_number(edge);
    if (num==-1)
    {
        std::cout<<"Your program stated the direction of a NON-EXISTENT edge "<<edge.first<<" - "<<edge.second<<" to be "<<direction<<".\n";
        if (!__invalid && !__incorrect) __invalid=true;
    }
    else
    {
        __stated[num]=true;
        if (__direction[num]!=direction)
        {
            std::cout<<"Your program WRONGLY stated the direction of the edge "<<edge.first<<" - "<<edge.second<<" to be "<<direction<<".\n";
            if (!__invalid && !__incorrect) __incorrect=true;
        }
        else
        {
            std::cout<<"Your program CORRECTLY stated the direction of the edge "<<edge.first<<" - "<<edge.second<<" to be "<<direction<<".\n";
        }
    }
}
void __get_input_data()
{
    std::cout<<"N: ";
    std::cin>>__n;
    std::pair<int, int> edge;
    for (int i=0; i<4*__n+1; ++i)
    {
        edge=__get_edge(i);
        std::cout<<edge.first<<" - "<<edge.second<<": ";
        std::cin>>__direction[i];
    }
}
void __check_all()
{
    for (int i=0; i<4*__n+1; ++i)
    {
        if (!__stated[i])
        {
            std::pair<int, int> edge=__get_edge(i);
            std::cout<<"Your program DIDN'T state the direction of "<<edge.first<<" - "<<edge.second<<".\n";
            if (!__invalid && !__incorrect) __missing=true;
        }
    }
}
void __verdict()
{
    if (__invalid)
    {
        std::cout<<"Wrong Answer: Your program asked for an invalid edge.\n";
    }
    else if (__incorrect)
    {
        std::cout<<"Wrong Answer: Your program incorrectly stated the direction of an edge.\n";
    }
    else if (__missing)
    {
        std::cout<<"Wrong Answer: Your program didn't state the directions of all edges.\n";
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
            std::cout<<"Wrong Answer: Your program used too many ("<<__queries<<") queries.\n";
        }
        else if (__queries>ST2)
        {
            std::cout<<"OK: Your program used "<<__queries<<" queries - subtask 1"<<".\n";
        }
        else if (__queries>ST3)
        {
            std::cout<<"OK: Your program used "<<__queries<<" queries - subtask 2"<<".\n";
        }
        else if (__queries>ST4)
        {
            std::cout<<"OK: Your program used "<<__queries<<" queries - subtask 3"<<".\n";
        }
        else
        {
            std::cout<<"OK: Your program used "<<__queries<<" queries - subtask 4"<<".\n";
        }
    }
}
int main()
{
    __get_input_data();
    run(__n);
    __check_all();
    __verdict();
    return 0;
}
