#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <map>

const int MAX_STACKS = 10;
const int MAX_ITERS = 1e7;

struct Edge
{
    int to;
    int len;
};

bool cmpEdges(const Edge& e1, const Edge& e2)
{
    return e1.len < e2.len;
}

struct Graph
{
    int n;
    int start, end;
    std::vector<std::vector<Edge>> edges;
};

void fail()
{
    exit(0);
}

void graphAssert(bool f)
{
    if (!f)
    {
        std::cerr << "Inavlid graph" << std::endl;
        fail();
    }
}

void nodeCorrect(int a, int n)
{
    graphAssert(a >= 0 && a < n);
}

Graph readGraph(std::istream& in)
{
    Graph graph;
    int m, a, b, len;
    in >> graph.n >> m;
    in >> graph.start >> graph.end;
    nodeCorrect(graph.start, graph.n);
    nodeCorrect(graph.end, graph.n);
    graph.edges.resize(graph.n);
    for (int i = 0; i < m; ++i)
    {
        in >> a >> b >> len;
        nodeCorrect(a, graph.n);
        nodeCorrect(b, graph.n);
        graphAssert(len == 0 || len == 1);
        graph.edges[a].push_back({b, len});
    }
    for (int i = 0; i < graph.n; ++i)
    {
        std::sort(graph.edges[i].begin(), graph.edges[i].end(), cmpEdges);
    }
    return graph;
}

struct Code
{
    std::vector<std::string> words;

    void debugPrint() const
    {
        std::cerr << "\nCode:" << std::endl;
        for (const std::string& word : words)
        {
            std::cerr << word << std::endl;
        }
    }
};

#define COM_MARK "#"

Code readProgram(std::istream& in)
{
    Code code;
    std::string word;
    bool comment = false;
    while (true)
    {
        if (in.eof()) break;
        word = "";
        in >> word;
        if (word == "") continue;
        if (word == COM_MARK)
        {
            comment = !comment;
            continue;
        }
        if (!comment) code.words.push_back(word);
    }
    if (comment)
    {
        std::cerr << "Unclosed comment" << std::endl;
        fail();
    }
    return code;
}

#define T_INT 1
#define T_STACK 2
#define T_LABEL 3
#define T_RAND 4
#define T_CUSTOM 5

#define ALLOC -1
#define POP -2
#define PUSH -3
#define ADD -4
#define SUB -5
#define PRINT -6
#define RETURN -7
#define GETSTART -8
#define GETEND -9
#define GETEDGES -10
#define LABEL -11
#define GOTO -12
#define IF -13

#define EMPTY -101
#define NOTEMPTY -102
#define EQUAL -103
#define NOTEQUAL -104

std::map<std::string, int> opMap = {
    {"alloc", ALLOC},
    {"pop", POP},
    {"push", PUSH},
    {"add", ADD},
    {"sub", SUB},
    {"print", PRINT},
    {"return", RETURN},
    {"getStart", GETSTART},
    {"getEnd", GETEND},
    {"getEdges", GETEDGES},
    {"label", LABEL},
    {"goto", GOTO},
    {"if", IF},
    {"empty", EMPTY},
    {"notEmpty", NOTEMPTY},
    {"equal", EQUAL},
    {"notEqual", NOTEQUAL}
};

std::map<int, std::string> invOpMap = {
    {ALLOC, "alloc"},
    {POP, "pop"},
    {PUSH, "push"},
    {ADD, "add"},
    {SUB, "sub"},
    {PRINT, "print"},
    {RETURN, "return"},
    {GETSTART, "getStart"},
    {GETEND, "getEnd"},
    {GETEDGES, "getEdges"},
    {LABEL, "label"},
    {GOTO, "goto"},
    {IF, "if"},
    {EMPTY, "empty"},
    {NOTEMPTY, "notEmpty"},
    {EQUAL, "equal"},
    {NOTEQUAL, "notEqual"}
};

struct Token
{
    int type;

    int intVal;
    std::string word;

    void debugPrint() const
    {
        if (type == T_INT) std::cerr << intVal;
        else if (type == T_CUSTOM) std::cerr << "\"" << word << "\"";
        else std::cerr << invOpMap[type];
        std::cerr << std::endl;
    }
};

struct TokenStream
{
    std::vector<Token> tokens;

    void debugPrint() const
    {
        std::cerr << "\nTokens:" << std::endl;
        for (const Token& token : tokens)
        {
            token.debugPrint();
        }
    }
};

bool isNumber(const std::string& word)
{
    if (word == "") return false;
    bool first = true;
    for (const char c: word)
    {
        if (!isdigit(c) && (!first || c != '-')) return false;
        first = false;
    }
    return true;
}

bool isValidName(const std::string& word)
{
    if (word == "") return false;
    bool first = true;
    for (const char c: word)
    {
        if (!isalpha(c) && c != '_' && (first || !isdigit(c))) return false;
        first = false;
    }
    return true;
}

Token lexWord(const std::string& word)
{
    Token token;
    token.word = word;
    if (opMap.find(word) != opMap.end())
    {
        token.type = opMap[word];
    }
    else if (isNumber(word))
    {
        token.type = T_INT;
        token.intVal = std::stoi(word);
    }
    else if (isValidName(word))
    {
        token.type = T_CUSTOM;
    }
    else 
    {
        std::cerr << "Invalid token: " << word << std::endl;
        fail();
    }
    return token;
}

TokenStream lexProgram(const Code& code)
{
    TokenStream tokenStream;
    for (const std::string& word : code.words)
    {
        tokenStream.tokens.push_back(lexWord(word));
    }
    return tokenStream;
}

struct Arg
{
    int type;

    int intVal;
    std::string customName;

    void debugPrint() const
    {
        if (type == T_INT) std::cerr << intVal;
        else if (type == T_STACK) std::cerr << customName;
        else if (type == T_LABEL) std::cerr << "@" << customName;
        else std::cerr << invOpMap[type];
    }
};

struct Instr
{
    int type;
    std::vector<Arg> args;

    void debugPrint() const
    {
        std::cerr << invOpMap[type];
        for (const Arg& arg : args)
        {
            std::cerr << " ";
            arg.debugPrint();
        }
        std::cerr << std::endl;
    }
};

struct InstrStream
{
    std::vector<Instr> instrs;

    void debugPrint() const
    {
        std::cerr << "\nInstructions:" << std::endl;
        for (const Instr& instr : instrs)
        {
            instr.debugPrint();
        }
    }
};

bool isInstr(int type)
{
    return -100 < type && type < 0;
}

bool isCond(int type)
{
    return -200 < type && type < -100;
}

std::map<int, std::vector<int>> argTypes = {
    {ALLOC, {}},
    {POP, {T_STACK}},
    {PUSH, {T_STACK, T_RAND}},
    {ADD, {T_STACK, T_RAND}},
    {SUB, {T_STACK, T_RAND}},
    {PRINT, {T_RAND}},
    {RETURN, {T_RAND}},
    {GETSTART, {T_STACK}},
    {GETEND, {T_STACK}},
    {GETEDGES, {T_STACK, T_RAND}},
    {LABEL, {T_LABEL}},
    {GOTO, {T_LABEL}},
    {IF, {}},
    {EMPTY, {T_STACK}},
    {NOTEMPTY, {T_STACK}},
    {EQUAL, {T_RAND, T_RAND}},
    {NOTEQUAL, {T_RAND, T_RAND}}
};

typedef std::vector<Token>::const_iterator TokenIter;

Token getToken(TokenIter& curr, TokenIter end)
{
    if (curr == end)
    {
        std::cerr << "Reached end of file while parsing" << std::endl;
        fail();
    }
    return *(curr++);
}

bool isOfType(int type, int target)
{
    if (type != T_INT && type != T_CUSTOM) return false;
    if (target == T_RAND) return true;
    return (target == T_INT) == (type == T_INT);
}

Arg parseArg(int type, TokenIter& curr, TokenIter end)
{
    Arg arg;
    Token token = getToken(curr, end);
    if (!isOfType(token.type, type))
    {
        if (type == T_INT) std::cerr << "Invalid int: ";
        else if (type == T_STACK) std::cerr << "Invalid stack: ";
        else if (type == T_LABEL) std::cerr << "Invalid label: ";
        else if (type == T_RAND) std::cerr << "Invalid rand: ";
        std::cerr << token.word << std::endl;
        fail();
    }

    if (type != T_RAND) arg.type = type;
    else arg.type = token.type == T_INT ? T_INT : T_STACK;

    if (arg.type == T_INT) arg.intVal = token.intVal;
    else arg.customName = token.word;

    return arg;
}

Instr parseInstr(bool cond, TokenIter& curr, TokenIter end)
{
    Instr instr;
    Token token = getToken(curr, end);
    if ((!cond && !isInstr(token.type)) || (cond && !isCond(token.type)))
    {
        if (cond) std::cerr << "Invalid condition: ";
        else std::cerr << "Invalid instruction: ";
        std::cerr << token.word << std::endl;
        fail();
    }
    instr.type = token.type;

    if (instr.type == ALLOC)
    {
        int n = parseArg(T_INT, curr, end).intVal;
        if (n < 0)
        {
            std::cerr << "Negative number of stacks allocated: " << n << std::endl;
            fail();
        }
        for (int i = 0; i < n; ++i)
        {
            instr.args.push_back(parseArg(T_STACK, curr, end));
        }
    }
    else if (instr.type == IF)
    {
        Instr cond = parseInstr(true, curr, end);
        instr.args.push_back({cond.type});
        for (const Arg& condArg : cond.args)
        {
            instr.args.push_back(condArg);
        }
        Token gotoToken = getToken(curr, end);
        if (gotoToken.type != GOTO)
        {
            std::cerr << "No goto after if: " << gotoToken.word << std::endl;
            fail();
        }
        instr.args.push_back(parseArg(T_LABEL, curr, end));
    }
    else
    {
        for (int type : argTypes[instr.type])
        {
            instr.args.push_back(parseArg(type, curr, end));
        }
    }

    return instr;
}

InstrStream parseProgram(const TokenStream& tokenStream)
{
    InstrStream instrStream;
    TokenIter curr = tokenStream.tokens.begin();
    TokenIter end = tokenStream.tokens.end();
    while (curr != end)
    {
        instrStream.instrs.push_back(parseInstr(false, curr, end));
    }
    return instrStream;
}

struct Program
{
    int startPC;
    int numStacks;
    std::vector<int> data;

    void debugPrint() const
    {
        std::cerr << "\nProgram:" << std::endl;
        std::cerr << "Stacks: " << numStacks << std::endl;
        std::cerr << "Start PC: " << startPC << std::endl;
        for (int addr = 0; addr < data.size(); ++addr)
        {
            std::cerr << addr << ": " << data[addr] << std::endl;
        }
    }
};

Program linkProgram(const InstrStream& instrStream)
{
    Program program;
    std::map<std::string, int> stackCodes;
    std::map<std::string, int> labelRelAddr;
    std::map<int, int> constantPos;

    int currAddr = 0;
    program.numStacks = 0;
    for (const Instr& instr : instrStream.instrs)
    {
        if (instr.type == ALLOC)
        {
            for (const Arg& arg : instr.args)
            {
                if (stackCodes.find(arg.customName) != stackCodes.end())
                {
                    std::cerr << "Dupplicate stacks: " << arg.customName << std::endl;
                    fail();
                }
                stackCodes[arg.customName] = program.numStacks++;
            }
        }
        else if (instr.type == LABEL)
        {
            const Arg& arg = instr.args[0];
            if (labelRelAddr.find(arg.customName) != labelRelAddr.end())
            {
                std::cerr << "Dupplicate labels: " << arg.customName << std::endl;
                fail();
            }
            labelRelAddr[arg.customName] = currAddr;
        }
        else
        {
            currAddr += 1 + instr.args.size();
            for (const Arg& arg : instr.args)
            {
                if (arg.type != T_INT) continue;
                if (constantPos.find(arg.intVal) == constantPos.end())
                {
                    constantPos[arg.intVal] = program.data.size();
                    program.data.push_back(arg.intVal);
                }
            }
        }
    }
    program.startPC = program.data.size();

    for (const Instr& instr : instrStream.instrs)
    {
        if (instr.type == ALLOC || instr.type == LABEL) continue;
        program.data.push_back(instr.type);
        for (const Arg& arg : instr.args)
        {
            if (arg.type == T_INT)
            {
                program.data.push_back(constantPos[arg.intVal] + program.numStacks);
            }
            else if (arg.type == T_STACK)
            {
                if (stackCodes.find(arg.customName) == stackCodes.end())
                {
                    std::cerr << "Undefined stack: " << arg.customName << std::endl;
                    fail();
                }
                program.data.push_back(stackCodes[arg.customName]);
            }
            else if (arg.type == T_LABEL)
            {
                if (labelRelAddr.find(arg.customName) == labelRelAddr.end())
                {
                    std::cerr << "Undefined label: " << arg.customName << std::endl;
                    fail();
                }
                program.data.push_back(labelRelAddr[arg.customName] + program.startPC);
            }
            else program.data.push_back(arg.type);
        }
    }

    return program;
}

typedef std::vector<std::stack<int>> Stacks;

void notEmpty(int arg, const Stacks& stacks)
{
    if (stacks[arg].empty())
    {
        std::cerr << "Program tried to access empty stack" << std::endl;
        fail();
    }
}

void validNode(int idx, const Graph& graph)
{
    if (idx < 0 || idx >= graph.n)
    {
        std::cerr << "Program tried to get edges of invalid node" << std::endl;
        fail();
    }
}

int getVal(int arg, const Program& program, const Stacks& stacks)
{
    if (arg >= program.numStacks) return program.data[arg - program.numStacks];
    notEmpty(arg, stacks);
    return stacks[arg].top();
}

std::pair<int, int> runProgram(const Program& program, const Graph& graph)
{
    if (program.numStacks > MAX_STACKS)
    {
        std::cerr << "Program tried to allocate too many stacks: " << program.numStacks << std::endl;
        fail();
    }

    unsigned int pc = program.startPC;
    Stacks stacks(program.numStacks);

    bool canGetStart = true;
    bool canGetEnd = true;
    std::vector<bool> canGetEdge(graph.n, true);

    int iters = 0;
    while (pc < program.data.size() && iters < MAX_ITERS)
    {
        ++iters;
        int type = program.data[pc++];

        bool sat;
        int cond, arg, other, idx;
        switch (type)
        {
        case POP:
            arg = program.data[pc++];
            notEmpty(arg, stacks);
            stacks[arg].pop();
            break;

        case PUSH:
            arg = program.data[pc++];
            other = program.data[pc++];
            stacks[arg].push(getVal(other, program, stacks));
            break;

        case ADD:
            arg = program.data[pc++];
            other = program.data[pc++];
            notEmpty(arg, stacks);
            stacks[arg].top() += getVal(other, program, stacks);
            break;

        case SUB:
            arg = program.data[pc++];
            other = program.data[pc++];
            notEmpty(arg, stacks);
            stacks[arg].top() -= getVal(other, program, stacks);
            break;

        case PRINT:
            arg = program.data[pc++];
            std::cerr << "> " << getVal(arg, program, stacks) << std::endl;
            break;

        case RETURN:
            arg = program.data[pc++];
            return {getVal(arg, program, stacks), iters};

        case GETSTART:
            arg = program.data[pc++];
            if (canGetStart)
            {
                stacks[arg].push(graph.start);
                canGetStart = false;
            }
            break;

        case GETEND:
            arg = program.data[pc++];
            if (canGetEnd)
            {
                stacks[arg].push(graph.end);
                canGetEnd = false;
            }
            break;

        case GETEDGES:
            arg = program.data[pc++];
            other = program.data[pc++];
            idx = getVal(other, program, stacks);
            validNode(idx, graph);
            if (canGetEdge[idx])
            {
                for (const Edge& edge : graph.edges[idx])
                {
                    stacks[arg].push(edge.to);
                    stacks[arg].push(edge.len);
                }
                canGetEdge[idx] = false;
            }
            break;

        case GOTO:
            pc = program.data[pc];
            break;

        case IF:
            sat = false;
            switch (program.data[pc++])
            {
            case EMPTY:
                arg = program.data[pc++];
                sat = stacks[arg].empty();
                break;
            case NOTEMPTY:
                arg = program.data[pc++];
                sat = !stacks[arg].empty();
                break;
            case EQUAL:
                arg = program.data[pc++];
                other = program.data[pc++];
                sat = getVal(arg, program, stacks) == getVal(other, program, stacks);
                break;
            case NOTEQUAL:
                arg = program.data[pc++];
                other = program.data[pc++];
                sat = getVal(arg, program, stacks) != getVal(other, program, stacks);
                break;
            }
            if (sat) pc = program.data[pc];
            else ++pc;
            break;
        }
    }

    if (iters == MAX_ITERS) return {0, -1};
    else return {0, -2};
}

int main()
{
    std::cerr << "Enter graph:" << std::endl;
    Graph graph = readGraph(std::cin);

    std::cerr << "\nEnter program:" << std::endl;
    Code code = readProgram(std::cin);
    //code.debugPrint();
    TokenStream tokenStream = lexProgram(code);
    //tokenStream.debugPrint();
    InstrStream instrStream = parseProgram(tokenStream);
    //instrStream.debugPrint();
    Program program = linkProgram(instrStream);
    //program.debugPrint();

    std::pair<int, int> result = runProgram(program, graph);

    if (result.second == -1)
    {
        std::cerr << "Program exceeded allowed number of iterations" << std::endl;
        fail();
    }
    else if (result.second == -2)
    {
        std::cerr << "Program counter went past end of program" << std::endl;
        fail();
    }

    std::cout << "Program returned " << result.first << " in " << result.second << " iterations" << std::endl;

    return 0;
}
