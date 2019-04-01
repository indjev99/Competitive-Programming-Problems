#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;
int main(int argc, char *argv[])
{
    double res=0;
    double eps=0.011;
    double corr_ans,ans;

    ifstream correct_output;
    correct_output.open(argv[2]);

    ifstream output;
    output.open(argv[3]);

    if(output.is_open())
    {
        correct_output>>corr_ans;
        output>>ans;

        if(fabs(corr_ans-ans)<eps) res=1.0;
        else res=0.0;
    }
    cout<<res<<endl;
    correct_output.close();
    output.close();
    return 0;
}
