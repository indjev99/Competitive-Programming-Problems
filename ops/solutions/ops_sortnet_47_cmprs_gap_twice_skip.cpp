#include "ops.h"

#include <vector>
#include <map>

constexpr int MAX_NUMBER_BITS = 60;
constexpr long long MAX_NUMBER = (1ll << MAX_NUMBER_BITS) - 1;

TableT makeTable()
{
    return TableT{0};
}

constexpr int CMP_EQ = 0;
constexpr int CMP_LT = 1;
constexpr int CMP_GT = 2;

TableT cpyTable = makeTable();
TableT orTable = makeTable();
TableT xorTable = makeTable();
std::vector<TableT> setTables(NUM_REG_VALS, makeTable());
std::map<std::pair<int, std::vector<int>>, TableT> cpySetIfTables;
TableT cmpTable = makeTable();
std::vector<TableT> cmpValTables(NUM_REG_VALS, makeTable());
std::vector<TableT> cmpVal0Tables(NUM_REG_VALS, makeTable());

void initTables()
{
    for (int val = 0; val < NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            cpyTable[val][val2] = val;
        }
    }

    for (int val = 0; val < NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            orTable[val][val2] = val == 0 ? val2 : val;
        }
    }

    for (int val = 0; val < NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            xorTable[val][val2] = val ^ val2;
        }
    }

    for (int sv = 0; sv < NUM_REG_VALS; sv++)
    {
        for (int val = 0; val < NUM_REG_VALS; val++)
        {
            for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
            {
                setTables[sv][val][val2] = sv;
            }
        }
    }

    for (int val = 0; val < NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            if (val < val2) cmpTable[val][val2] = CMP_LT;
            else if (val > val2) cmpTable[val][val2] = CMP_GT;
            else cmpTable[val][val2] = CMP_EQ;
        }
    }

    for (int cv = 0; cv < NUM_REG_VALS; cv++)
    {
        for (int val = 0; val < NUM_REG_VALS; val++)
        {
            if (val < cv) cmpValTables[cv][val][CMP_EQ] = CMP_LT;
            else if (val > cv) cmpValTables[cv][val][CMP_EQ] = CMP_GT;
            else cmpValTables[cv][val][CMP_EQ] = CMP_EQ;

            cmpValTables[cv][val][CMP_LT] = CMP_LT;
            cmpValTables[cv][val][CMP_GT] = CMP_GT;
        }
    }

    for (int cv = 0; cv < NUM_REG_VALS; cv++)
    {
        for (int val = 0; val < NUM_REG_VALS; val++)
        {
            for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
            {
                if (val < cv) cmpVal0Tables[cv][val][val2] = CMP_LT;
                else if (val > cv) cmpVal0Tables[cv][val][val2] = CMP_GT;
                else cmpVal0Tables[cv][val][val2] = CMP_EQ;
            }
        }
    }
}

const TableT& getCpySetIfTable(int setVal, const std::vector<int>& setConds)
{
    std::pair<int, std::vector<int>> key = {setVal, setConds};
    auto it = cpySetIfTables.find(key);
    if (it != cpySetIfTables.end()) return it->second;

    TableT table = makeTable();

    for (int val = 0; val < NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            table[val][val2] = val;
        }

        for (int val2 : setConds)
        {
            table[val][val2] = setVal;
        }
    }

    auto [it2, _] = cpySetIfTables.insert({key, std::move(table)});
    return it2->second;
}

void doCpy(int in, int out, long long maxVal = MAX_NUMBER)
{
    int maxRegs = 0;
    while (maxVal > 0)
    {
        maxVal /= NUM_REG_VALS;
        maxRegs++;
    }

    if ((out > in && (out - in) >= maxRegs) || (out < in && (in - out) < maxRegs))
    {
        for (int i = 0; i < maxRegs; i++)
        {
            applyOp(in + i, in + i, out + i, cpyTable);
        }
    }
    else
    {
        for (int i = maxRegs - 1; i >= 0; i--)
        {
            applyOp(in + i, in + i, out + i, cpyTable);
        }
    }
}

void doOr(int in1, int in2, int out)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        applyOp(in1 + i, in2 + i, out + i, orTable);
    }
}

void doXor(int in1, int in2, int out)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        applyOp(in1 + i, in2 + i, out + i, xorTable);
    }
}

void doSet(int out, long long val)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int curr = val % NUM_REG_VALS;
        val /= NUM_REG_VALS;

        applyOp(out + i, out + i, out + i, setTables[curr]);
    }
}

void doCpySetIf(int in, int inCond, int out, long long val, const std::vector<int>& setConds)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int curr = val % NUM_REG_VALS;
        val /= NUM_REG_VALS;

        const TableT& table = getCpySetIfTable(curr, setConds);

        applyOp(in + i, inCond, out + i, table);
    }
}

void doCmp(int in1, int in2, int cmpReg, int cmpTmpReg)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int idx = NUM_REGS_IN_NUMBER - i - 1;
        applyOp(in1 + idx, in2 + idx, i == 0 ? cmpReg : cmpTmpReg, cmpTable);
        if (i > 0) applyOp(cmpReg, cmpTmpReg, cmpReg, orTable);
    }
}

void doCmpVal(int in, long long val, int cmpReg, long long maxVal = MAX_NUMBER)
{
    int maxRegs = 0;
    while (maxVal > 0)
    {
        maxVal /= NUM_REG_VALS;
        maxRegs++;
    }

    std::vector<int> currs(maxRegs);
    for (int i = 0; i < maxRegs; i++)
    {
        currs[i] = val % NUM_REG_VALS;
        val /= NUM_REG_VALS;
    }

    for (int i = 0; i < maxRegs; i++)
    {
        int idx = maxRegs - i - 1;
        applyOp(in + idx, cmpReg, cmpReg, i > 0 ? cmpValTables[currs[idx]] : cmpVal0Tables[currs[idx]]);
    }
}

void doMove(int in, int out)
{
    while (in < out)
    {
        int next = std::min(out, in + MAX_OP_DIST);
        doCpy(in, next);
        in = next;
    }

    while (in > out)
    {
        int next = std::max(out, in - MAX_OP_DIST);
        doCpy(in, next);
        in = next;
    }
}

void doSwapDiff(int regs1, int regs2, long long maxVal = MAX_NUMBER)
{
    int maxRegs = 0;
    while (maxVal > 0)
    {
        maxVal /= NUM_REG_VALS;
        maxRegs++;
    }

    for (int i = 0; i < maxRegs; i++)
    {
        applyOp(regs1 + i, regs2 + i, regs2 + i, xorTable);
        applyOp(regs1 + i, regs2 + i, regs1 + i, xorTable);
        applyOp(regs1 + i, regs2 + i, regs2 + i, xorTable);
    }

    for (int i = maxRegs; i < NUM_REGS_IN_NUMBER; i++)
    {
        applyOp(regs1 + i, regs1 + i, regs2 + i, cpyTable);
    }
}

const int SPACE_PER_NUM = NUM_REGS_IN_NUMBER + 1;

void doOrderPair(int regs1, int regs2)
{
    int cmpReg = regs1 + NUM_REGS_IN_NUMBER;
    int tmpReg = regs1 - 1;

    const TableT& cpySetIfTable = getCpySetIfTable(0, {CMP_EQ, CMP_LT});

    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int idx = NUM_REGS_IN_NUMBER - i - 1;

        int reg1 = regs1 + idx;
        int reg2 = regs2 + idx;
        int origReg2 = reg2;

        while (reg2 - tmpReg > MAX_OP_DIST)
        {
            int next = reg2 / SPACE_PER_NUM * SPACE_PER_NUM - 1;

            while (next - tmpReg > MAX_OP_DIST && reg2 - (next - SPACE_PER_NUM) <= MAX_OP_DIST)
            {
                next -= SPACE_PER_NUM;
            }

            applyOp(reg2, reg2, next, cpyTable);

            reg2 = next;
        }

        applyOp(reg1, reg2, i == 0 ? cmpReg : tmpReg, cmpTable);
        if (i > 0) applyOp(cmpReg, tmpReg, cmpReg, orTable);

        while (reg2 < origReg2)
        {
            int next = std::min(reg2 + SPACE_PER_NUM, origReg2);

            while (next < origReg2 && std::min(next + SPACE_PER_NUM, origReg2) - reg2 <= MAX_OP_DIST)
            {
                next = std::min(next + SPACE_PER_NUM, origReg2);
            }

            applyOp(reg2, reg2, next, cpyTable);

            reg2 = next;
        }
    }

    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int idx = NUM_REGS_IN_NUMBER - i - 1;

        int reg1 = regs1 + idx;
        int reg2 = regs2 + idx;
        int origReg2 = reg2;

        while (reg2 - tmpReg > MAX_OP_DIST)
        {
            int next = reg2 / SPACE_PER_NUM * SPACE_PER_NUM - 1;

            while (next - tmpReg > MAX_OP_DIST && reg2 - (next - SPACE_PER_NUM) <= MAX_OP_DIST)
            {
                next -= SPACE_PER_NUM;
            }

            applyOp(reg2, reg2, next, cpyTable);

            reg2 = next;
        }

        applyOp(reg1, reg2, tmpReg, xorTable);
        applyOp(tmpReg, cmpReg, tmpReg, cpySetIfTable);
        applyOp(reg2, tmpReg, reg2, xorTable);
        applyOp(reg1, tmpReg, reg1, xorTable);

        while (reg2 < origReg2)
        {
            int next = std::min(reg2 + SPACE_PER_NUM, origReg2);

            while (next < origReg2 && std::min(next + SPACE_PER_NUM, origReg2) - reg2 <= MAX_OP_DIST)
            {
                next = std::min(next + SPACE_PER_NUM, origReg2);
            }

            applyOp(reg2, reg2, next, cpyTable);

            reg2 = next;
        }
    }
}

void sortNumbers(int maxN)
{
    initTables();

    for (int i = maxN; i >= 0; i--)
    {
        int oldP = i * NUM_REGS_IN_NUMBER;
        int newP = SPACE_PER_NUM + i * SPACE_PER_NUM;

        doMove(oldP, newP);
    }

    for (int i = 0; i < maxN; i++)
    {
        int regs = 2 * SPACE_PER_NUM + i * SPACE_PER_NUM;
        int nRegs = regs - SPACE_PER_NUM;

        int cmpReg = nRegs + NUM_REGS_IN_NUMBER;

        doCmpVal(nRegs, i, cmpReg, maxN);

        doCpySetIf(regs, cmpReg, regs, MAX_NUMBER, {CMP_EQ, CMP_LT});

        doSwapDiff(regs, nRegs, maxN);
    }

    for (int block = 1; block < maxN; block *= 2)
    {
        for (int gap = block; gap >= 1; gap /= 2)
        {
            for (int j = gap % block; j <= maxN - gap - 1; j += 2 * gap)
            {
                for (int i = 0; i <= std::min(gap - 1, maxN - gap - j - 1); i++)
                {
                    if ((i + j) / (2 * block) < (i + j + gap) / (2 * block)) continue;

                    int regs1 = SPACE_PER_NUM + (i + j) * SPACE_PER_NUM;
                    int regs2 = SPACE_PER_NUM + (i + j + gap) * SPACE_PER_NUM;

                    doOrderPair(regs1, regs2);
                }
            }
        }
    }

    for (int i = 0; i < maxN; i++)
    {
        int oldP = SPACE_PER_NUM + i * SPACE_PER_NUM;
        int newP = i * NUM_REGS_IN_NUMBER;

        doMove(oldP, newP);
    }
}
