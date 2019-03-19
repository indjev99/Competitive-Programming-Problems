#include "transmission.h"
#include <iostream>
#include <stdlib.h>

static int n,d,k,seed;
static std::vector<bool> data;
static std::vector<bool> message;
static std::vector<bool> deletions;
static std::vector<bool> corruptedMessage;
static std::vector<bool> reconstructedData;

static const long long HASH_BASE=198437;
static const long long HASH_MOD=1940242723;
static long long hash_input_with(long long num)
{
    long long hsh=num%HASH_MOD;
    hsh=(hsh*HASH_BASE+n)%HASH_MOD;
    hsh=(hsh*HASH_BASE+d)%HASH_MOD;
    hsh=(hsh*HASH_BASE+k)%HASH_MOD;
    hsh=(hsh*HASH_BASE+seed)%HASH_MOD;
    return hsh;
}
static long long hash_message_with(long long num)
{
    long long hsh=num%HASH_MOD;
    for (int i=0;i<message.size();++i)
    {
        hsh=(hsh*HASH_BASE+message[i])%HASH_MOD;
    }
    return hsh;
}
static long long random_number()
{
    return rand()*(RAND_MAX+1ll)+rand();
}
static void input()
{
    std::cin>>n>>d>>k>>seed;
    data.resize(n);
    srand(seed);
    for (int i=0;i<n;++i)
    {
        data[i]=rand()%2;
    }
}
static void output()
{
    std::cout<<hash_input_with(0)<<'\n';
    if (message.size()>100000)
    {
        std::cout<<"-1 -1\n";
        return;
    }
    std::cout<<reconstructedData.size()<<' '<<corruptedMessage.size()<<'\n';
    for (int i=0;i<reconstructedData.size();++i)
    {
        if (i) std::cout<<' ';
        std::cout<<reconstructedData[i];
    }
    std::cout<<'\n';
}
static void generate_deletions()
{
    srand(hash_message_with(seed));
    deletions.resize(message.size());
    int deleted=0;
    int toDelete=std::min(d,(int)message.size());
    while (deleted<toDelete)
    {
        int pos=random_number()%message.size();
        for (int j=0;j<k && pos+j<message.size() && deleted<toDelete;++j)
        {
            if (deletions[pos+j]==0)
            {
                deletions[pos+j]=1;
                ++deleted;
            }
        }
    }
}
static void corrupt_message()
{
    corruptedMessage.resize(std::max(0,(int)message.size()-d));
    int j=0;
    for (int i=0;i<corruptedMessage.size();++i)
    {
        while (deletions[j]) ++j;
        corruptedMessage[i]=message[j];
        ++j;
    }
}
int main()
{
    input();
    message=transmit(data,d);
    if (message.size()<=100000)
    {
        generate_deletions();
        corrupt_message();
        reconstructedData=receive(corruptedMessage,n,d);
    }
    output();
    return 0;
}
