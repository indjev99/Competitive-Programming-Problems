#include "transmission.h"
#include <iostream>
#include <stdlib.h>

static int n,d,k;
static std::vector<bool> data;
static std::vector<bool> message;
static std::vector<bool> deletions;
static std::vector<bool> corruptedMessage;
static std::vector<bool> reconstructedData;
static long double score;

static long long random_number()
{
    return rand()*(RAND_MAX+1ll)+rand();
}
static void input()
{
    std::cin>>n>>d>>k;
    data.resize(n);
    bool curr;
    for (int i=0;i<n;++i)
    {
        //std::cin>>curr;
        curr=rand()%2;
        data[i]=curr;
    }
}
static void generate_deletions()
{
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
int correct;
static void evaluate()
{
    if (reconstructedData.size()!=n)
    {
        correct=-1;
        score=0;
        return;
    }
    correct=0;
    for (int i=0;i<n;++i)
    {
        if (reconstructedData[i]==data[i]) ++correct;
    }
    score=2*correct-n;
    score/=std::max((int)corruptedMessage.size(),n);
    if (score<0) score=0;
}
int main()
{
    srand(0);
    input();
    message=transmit(data,d);
    generate_deletions();
    corrupt_message();
    reconstructedData=receive(corruptedMessage,n,d);
    evaluate();
    std::cout<<"Your score: "<<score<<std::endl;
    std::cout<<"Correct: "<<correct<<" / "<<n<<std::endl;
    std::cout<<"Sent: "<<message.size()<<std::endl;
    std::cout<<"Received: "<<corruptedMessage.size()<<std::endl;
    return 0;
}
