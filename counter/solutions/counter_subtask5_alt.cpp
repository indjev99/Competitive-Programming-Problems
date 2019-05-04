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
{{19, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
{190, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}};
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
    // MAX_N = 989
    const int BACK=0;
    const int MOVE=1;
    instr[START][BACK]={STAY,false,START,MOVE};
    for (int i=MOVE;i<STATES;++i)
    {
        if (i<STATES-1)
        {
            instr[0][i]={STAY,false,0,i+1};
            instr[1][i]={RIGHT,false,1,i+1};
            instr[START][i]={STAY,false,START,i+1};
        }
        else
        {
            instr[0][i]={RIGHT,false,1,BACK};
            instr[1][i]={HALT,false};
            instr[START][i]={RIGHT,false,START,MOVE};
        }
    }
    instr[0][BACK]={LEFT,false,0,BACK};
    instr[1][BACK]={LEFT,false,1,BACK};
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
