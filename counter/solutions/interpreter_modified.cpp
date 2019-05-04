#include <iostream>
#include <stdlib.h>

#define START SYMBOLS
#define LEFT -1
#define STAY 0
#define RIGHT 1
#define HALT -100
#define ERROR -99999
#define TOOMANY -88888
#define OUTOFBOUNDS -77777

const int MAX_STATES=20;
const int SYMBOLS=2;
const int MAX_ITER=3e7;
const int TAPE_LEN=MAX_ITER*2+1;
int STATES;
int pos,state,cnt,iter;
int tape[TAPE_LEN];

struct Instruction
{
    int mov;
    bool cnt;
    int sym;
    int state;
    std::string cmd;
    int uses;
};
Instruction instr[SYMBOLS+1][MAX_STATES];

bool is_number(const std::string& s)
{
    for (int i=0;i<s.size();++i)
    {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}
int to_symbol(int i)
{
    if (i==-1) return START;
    else return i;
}
void load()
{
    std::string mc;
    std::string sym;
    std::string state;
    int nSym;
    int nState;
    std::cin>>STATES;
    if (STATES<=0 || STATES>MAX_STATES) return;
    for (int i=-1;i<SYMBOLS;++i)
    {
        int s=to_symbol(i);
        for (int j=0;j<STATES;++j)
        {
            instr[s][j].uses=0;
            std::cin>>mc;
            instr[s][j].cmd=mc;
            switch (mc[0])
            {
                case 'L': instr[s][j].mov=LEFT;
                          break;
                case 'S': instr[s][j].mov=STAY;
                          break;
                case 'R': instr[s][j].mov=RIGHT;
                          break;
                case 'H': instr[s][j].mov=HALT;
                          break;
                default:  instr[s][j].mov=ERROR;
                          std::cerr<<"Warning: Invalid move: "<<mc[0]<<std::endl;
            }
            instr[s][j].cnt=false;
            if (mc.size()>1)
            {
                if (mc[1]=='C')
                {
                    instr[s][j].cnt=true;
                    if (mc.size()>2) std::cerr<<"Warning: Unnecessary characters for move-counter instruction: "<<mc.substr(2,mc.size()-2)<<std::endl;
                }
                else std::cerr<<"Warning: Unnecessary characters for move-counter instruction: "<<mc.substr(1,mc.size()-1)<<std::endl;
            }
            if (instr[s][j].mov!=HALT)
            {
                std::cin>>sym>>state;
                instr[s][j].cmd+=" "+sym+" "+state;
                if (is_number(sym)) nSym=atoi(sym.c_str());
                else nSym=-1;
                if (s==START && sym=="S")
                {
                    instr[s][j].sym=START;
                }
                else if (nSym>=0 && nSym<SYMBOLS)
                {
                    instr[s][j].sym=nSym;
                }
                else
                {
                    instr[s][j].sym=ERROR;
                    std::cerr<<"Warning: Invalid symbol: "<<sym<<std::endl;
                }
                if (is_number(state)) nState=atoi(state.c_str());
                else nState=-1;
                if (nState>=0 && nState<STATES)
                {
                    instr[s][j].state=nState;
                }
                else
                {
                    instr[s][j].state=ERROR;
                    std::cerr<<"Warning: Invalid state: "<<state<<std::endl;
                }
            }
            else
            {
                instr[s][j].mov=STAY;
                instr[s][j].sym=HALT;
                instr[s][j].state=HALT;
            }
        }
    }
}
void init()
{
    pos=TAPE_LEN/2;
    tape[pos]=START;
    state=0;
    cnt=0;
    iter=0;
}
void doIteration()
{
    if (iter==MAX_ITER)
    {
        state=TOOMANY;
        return;
    }
    Instruction& toDo=instr[tape[pos]][state];
    if (toDo.mov==ERROR || toDo.sym==ERROR || toDo.state==ERROR)
    {
        std::cerr<<"ERROR: You tried executing an invalid instruction: "<<toDo.cmd<<std::endl;
        state=ERROR;
        return;
    }
    if (toDo.cnt) ++cnt;
    state=toDo.state;
    tape[pos]=toDo.sym;
    pos+=toDo.mov;
    if (pos<0 || pos>=TAPE_LEN) state=OUTOFBOUNDS;
    ++toDo.uses;
    ++iter;
}
void simulate()
{
    while (state!=HALT && state!=ERROR && state!=TOOMANY && state!=OUTOFBOUNDS)
    {
        doIteration();
    }
}
int main()
{
    load();
    if (STATES>0 && STATES<=MAX_STATES)
    {
        init();
        simulate();
        if (state==HALT)
        {
            std::cout<<"Your program halted successfully with counter: "<<cnt<<std::endl;
            std::cout<<"Instructions executed: "<<iter<<std::endl;
            std::cout<<"{";
            for (int i=0;i<=SYMBOLS;++i)
            {
                if (i) std::cout<<",\n";
                std::cout<<"{";
                for (int j=0;j<STATES;++j)
                {
                    if (j) std::cout<<", ";
                    std::cout<<instr[i][j].uses;
                }
                std::cout<<"}";
            }
            std::cout<<"};\n";
        }
        else if (state==ERROR) std::cout<<"Your program tried executing an invalid instruction."<<std::endl;
        else if (state==TOOMANY) std::cout<<"Your program exceeded the number of allowed iterations."<<std::endl;
        else if (state==OUTOFBOUNDS) std::cout<<"Your program went out of bounds."<<std::endl;
        else std::cout<<"Unknown error."<<std::endl;
    }
    else
    {
        std::cout<<"Your program required and invalid number of states."<<std::endl;
    }
    return 0;
}
