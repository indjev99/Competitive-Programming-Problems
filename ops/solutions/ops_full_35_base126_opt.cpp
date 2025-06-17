#include "ops.h"

#include <vector>
#include <map>

constexpr int MAX_NUMBER_BITS = 60;
constexpr long long MAX_NUMBER = (1ll << MAX_NUMBER_BITS) - 1;

constexpr int calcExpNumRegsInNumber(int base)
{
    int digs = 0;
    long long maxNum = 0;

    while (maxNum < MAX_NUMBER)
    {
        digs++;
        maxNum = (maxNum + 1) * base - 1;
    }

    return digs;
}

constexpr int EXP_NUM_REG_VALS = (NUM_REG_VALS - 3) / 2;
constexpr int EXP_NUM_REGS_IN_NUMBER = calcExpNumRegsInNumber(EXP_NUM_REG_VALS);

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

TableT divTable = makeTable();
TableT modTable = makeTable();
TableT div0Table = makeTable();
TableT mod0Table = makeTable();

TableT expDivTable = makeTable();
TableT expModTable = makeTable();
TableT expDiv0Table = makeTable();
TableT expMod0Table = makeTable();

TableT expCmpTable = makeTable();
TableT expCmp0Table = makeTable();
TableT expCmpCarryTable = makeTable();
TableT expCpy1IfTable = makeTable();
TableT expCpy2IfTable = makeTable();

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

    for (int val = 0; val < NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            divTable[val][val2] = (val2 * NUM_REG_VALS + val) / EXP_NUM_REG_VALS;
            modTable[val][val2] = (val2 * NUM_REG_VALS + val) % EXP_NUM_REG_VALS;
            div0Table[val][val2] = val / EXP_NUM_REG_VALS;
            mod0Table[val][val2] = val % EXP_NUM_REG_VALS;
        }
    }

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
        {
            expDivTable[val][val2] = (val2 * EXP_NUM_REG_VALS + val) / NUM_REG_VALS;
            expModTable[val][val2] = (val2 * EXP_NUM_REG_VALS + val) % NUM_REG_VALS;
            expDiv0Table[val][val2] = val / NUM_REG_VALS;
            expMod0Table[val][val2] = val % NUM_REG_VALS;
        }
    }

    int markLt = 0;
    int markGt = EXP_NUM_REG_VALS;
    int markEq = 2 * EXP_NUM_REG_VALS;
    int markEqLt = 2 * EXP_NUM_REG_VALS + 1;
    int markEqGt = 2 * EXP_NUM_REG_VALS + 2;

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
        {
            if (val < val2) expCmpTable[val][val2] = val + markLt;
            else if (val > val2) expCmpTable[val][val2] = val + markGt;
            else expCmpTable[val][val2] = markEq;
        }
    }

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
        {
            expCmpCarryTable[markEq][markEq] = markEq;
            expCmpCarryTable[markEq][val2 + markLt] = val2 + markLt;
            expCmpCarryTable[markEq][val2 + markGt] = val2 + markGt;
            expCmpCarryTable[markEqLt][markEq] = markEqLt;
            expCmpCarryTable[markEqLt][val2 + markLt] = val2 + markLt;
            expCmpCarryTable[markEqLt][val2 + markGt] = val2 + markLt;
            expCmpCarryTable[markEqGt][markEq] = markEqGt;
            expCmpCarryTable[markEqGt][val2 + markLt] = val2 + markGt;
            expCmpCarryTable[markEqGt][val2 + markGt] = val2 + markGt;
            expCmpCarryTable[val + markLt][markEq] = markEqLt;
            expCmpCarryTable[val + markLt][val2 + markLt] = val2 + markLt;
            expCmpCarryTable[val + markLt][val2 + markGt] = val2 + markLt;
            expCmpCarryTable[val + markGt][markEq] = markEqGt;
            expCmpCarryTable[val + markGt][val2 + markLt] = val2 + markGt;
            expCmpCarryTable[val + markGt][val2 + markGt] = val2 + markGt;
        }
    }

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
        {
            expCpy1IfTable[markEq][val2] = val2;
            expCpy1IfTable[markEqLt][val2] = val2;
            expCpy1IfTable[markEqGt][val2] = val2;
            expCpy1IfTable[val + markLt][val2] = val2;
            expCpy1IfTable[val + markGt][val2] = val;

            expCpy2IfTable[markEq][val2] = val2;
            expCpy2IfTable[markEqLt][val2] = val2;
            expCpy2IfTable[markEqGt][val2] = val2;
            expCpy2IfTable[val + markLt][val2] = val;
            expCpy2IfTable[val + markGt][val2] = val2;
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

void doDivInPlace(int in, int outMod, int tmp, int numDigs)
{
    for (int i = 0; i < numDigs; i++)
    {
        int idx = numDigs - i - 1;
        if (i > 0) applyOp(outMod, outMod, tmp, cpyTable);
        applyOp(in + idx, tmp, outMod, i > 0 ? modTable : mod0Table);
        applyOp(in + idx, tmp, in + idx, i > 0 ? divTable : div0Table);
    }
}

void doExpDivInPlace(int in, int outMod, int tmp, int numDigs)
{
    for (int i = 0; i < numDigs; i++)
    {
        int idx = numDigs - i - 1;
        if (i > 0) applyOp(outMod, outMod, tmp, cpyTable);
        applyOp(in + idx, tmp, outMod, i > 0 ? expModTable : expMod0Table);
        applyOp(in + idx, tmp, in + idx, i > 0 ? expDivTable : expDiv0Table);
    }
}

void doExpand(int in, int out, int tmp)
{
    long long maxNum = MAX_NUMBER;
    for (int i = 0; i < EXP_NUM_REGS_IN_NUMBER; i++)
    {
        int maxDigs = 0;
        long long num = maxNum;
        while (num)
        {
            maxDigs++;
            num /= NUM_REG_VALS;
        }

        doDivInPlace(in, out + i, tmp, maxDigs);
        maxNum /= EXP_NUM_REG_VALS;
    }
}

void doUnexpand(int in, int out, int tmp)
{
    long long maxNum = MAX_NUMBER;
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int maxDigs = 0;
        long long num = maxNum;
        while (num)
        {
            maxDigs++;
            num /= EXP_NUM_REG_VALS;
        }

        doExpDivInPlace(in, out + i, tmp, maxDigs);
        maxNum /= NUM_REG_VALS;
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

const int SPACE_PER_NUM = 2 * EXP_NUM_REGS_IN_NUMBER;

void doOrderPair(int regs1, int regs2)
{
    int tmpRegs = regs1 + EXP_NUM_REGS_IN_NUMBER;

    for (int i = 0; i < EXP_NUM_REGS_IN_NUMBER; i++)
    {
        int idx = EXP_NUM_REGS_IN_NUMBER - i - 1;
        applyOp(regs1 + idx, regs2 + idx, tmpRegs + idx, expCmpTable);
        if (i > 0) applyOp(tmpRegs + idx + 1, tmpRegs + idx, tmpRegs + idx, expCmpCarryTable);
    }

    for (int i = 0; i < EXP_NUM_REGS_IN_NUMBER; i++)
    {
        applyOp(tmpRegs + i, regs2 + i, regs1 + i, expCpy2IfTable);
    }

    for (int i = 0; i < EXP_NUM_REGS_IN_NUMBER; i++)
    {
        applyOp(tmpRegs + i, regs2 + i, regs2 + i, expCpy1IfTable);
    }
}

void sortNumbers(int maxN)
{
    initTables();

    for (int i = maxN - 1; i >= 0; i--)
    {
        int oldP = NUM_REGS_IN_NUMBER + i * NUM_REGS_IN_NUMBER;
        int newP = EXP_NUM_REGS_IN_NUMBER + i * SPACE_PER_NUM;

        doMove(oldP, newP);
    }

    for (int i = 0; i < maxN; i++)
    {
        int regs = EXP_NUM_REGS_IN_NUMBER + i * SPACE_PER_NUM;
        int nRegs = regs - EXP_NUM_REGS_IN_NUMBER;

        int cmpReg = regs + NUM_REGS_IN_NUMBER;

        doCmpVal(nRegs, i, cmpReg, maxN);

        doCpySetIf(regs, cmpReg, regs, MAX_NUMBER, {CMP_EQ, CMP_LT});

        doCpy(nRegs, nRegs + SPACE_PER_NUM, maxN);
    }

    for (int i = 0; i < maxN; i++)
    {
        int oldP = EXP_NUM_REGS_IN_NUMBER + i * SPACE_PER_NUM;
        int newP = i * SPACE_PER_NUM;

        doExpand(oldP, newP, oldP + NUM_REGS_IN_NUMBER);
    }

    for (int i = maxN; i >= 0; i--)
    {
        for (int j = 0; j < i - 1; j++)
        {
            int regs1 = j * SPACE_PER_NUM;
            int regs2 = (j + 1) * SPACE_PER_NUM;

            doOrderPair(regs1, regs2);
        }
    }

    for (int i = 0; i < maxN; i++)
    {
        int oldP = i * SPACE_PER_NUM;
        int newP = EXP_NUM_REGS_IN_NUMBER + i * SPACE_PER_NUM;

        doUnexpand(oldP, newP, newP + NUM_REGS_IN_NUMBER);
    }

    for (int i = 0; i < maxN; i++)
    {
        int oldP = EXP_NUM_REGS_IN_NUMBER + i * SPACE_PER_NUM;
        int newP = i * NUM_REGS_IN_NUMBER;

        doMove(oldP, newP);
    }
}
