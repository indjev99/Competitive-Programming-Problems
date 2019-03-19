#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdlib.h>

int n,d,chunkSize,seed;
std::vector<bool> data;
int m2;
std::vector<bool> reconstructedData;
std::ifstream in,author,out;
double authorScore;
double score;
double invalid;

const long long HASH_BASE=198437;
const long long HASH_MOD=1940242723;
long long hash_input_with(long long num)
{
    long long hsh=num%HASH_MOD;
    hsh=(hsh*HASH_BASE+n)%HASH_MOD;
    hsh=(hsh*HASH_BASE+d)%HASH_MOD;
    hsh=(hsh*HASH_BASE+chunkSize)%HASH_MOD;
    hsh=(hsh*HASH_BASE+seed)%HASH_MOD;
    return hsh;
}
void get_input_data()
{
    in>>n>>d>>chunkSize>>seed;
    data.resize(n);
    srand(seed);
    for (int i=0;i<n;++i)
    {
        data[i]=rand()%2;
    }
}
void get_output_data()
{
    long long id;
    out>>id;
    invalid=0;
    if (id!=hash_input_with(0))
    {
        invalid=-1;
        return;
    }
    int n2;
    out>>n2>>m2;
    if (n2==-1)
    {
        invalid=-3;
        return;
    }
    if (n2!=n)
    {
        invalid=-2;
        return;
    }
    reconstructedData.resize(n);
    bool curr;
    for (int i=0;i<n;++i)
    {
        out>>curr;
        reconstructedData[i]=curr;
    }
}
int correct=0;
void get_scores()
{
    author>>authorScore;
    for (int i=0;i<n;++i)
    {
        if (reconstructedData[i]==data[i]) ++correct;
    }
    score=std::max(2*correct-n,0);
    score/=std::max(m2,n);
}
void get_result()
{
    if (invalid==0)
    {
        std::cout<<std::setprecision(12)<<std::min(score/authorScore,1.0)<<"\n";
        std::cerr<<"Result: "<<std::min(score/authorScore,1.0)<<"\n";
        return;
    }
    if (invalid==-1)
    {
        std::cout<<"0\n";
        std::cerr<<"Your program printed to stdout, it shouldn't.\n";
        return;
    }
    if (invalid==-2)
    {
        std::cout<<"0\n";
        std::cerr<<"Your receiver returned too few/many bits.\n";
        return;
    }
    if (invalid==-3)
    {
        std::cout<<"0\n";
        std::cerr<<"Your transmitter returned too many bits.\n";
        return;
    }
    std::cout<<"0\n";
    std::cerr<<"Unknown error.\n";
}
int main(int argc, char *argv[])
{
    in.open(argv[1]);
    author.open(argv[2]);
    out.open(argv[3]);
    if (!in)
    {
        std::cout<<"0\n";
        std::cerr<<"In-file "<<argv[1]<<" not found.\n";
        return -1;
    }
    if (!author)
    {
        std::cout<<"0\n";
        std::cerr<<"Author-file "<<argv[2]<<" not found.\n";
        return -2;
    }
    if (!out)
    {
        std::cout<<"0\n";
        std::cerr<<"Out-file "<<argv[3]<<" not found.\n";
        return -3;
    }
    get_input_data();
    get_output_data();
    if (!invalid) get_scores();
    get_result();
    return 0;
}
