#include "ops.h"

#include <vector>
#include <map>

constexpr int MAX_NUMBER_BITS = 60;
constexpr long long MAX_NUMBER = (1ll << MAX_NUMBER_BITS) - 1;

constexpr int EXP_NUM_REG_BITS = NUM_REG_BITS - 2;
constexpr int EXP_NUM_REG_VALS = 1 << EXP_NUM_REG_BITS;
constexpr int EXP_NUM_REGS_IN_NUMBER = 1 + (MAX_NUMBER_BITS - 1) / EXP_NUM_REG_BITS;

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

std::vector<int> expUnexpPoses(EXP_NUM_REGS_IN_NUMBER, 0);
std::vector<TableT> expTables(EXP_NUM_REGS_IN_NUMBER, makeTable());
std::vector<int> unexpExpPoses(NUM_REGS_IN_NUMBER, 0);
std::vector<TableT> unexpTables(NUM_REGS_IN_NUMBER, makeTable());

TableT expCmpTable = makeTable();
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

    int currBits = 0;
    int unexpPos = 0;
    for (int pos = 0; pos < EXP_NUM_REGS_IN_NUMBER; pos++)
    {
        expUnexpPoses[pos] = unexpPos;
        int doneBits = std::min(NUM_REG_BITS - currBits, EXP_NUM_REG_BITS);
        int nextBits = EXP_NUM_REG_BITS - doneBits;
        for (int val = 0; val < NUM_REG_VALS; val++)
        {
            for (int val2 = 0; val2 < NUM_REG_VALS; val2++)
            {
                int expVal = (val >> currBits) & ((1 << doneBits) - 1);
                if (nextBits > 0 && unexpPos < NUM_REGS_IN_NUMBER - 1)
                {
                    expVal |= (val2 & ((1 << nextBits) - 1)) << doneBits;
                }

                expTables[pos][val][val2] = expVal;
            }
        }

        if (nextBits > 0)
        {
            currBits = nextBits;
            unexpPos++;
        }
        else
        {
            currBits = currBits + doneBits;
            if (currBits == NUM_REG_BITS)
            {
                currBits = 0;
                unexpPos++;
            }
        }
    }

    currBits = 0;
    int expPos = 0;
    for (int pos = 0; pos < NUM_REGS_IN_NUMBER; pos++)
    {
        unexpExpPoses[pos] = expPos;
        int doneBits = EXP_NUM_REG_BITS - currBits;
        int nextBits = NUM_REG_BITS - doneBits;
        for (int val = 0; val < EXP_NUM_REG_VALS; val++)
        {
            for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
            {
                int unexpVal = (val >> currBits) & ((1 << doneBits) - 1);
                if (nextBits > 0 && expPos < EXP_NUM_REGS_IN_NUMBER - 1)
                {
                    unexpVal |= (val2 & ((1 << nextBits) - 1)) << doneBits;
                }

                unexpTables[pos][val][val2] = unexpVal;
            }
        }
        currBits = nextBits;
        expPos++;
        if (currBits == EXP_NUM_REG_BITS)
        {
            currBits = 0;
            expPos++;
        }
    }

    int markLt = CMP_LT << EXP_NUM_REG_BITS;
    int markGt = CMP_GT << EXP_NUM_REG_BITS;

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
        {
            if (val < val2) expCmpTable[val][val2] = val | markLt;
            else if (val > val2) expCmpTable[val][val2] = val | markGt;
            else expCmpTable[val][val2] = val;

            expCmpTable[val | markLt][val2] = val | markLt;
            expCmpTable[val | markGt][val2] = val | markGt;
        }
    }

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
        {
            expCmpCarryTable[val][val2] = val2;
            expCmpCarryTable[val | markLt][val2] = val2 | markLt;
            expCmpCarryTable[val | markGt][val2] = val2 | markGt;
        }
    }

    for (int val = 0; val < EXP_NUM_REG_VALS; val++)
    {
        for (int val2 = 0; val2 < EXP_NUM_REG_VALS; val2++)
        {
            expCpy1IfTable[val][val2] = val2;
            expCpy1IfTable[val | markLt][val2] = val2;
            expCpy1IfTable[val | markGt][val2] = val;

            expCpy2IfTable[val][val2] = val;
            expCpy2IfTable[val | markLt][val2] = val;
            expCpy2IfTable[val | markGt][val2] = val2;
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

void doExpand(int in, int out)
{
    for (int i = EXP_NUM_REGS_IN_NUMBER - 1; i >= 0; i--)
    {
        int l = expUnexpPoses[i];
        int r = std::min(l + 1, NUM_REGS_IN_NUMBER - 1);
        applyOp(in + l, in + r, out + i, expTables[i]);
    }
}

void doUnexpand(int in, int out)
{
    for (int i = 0; i < NUM_REGS_IN_NUMBER; i++)
    {
        int l = unexpExpPoses[i];
        int r = std::min(l + 1, EXP_NUM_REGS_IN_NUMBER - 1);
        applyOp(in + l, in + r, out + i, unexpTables[i]);
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

const int TMP_SPACE = 1;

void doOrderPair(int regs1, int regs2, int tmpReg)
{
    for (int i = 0; i < EXP_NUM_REGS_IN_NUMBER; i++)
    {
        int idx = EXP_NUM_REGS_IN_NUMBER - i - 1;
        applyOp(regs1 + idx, regs2 + idx, tmpReg, expCmpTable);
        if (idx > 0) applyOp(tmpReg, regs1 + idx - 1, regs1 + idx - 1, expCmpCarryTable);
        applyOp(tmpReg, regs2 + idx, regs1 + idx, expCpy2IfTable);
        applyOp(tmpReg, regs2 + idx, regs2 + idx, expCpy1IfTable);
    }
}

void sortNumbers(int maxN)
{
    initTables();

    int currPos = 0;
    int maxNextTmpPos = 0;
    std::vector<int> poses;
    std::vector<int> tmpPoses;
    std::vector<int> nearestTmp;

    for (int i = 0; i <= maxN; i++)
    {
        if (tmpPoses.empty() || (maxNextTmpPos != -1 && currPos + EXP_NUM_REGS_IN_NUMBER > maxNextTmpPos))
        {
            tmpPoses.push_back(currPos);
            currPos += TMP_SPACE;
            maxNextTmpPos = -1;
        }

        poses.push_back(currPos);
        currPos += EXP_NUM_REGS_IN_NUMBER;

        int lastTmpPos = tmpPoses.back();

        if (maxNextTmpPos == -1 && currPos + EXP_NUM_REGS_IN_NUMBER - lastTmpPos > MAX_OP_DIST)
        {
            maxNextTmpPos = currPos - EXP_NUM_REGS_IN_NUMBER + MAX_OP_DIST - TMP_SPACE + 1;
        }

        if (maxNextTmpPos == -1) nearestTmp.push_back(tmpPoses.size() - 1);
        else nearestTmp.push_back(tmpPoses.size());
    }

    tmpPoses.push_back(currPos);
    currPos += TMP_SPACE;

    std::vector<int> nearestTmpPos;
    for (int i = 0; i <= maxN; i++)
    {
        nearestTmpPos.push_back(tmpPoses[nearestTmp[i]]);
    }

    for (int i = maxN; i >= 0; i--)
    {
        int oldP = i * NUM_REGS_IN_NUMBER;
        int newP = poses[i];

        doMove(oldP, newP);
    }

    for (int i = 0; i < maxN; i++)
    {
        int regs = poses[i + 1];
        int nRegs = poses[i];

        int cmpReg = nRegs + NUM_REGS_IN_NUMBER;

        doCmpVal(nRegs, i, cmpReg, maxN);

        doCpySetIf(regs, cmpReg, regs, MAX_NUMBER, {CMP_EQ, CMP_LT});

        doSwapDiff(regs, nRegs, maxN);
    }

    for (int i = 0; i < maxN; i++)
    {
        int regs = poses[i];

        doExpand(regs, regs);
    }

    for (int i = maxN; i >= 0; i--)
    {
        for (int j = 0; j < i - 1; j++)
        {
            int regs1 = poses[j];
            int regs2 = poses[j + 1];
            int tmpPos = nearestTmpPos[j];

            doOrderPair(regs1, regs2, tmpPos);
        }
    }

    for (int i = 0; i < maxN; i++)
    {
        int regs = poses[i];

        doUnexpand(regs, regs);
    }

    for (int i = 0; i < maxN; i++)
    {
        int oldP = poses[i];
        int newP = i * NUM_REGS_IN_NUMBER;

        doMove(oldP, newP);
    }
}
