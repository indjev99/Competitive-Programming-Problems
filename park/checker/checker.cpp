#include <fstream>
#include <iostream>

const int __MAX_N=1e5+10;
int __n;
bool __direction[__MAX_N*4+1];

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
std::ifstream in,out;
bool __get_input_data()
{
    std::string s;
    if (!in) return false;
    in>>s;
    __n=__string_to_number(s);
    if (__n>=__MAX_N || __n<=0) return false;
    int a;
    for (int i=0; i<4*__n+1; ++i)
    {
        if (!in) return false;
        in>>s;
        a=__string_to_number(s);
        if (a<0 || a>1) return false;
        __direction[i]=a;
    }
    return true;
}
void __get_score()
{
    long long res;
    out>>res;
    if (res==__hash_input_with(-4))
    {
        std::cout<<"0\n";
        std::cerr<<"Your program asked for an invalid edge.\n";
        return;
    }
    if (res==__hash_input_with(-3))
    {
        std::cout<<"0\n";
        std::cerr<<"Your program incorrectly stated the direction of an edge.\n";
        return;
    }
    if (res==__hash_input_with(-2))
    {
        std::cout<<"0\n";
        std::cerr<<"Your program didn't state the directions of all edges.\n";
        return;
    }
    if (res==__hash_input_with(-1))
    {
        std::cout<<"0\n";
        std::cerr<<"Your program used more than 3N queries.\n";
        return;
    }
    if (res==__hash_input_with(1))
    {
        std::cout<<"0.15\n";
        std::cerr<<"Your program used more than 2N queries.\n";
        return;
    }
    if (res==__hash_input_with(2))
    {
        std::cout<<"0.45\n";
        std::cerr<<"Your program used more than 3N/2 queries.\n";
        return;
    }
    if (res==__hash_input_with(3))
    {
        std::cout<<"0.7\n";
        std::cerr<<"Your program used more than 16N/11 queries.\n";
        return;
    }
    if (res==__hash_input_with(4))
    {
        std::cout<<"1\n";
        return;
    }
    std::cout<<"0\n";
    std::cerr<<"Your program printed to stdout, it shouldn't.\n";
}
int main(int argc, char *argv[])
{
    in.open(argv[1]);
    out.open(argv[3]);

    if (!in)
    {
        std::cout<<"0\n";
        std::cerr<<"In-file "<<argv[1]<<" not found.\n";
        return 0;
    }
    if (!out)
    {
        std::cout<<"0\n";
        std::cerr<<"Out-file "<<argv[3]<<" not found.\n";
        return 0;
    }
    if (!__get_input_data())
    {
        std::cout<<"0\n";
        std::cerr<<"Bad in-file.\n";
        return 0;
    }
    __get_score();
    return 0;
}
