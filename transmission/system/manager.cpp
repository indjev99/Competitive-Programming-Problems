#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <signal.h>

static int n,d,k,seed,m,m2,n2;
static std::vector<bool> data;
static std::vector<bool> message;
static std::vector<bool> deletions;
static std::vector<bool> corruptedMessage;
static std::vector<bool> reconstructedData;

static double authorScore;
static double score;
static int invalid;

static const long long HASH_BASE=198437;
static const long long HASH_MOD=1940242723;
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
    std::cin>>n>>d>>k>>seed>>authorScore;
    data.resize(n);
    srand(seed);
    for (int i=0;i<n;++i)
    {
        data[i]=rand()%2;
    }
}
static void output_result()
{
    if (invalid==0)
    {
        std::cout<<std::setprecision(6)<<std::min(score/authorScore,1.0)<<"\n";
        std::cerr<<"Result: "<<std::setprecision(6)<<std::min(score/authorScore,1.0)<<"\n";
        return;
    }
    if (invalid==-1)
    {
        std::cout<<"0\n";
        std::cerr<<"Your transmitter returned too many bits.\n";
        return;
    }
    if (invalid==-2)
    {
        std::cout<<"0\n";
        std::cerr<<"Your receiver returned too few/many bits.\n";
        return;
    }
    if (invalid=-3)
    {
        std::cout<<"0\n";
        std::cerr<<"Your receiver didn't get more then half the bits correct.\n";
        return;
    }
    std::cout<<"0\n";
    std::cerr<<"Unknown error.\n";
}
static void generate_deletions()
{
    srand(hash_message_with(seed));
    deletions.resize(m);
    int deleted=0;
    int toDelete=std::min(d,m);
    while (deleted<toDelete)
    {
        int pos=random_number()%m;
        for (int j=0;j<k && pos+j<m && deleted<toDelete;++j)
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
    m2=std::max(0,m-d);
    corruptedMessage.resize(m2);
    int j=0;
    for (int i=0;i<m2;++i)
    {
        while (deletions[j]) ++j;
        corruptedMessage[i]=message[j];
        ++j;
    }
}
static void get_score()
{
    int correct=0;
    for (int i=0;i<n;++i)
    {
        if (reconstructedData[i]==data[i]) ++correct;
    }
    if (2*correct<=n)
    {
       invalid=-3;
       return;
    }
    score=2*correct-n;
    score/=std::max(m2,n);
}
int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    FILE *fifo_in_1, *fifo_out_1, *fifo_in_2, *fifo_out_2;
    fifo_out_1 = fopen(argv[1],"w");
    fifo_in_1 = fopen(argv[2],"r");
    fifo_out_2 = fopen(argv[3],"w");
    fifo_in_2 = fopen(argv[4],"r");


    input();

    int x;

     //message=transmit(data,d);

    fprintf(fifo_out_1,"%d %d\n",n,d);
    for(int i=0;i<n;++i)
    {
        if (i) fprintf(fifo_out_1," ");
        fprintf(fifo_out_1,"%d",(int)data[i]);
    }
    fprintf(fifo_out_1,"\n");
    fflush(fifo_out_1);

    fscanf(fifo_in_1,"%d",&m);
    message.resize(m);
    for (int i=0;i<m;++i)
    {
        fscanf(fifo_in_1,"%d",&x);
        message[i]=x;
    }

    generate_deletions();
    corrupt_message();

    //reconstructedData=receive(corruptedMessage,n,d);

    fprintf(fifo_out_2,"%d %d %d\n",n,d,m2);

    for (int i=0;i<m2;++i)
    {
        if (i) fprintf(fifo_out_2," ");
        fprintf(fifo_out_2,"%d",(int)corruptedMessage[i]);
    }
    fprintf(fifo_out_2,"\n");
    fflush(fifo_out_2);

    fscanf(fifo_in_2,"%d",&n2);
    reconstructedData.resize(n2);
    for (int i=0;i<n2;++i)
    {
        fscanf(fifo_in_2,"%d",&x);
        reconstructedData[i]=x;
    }

    if (m>100000) invalid=-1;
    else if (n2!=n) invalid=-2;

    if (!invalid) get_score();
    output_result();

    fclose(fifo_in_1);
    fclose(fifo_out_1);
    fclose(fifo_in_2);
    fclose(fifo_out_2);

    return 0;
}
