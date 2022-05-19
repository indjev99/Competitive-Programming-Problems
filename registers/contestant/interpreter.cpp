#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <string.h>
#include <random>
#include <fstream>
#include <iomanip>
using namespace std;

void endWithError(string error)
{
    printf("%s\n", error.c_str());
    exit(0);
}

string joinCommand(vector<string> command)
{
    string fullCommand;

    for (int i = 0; i < command.size(); i++)
    {
        if (i != 0)
            fullCommand.push_back(' ');

        fullCommand += command[i];
    }

    return fullCommand;
}

struct instruction
{
    int cmd;
    int reg1, reg2, jlab;

    instruction(int c, int r): cmd(c), reg1(r) {}
    instruction(int c, int r1, int r2, int l): cmd(c), reg1(r1), reg2(r2), jlab(l) {}
};

bool coinFlip(mt19937 &rng)
{
    return rng() % 2 == 0;
}

map<string, int> registersNameMap;
map<string, int> labels;
int registerEnumerator = 0;

int registerValues[111];

int executeProgram(vector<instruction> &program, int X, int Y, mt19937 &rng)
{
    int executedLines = 0;
    int curInstruction = 0;

    memset(registerValues, 0, sizeof(registerValues));
    registerValues[0] = X;
    registerValues[1] = Y;

    while(curInstruction < program.size())
    {
        instruction inst = program[curInstruction];

        if (inst.cmd == 2)
        {
            executedLines++;
            if (registerValues[inst.reg1] == registerValues[inst.reg2])
                curInstruction = inst.jlab;
            else
                curInstruction++;
        }
        else
        {
            if (coinFlip(rng))
            {
                executedLines++;
                if (inst.cmd == 0)
                    registerValues[inst.reg1]++;
                else
                    registerValues[inst.reg1] = max(0, registerValues[inst.reg1] - 1);
            }

            curInstruction++;
        }

        if (executedLines > 100000000)
            endWithError("Your program executed more than 100 000 000 instructions");
    }

    return executedLines;
}

vector<instruction> compile(vector<vector<string>> &program)
{
    if (program.empty())
        endWithError("Program cannot be empty");

    // Parse declared registers
    vector<string> firstCommand = program[0];
    if (firstCommand[0] != "registers")
        endWithError("Invalid first command - the first command must declare the registers used");
    if (firstCommand.size() > 13)
        endWithError("You cannot declare more than 12 registers");

    registerEnumerator = 0;
    registersNameMap.clear();

    registersNameMap.insert(make_pair("X", registerEnumerator));
    registerEnumerator++;
    registersNameMap.insert(make_pair("Y", registerEnumerator));
    registerEnumerator++;
    registersNameMap.insert(make_pair("Out", registerEnumerator));
    registerEnumerator++;
    for (int i = 1; i < firstCommand.size(); i++)
    {
        if (firstCommand[i] == "X" || firstCommand[i] == "Y" || firstCommand[i] == "Out")
            endWithError("You cannot declare register \'" + firstCommand[i] + "\'");
        if (registersNameMap.find(firstCommand[i]) != registersNameMap.end())
            endWithError("Register \'" + firstCommand[i] + "\' declared more than once");

        registersNameMap.insert(make_pair(firstCommand[i], registerEnumerator));
        registerEnumerator++;
    }

    // Compile labels
    int lineJump = 0;
    for (int i = 1; i < program.size(); i++)
    {
        vector<string> command = program[i];

        if (command[0] != "inc" && command[0] != "dec" && command[0] != "jeq")
        {
            if (command.size() != 1)
                endWithError("Invalid command \'" + joinCommand(command) + "\'");

            string labelName = command[0];
            if (labelName[ labelName.length()-1 ] == ':')
                labelName.pop_back();
            else
                endWithError("Invalid label \'" + labelName + "\'");

            if (labels.find(labelName) != labels.end())
                endWithError("Label \'" + labelName + "\' declared more than once");

            labels.insert(make_pair(labelName, lineJump));
        }
        else
            lineJump++;
    }

    // Parse actions
    vector<instruction> compiledCode;
    for (int i = 1; i < program.size(); i++)
    {
        vector<string> command = program[i];

        if (command[0] == "inc" || command[0] == "dec")
        {
            if (command.size() != 2)
                endWithError("Invalid command \'" + joinCommand(command) + "\'");

            auto reg = registersNameMap.find(command[1]);
            if (reg == registersNameMap.end())
                endWithError("Undeclared register \'" + command[1] + "\' in command \'" + joinCommand(command) + "\'");

            if (command[0] == "inc")
                compiledCode.push_back(instruction(0, (*reg).second));
            else
                compiledCode.push_back(instruction(1, (*reg).second));
        }
        else if (command[0] == "jeq")
        {
            if (command.size() != 4)
                endWithError("Invalid command \'" + joinCommand(command) + "\'");

            auto reg1 = registersNameMap.find(command[1]);
            if (reg1 == registersNameMap.end())
                endWithError("Undeclared register \'" + command[1] + "\' in command \'" + joinCommand(command) + "\'");

            auto reg2 = registersNameMap.find(command[2]);
            if (reg2 == registersNameMap.end())
                endWithError("Undeclared register \'" + command[2] + "\' in command \'" + joinCommand(command) + "\'");

            auto lbl = labels.find(command[3]);
            if (lbl == labels.end())
                endWithError("Unknown label \'" + command[3] + "\' in command \'" + joinCommand(command) + "\'");

            compiledCode.push_back(instruction(2, (*reg1).second, (*reg2).second, (*lbl).second));
        }
    }

    return compiledCode;
}

vector<vector<string>> tokenizeCommands(string fullProgram)
{
    vector<vector<string>> commands;
    vector<string> tokens;
    string token;

    for (int i = 0; i < fullProgram.length(); i++)
    {
        if (isspace(fullProgram[i]))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }

            if (fullProgram[i] == 10 || fullProgram[i] == 13)
            {
                if (!tokens.empty())
                {
                    if (tokens[0][0] != '#')
                        commands.push_back(tokens);
                    tokens.clear();
                }
            }
        }
        else
            token.push_back(fullProgram[i]);
    }

    if (!token.empty())
        tokens.push_back(token);
    if (!tokens.empty())
        commands.push_back(tokens);

    return commands;
}

string readProgram()
{
    char ch;
    string fullCode;
    while(scanf("%c", &ch) == 1)
    {
        fullCode.push_back(ch);
    }
    return fullCode;
}

int main()
{
    const int seed = 1337;
    int X, Y;

    scanf("%d %d", &X, &Y);

    mt19937 mt(seed);
    string fullCode = readProgram();
    vector<vector<string>> program = tokenizeCommands(fullCode);
    vector<instruction> compiled = compile(program);
    int runtime = executeProgram(compiled, X, Y, mt);

    int expectedValue = X * Y;

    if (expectedValue != registerValues[2])
    {
        printf("Wrong answer. Register \'Out\' contains %d; expected %d\n", registerValues[2], expectedValue);
        endWithError("Your solution produced an incorrect result");
    }

    int denom = X * Y + X + Y + 1;
    double factor = (double)runtime / (double)denom;
    printf("Correct answer.\n%d executed operations (factor %.2f)\n", runtime, factor);

    return 0;
}
