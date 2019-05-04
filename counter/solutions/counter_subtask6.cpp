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
    // MAX_N = 1024
    if (!n) return;
    const int BACK=STATES/2-1;
    const int FREE=STATES-1;
    const int n2=(n+1)/2-1;
    instr[START][0]={RIGHT,false,START,0};
    for (int i=0;i<BACK;++i)
    {
        instr[0][i]={RIGHT,false,(n2>>i)&1,i+1};
    }
    instr[0][BACK]={LEFT,false,0,BACK};
    instr[1][BACK]={LEFT,false,1,BACK};
    instr[START][BACK]={RIGHT,true,START,BACK+1};
    for (int i=BACK+1;i<FREE;++i)
    {
        if (i<FREE-1) instr[0][i]={RIGHT,false,1,i+1};
        else instr[0][i]={HALT,(n2+1)*2-1<n};
        instr[1][i]={LEFT,true,0,BACK};
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
