#include <iostream>
#include <algorithm>

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
const int uses[SYMBOLS+1][MAX_STATES]=
{{131054, 65536, 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 131072, 131071},
{0, 65536, 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 131071, 0},
{131072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 131072}};
struct InstructionWithUse
{
    int symbol;
    int state;
    int uses;
};
bool operator< (const InstructionWithUse& i1, const InstructionWithUse& i2)
{
    return i1.uses>i2.uses;
}
void assign_counts()
{
    const int m=(SYMBOLS+1)*MAX_STATES;
    InstructionWithUse iwu[m];
    int curr=0;
    for (int i=-1;i<SYMBOLS;++i)
    {
        for (int j=0;j<MAX_STATES;++j)
        {
            iwu[curr++]={to_symbol(i),j,uses[to_symbol(i)][j]};
        }
    }
    std::sort(iwu,iwu+m);
    int n2=n;
    for (int i=0;i<m;++i)
    {
        if (iwu[i].uses<=n2)
        {
            n2-=iwu[i].uses;
            instr[iwu[i].symbol][iwu[i].state].cnt=true;
        }
    }
}
void generate_skeleton()
{
    // TWOSTEP
    // MAX_N = 1048554
    const int BACK_CHECK=0;
    const int START_CHECK=1;
    const int BACK_ADD=STATES-1;
    const int FRWRD_ADD=STATES-2;
    instr[START][BACK_ADD]={RIGHT,false,START,START_CHECK};
    for (int i=START_CHECK;i<FRWRD_ADD;++i)
    {
        if (i<FRWRD_ADD-1)
        {
            instr[0][i]={RIGHT,false,0,i+1};
            instr[1][i]={LEFT,false,1,BACK_CHECK};
        }
        else
        {
            instr[0][i]={HALT,false};
            instr[1][i]={LEFT,false,1,BACK_CHECK};
        }
    }
    instr[0][BACK_CHECK]={LEFT,false,0,BACK_CHECK};
    instr[1][BACK_CHECK]={LEFT,false,1,BACK_CHECK};
    instr[START][BACK_CHECK]={RIGHT,false,START,FRWRD_ADD};
    instr[0][FRWRD_ADD]={LEFT,false,1,BACK_ADD};
    instr[1][FRWRD_ADD]={RIGHT,false,0,FRWRD_ADD};
    instr[0][BACK_ADD]={LEFT,false,0,BACK_ADD};
    instr[1][BACK_ADD]={LEFT,false,1,BACK_ADD};
}
void generate_code()
{
    generate_skeleton();
    assign_counts();
}
int main()
{
    std::cin>>n;
    init_code();
    generate_code();
    print_code();
    return 0;
}
