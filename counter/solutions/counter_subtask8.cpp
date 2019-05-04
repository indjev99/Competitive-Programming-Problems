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
    // MAX_N = 131071
    const int BACK_ADD=0;
    const int START_CHECK=1;
    const int BACK_CHECK=STATES-1;
    const int FRWRD_ADD=STATES-2;
    instr[START][BACK_ADD]={RIGHT,false,START,START_CHECK};
    for (int i=START_CHECK;i<FRWRD_ADD;++i)
    {
        bool bit=(n>>i-START_CHECK)&1;
        if (i<FRWRD_ADD-1) instr[bit][i]={RIGHT,false,bit,i+1};
        else instr[bit][i]={HALT,false};
        instr[!bit][i]={LEFT,false,!bit,BACK_CHECK};
    }
    instr[0][BACK_CHECK]={LEFT,false,0,BACK_CHECK};
    instr[1][BACK_CHECK]={LEFT,false,1,BACK_CHECK};
    instr[START][BACK_CHECK]={RIGHT,false,START,FRWRD_ADD};
    instr[0][FRWRD_ADD]={LEFT,true,1,BACK_ADD};
    instr[1][FRWRD_ADD]={RIGHT,false,0,FRWRD_ADD};
    instr[0][BACK_ADD]={LEFT,false,0,BACK_ADD};
    instr[1][BACK_ADD]={LEFT,false,1,BACK_ADD};
}
int main()
{
    std::cin>>n;
    init_code();
    generate_code();
    print_code();
    return 0;
}
