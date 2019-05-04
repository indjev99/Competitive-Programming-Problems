#include <iostream>

#define START SYMBOLS
#define LEFT -1
#define STAY 0
#define RIGHT 1
#define HALT -100

const int MAX_STATES=20;
const int SYMBOLS=2;
int n;

struct Instruction
{
    int mov;
    bool cnt;
    int sym;
    int state;
};
Instruction instr[SYMBOLS+1][MAX_STATES];
int STATES;

void print_instruction(const Instruction& instr)
{
    switch (instr.mov)
    {
        case LEFT:  std::cout<<'L';
                    break;
        case STAY:  std::cout<<'S';
                    break;
        case RIGHT: std::cout<<'R';
                    break;
        case HALT:  std::cout<<'H';
                    break;
        default:    std::cerr<<"Unknown movement: "<<instr.mov<<'\n';
    }
    if (instr.cnt) std::cout<<'C';
    if (instr.mov!=HALT)
    {
        std::cout<<' ';
        if (instr.sym==START) std::cout<<'S';
        else std::cout<<instr.sym;
        std::cout<<' '<<instr.state;
    }
    std::cout<<'\n';
}
int to_symbol(int i)
{
    if (i==-1) return START;
    else return i;
}
void print_code()
{
    std::cout<<STATES<<'\n';
    for (int i=-1;i<SYMBOLS;++i)
    {
        for (int j=0;j<STATES;++j)
        {
            print_instruction(instr[to_symbol(i)][j]);
        }
    }
}
void init_code()
{
    STATES=MAX_STATES;
    for (int i=-1;i<SYMBOLS;++i)
    {
        for (int j=0;j<MAX_STATES;++j)
        {
            instr[to_symbol(i)][j]={HALT,false};
        }
    }
}
void generate_code()
{
    // MAX_N = 399
    int r=(n-1)/(STATES-1);
    if (r)
    {
        for (int i=0;i<=r;++i)
        {
            if (i<r)
            {
                if (!i) instr[START][i]={RIGHT,false,START,0};
                else instr[0][i-1]={RIGHT,false,1,i};
            }
            else
            {
                if (!i) instr[START][i]={STAY,false,START,1};
                else instr[0][i-1]={STAY,false,1,1};
            }
        }
        for (int i=1;i<STATES;++i)
        {
            if (i<STATES-1) instr[1][i]={STAY,true,1,i+1};
            else instr[1][i]={LEFT,true,1,1};
        }
    }
    else instr[START][0]={STAY,false,START,1};
    int l=n-r*(STATES-1);
    for (int i=0;i<l;++i)
    {
        if (i<l-1) instr[START][1+i]={STAY,true,START,1+i+1};
        else instr[START][1+i]={HALT,true};
    }
}
int main()
{
    std::cin>>n;
    init_code();
    generate_code();
    print_code();
    return 0;
}
