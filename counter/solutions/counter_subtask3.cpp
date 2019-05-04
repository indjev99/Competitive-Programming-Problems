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
    // MAX_N = 120
    int r=(n-1)/(STATES/2);
    if (r)
    {
        for (int i=0;i<r;++i)
        {
            if (!i) instr[START][i]={RIGHT,false,START,(i==r-1)?STATES/2:0};
            else instr[0][i-1]={RIGHT,false,0,(i==r-1)?STATES/2:i};
        }
        for (int i=STATES/2;i<STATES;++i)
        {
            if (i<STATES-1) instr[0][i]={STAY,true,0,i+1};
            else instr[0][i]={LEFT,true,0,STATES/2};
        }
    }
    else instr[START][0]={STAY,false,START,STATES/2};
    int l=n-r*(STATES/2);
    for (int i=0;i<l;++i)
    {
        if (i<l-1) instr[START][STATES/2+i]={STAY,true,START,STATES/2+i+1};
        else instr[START][STATES/2+i]={HALT,true};
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
