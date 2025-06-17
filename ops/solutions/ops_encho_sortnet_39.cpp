#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include "ops.h"
using namespace std;
typedef long long llong;

const llong MAXVAL = 1000000000000000000LL;

///
vector<pair<int, int>> getSwapsEmo(int n)
{
    //printf("Getting swaps for n = %d\n", n);

    vector<pair<int, int>> ans;

    for (int block = 1; block < n; block *= 2)
    {
        for (int gap = block; gap >= 1; gap /= 2)
        {
            for (int j = gap % block; j <= n - gap - 1; j += 2 * gap)
            {
                for (int i = 0; i <= min(gap - 1, n - gap - j - 1); i++)
                {
                    if ((i + j) / (2 * block) < (i + j + gap) / (2 * block))
                        continue;
                    ans.push_back({i + j, i + j + gap});
                }
            }
        }
    }

    return ans;
}
///

template<typename F>
TableT op(F f)
{
    TableT r;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            r[i][j] = f(i, j);
        }
    }
    return r;
}

TableT copyOp;
void prepareOps()
{
    copyOp = op([](int x, int y){return x;});
}

int N;
vector<int> gaps;
vector<int> positions;
int EXPANDED_BYTES = 10;
int BITS_PER_BYTE = 6;

int getReachableGap(int A, int B)
{
    for (auto g : gaps)
    {
        if (abs(g - A) > MAX_OP_DIST)
            continue;
        if (abs(g + 1 - A) > MAX_OP_DIST)
            continue;
        if (abs(g - B) > MAX_OP_DIST)
            continue;
        if (abs(g + 1 - B) > MAX_OP_DIST)
            continue;
        return g;
    }
    assert(false);
}

void setVarLength()
{
    applyOp(0, 1, 6, op([](int g1, int g2){
        int nval = g2 * 256 + g1;
        return min(nval, 255);
    }));
    applyOp(0, 1, 7, op([](int g1, int g2){
        int nval = g2 * 256 + g1;
        return max(nval - 255, 0);
    }));

    assert(gaps[0] == 6);
    for (int i = 1; i < gaps.size(); i++)
    {
        applyOp(gaps[i - 1], gaps[i - 1], gaps[i], copyOp);
        applyOp(gaps[i - 1] + 1, gaps[i - 1] + 1, gaps[i] + 1, copyOp);
    }

    assert(BITS_PER_BYTE == 6);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < EXPANDED_BYTES; j++)
        {
            int idx = positions[i] + j;
            int g = getReachableGap(idx, idx);

            int effIdx = i + 1;
            if ( effIdx > 255 )
            {
                effIdx -= 255;
                g++;
            }

            applyOp(g, idx, idx, op([&](int nVal, int oldVal){
                if ( effIdx > nVal )
                    return 63;
                else
                    return oldVal;
            }));
        }
    }
}

void applyOpFar(int input1, int input2, int output, const TableT& opTable)
{
    assert(input1 <= input2);
    assert(input2 <= output);
    if (output - input1 <= MAX_OP_DIST)
    {
        applyOp(input1, input2, output, opTable);
        return;
    }

    applyOp(input1, input2, input1 + MAX_OP_DIST, opTable);

    int cur = input1 + MAX_OP_DIST;
    while(output - cur > MAX_OP_DIST)
    {
        applyOp(cur, cur, cur + MAX_OP_DIST, copyOp);
        cur += MAX_OP_DIST;
    }

    if (cur != output)
    {
        applyOp(cur, cur, output, copyOp);
    }
}

void applyOpFarBack(int input1, int input2, int output, const TableT& opTable)
{
    assert(input1 <= input2);
    assert(output <= input1);
    if (input2 - output <= MAX_OP_DIST)
    {
        applyOp(input1, input2, output, opTable);
        return;
    }

    applyOp(input1, input2, input2 - MAX_OP_DIST, opTable);

    int cur = input2 - MAX_OP_DIST;
    while(cur - output > MAX_OP_DIST)
    {
        applyOp(cur, cur, cur - MAX_OP_DIST, copyOp);
        cur -= MAX_OP_DIST;
    }

    if (cur != output)
    {
        applyOp(cur, cur, output, copyOp);
    }
}

void shiftExpanded()
{
    int curPos = 8;
    gaps.push_back(6);
    for (int i = 1; i <= N; i++)
    {
        if ( (curPos + EXPANDED_BYTES - 1) - gaps.back() + 2 > MAX_OP_DIST )
        {
            gaps.push_back(curPos);
            curPos += 2;
            i--;
        }
        else
        {
            positions.push_back(curPos);
            curPos += EXPANDED_BYTES;
        }
    }
    gaps.push_back(curPos);

    for (int i = 1; i < gaps.size(); i++)
    {
        assert(gaps[i] - gaps[i - 1] + 1 <= MAX_OP_DIST);
    }

    for (int i = N; i >= 1; i--)
    {
        //printf("Moving from %d to %d\n", i * 8, positions[i - 1]);

        int baseIdx = i * 8;
        int targetIdx = positions[i - 1] + EXPANDED_BYTES - 1;
        for (int j = 59; j > 0; j -= BITS_PER_BYTE)
        {
            int firstBit = max(0, j - BITS_PER_BYTE + 1);
            int lastBit = j;

            int inp1 = firstBit / 8;
            int inp2 = lastBit / 8;

            int L1 = firstBit - (inp1 * 8);
            int R1 = min(7, lastBit - (inp1 * 8));
            int LEN1 = R1 - L1 + 1;

            int L2 = max(0, firstBit - (inp2 * 8));
            int R2 = lastBit - (inp2 * 8);
            int LEN2 = R2 - L2 + 1;

            //printf("[%d; %d] from %d | [%d; %d] from %d\n", L1, R1, inp1, L2, R2, inp2);

            applyOpFar(baseIdx + inp1, baseIdx + inp2, targetIdx, op([&](int x, int y){
                int val1 = ((x >> L1) & ((1LL << LEN1) - 1));
                if (inp1 == inp2)
                    return val1;

                int val2 = ((y >> L2) & ((1LL << LEN2) - 1));
                return ((val2 << LEN1) | val1);
            }));

            targetIdx--;
        }
    }
}

void shiftContracted()
{
    for (int i = 1; i <= N; i++)
    {
        int baseIdx = positions[i - 1];
        int targetIdx = (i - 1) * 8;
        for (int j = 0; j < 60; j += 8)
        {
            int firstBit = j;
            int lastBit = min(59, j + 7);

            int inp1 = firstBit / BITS_PER_BYTE;
            int inp2 = lastBit / BITS_PER_BYTE;

            int L1 = firstBit - (inp1 * BITS_PER_BYTE);
            int R1 = min(5, lastBit - (inp1 * BITS_PER_BYTE));
            int LEN1 = R1 - L1 + 1;

            int L2 = max(0, firstBit - (inp2 * BITS_PER_BYTE));
            int R2 = lastBit - (inp2 * BITS_PER_BYTE);
            int LEN2 = R2 - L2 + 1;

            //printf("[%d; %d] from %d | [%d; %d] from %d\n", L1, R1, inp1, L2, R2, inp2);

            applyOpFarBack(baseIdx + inp1, baseIdx + inp2, targetIdx, op([&](int x, int y){
                int val1 = ((x >> L1) & ((1LL << LEN1) - 1));
                if (inp1 == inp2)
                    return val1;

                int val2 = ((y >> L2) & ((1LL << LEN2) - 1));
                return ((val2 << LEN1) | val1);
            }));

            targetIdx++;
        }
    }
}

const auto SWAP1 = op([](int x, int y){
            int cmpResult = x;
            if (x > y)
                cmpResult += (1 << 6);
            if (x == y)
                cmpResult += (1 << 7);
            return cmpResult;
        });
const auto SWAP2 = op([](int res, int acc){
            int cmpRes = (res >> 6);
            int cmpAcc = (acc >> 6);
            int finalCmp;
            if ( (cmpAcc & 2) > 0 )
                finalCmp = cmpRes;
            else
                finalCmp = cmpAcc;

            int finalVal = (res & ((1 << 6) - 1));
            finalVal += (finalCmp << 6);
            return finalVal;
        });
const auto SWAP3 = op([](int condA, int bVal){
        if ( (condA & (1 << 6)) > 0 )
            return (bVal & ((1 << 6) - 1));
        else
            return (condA & ((1 << 6) - 1));
    });
const auto SWAP4 = op([](int condA, int bVal){
        if ( (condA & (1 << 6)) == 0 )
            return (bVal & ((1 << 6) - 1));
        else
            return (condA & ((1 << 6) - 1));
    });

void compareAndSwapLong(int a, int b)
{
    assert(a < b);

    int leftMost = positions[a];
    int rightMost = positions[b] + EXPANDED_BYTES - 1;
    ///assert(rightMost - leftMost <= MAX_OP_DIST);

    int G1, G2;

    for (auto g : gaps)
    {
        if (g > leftMost)
        {
            G1 = g;
            break;
        }
    }
    for (auto g : gaps)
    {
        if (g < rightMost)
        {
            G2 = g;
        }
    }

    vector<int> allGaps;
    for (auto g : gaps)
    {
        if (g >= G1 && g <= G2)
        {
            allGaps.push_back(g);
        }
    }

    if (G1 > G2)
        G2 = G1;
    if (G1 == G2)
    {
        int G = G1;
        int A = positions[a];
        int B = positions[b];
        for (int i = EXPANDED_BYTES - 1; i >= 0; i--)
        {
            //printf("Byte #%d\n", i);
            int Abyte = A + i;
            int Bbyte = B + i;

            int outp = G;
            if (i != EXPANDED_BYTES - 1)
                outp = Abyte;

            applyOp(Abyte, Bbyte, outp, SWAP1);

            if (i != EXPANDED_BYTES - 1)
            {
                applyOp(Abyte, G, G, SWAP2);
            }

            applyOp(G, Bbyte, Abyte, SWAP3);
            applyOp(G, Bbyte, Bbyte, SWAP4);
        }
    }
    else
    {
        int G = G1;

        int A = positions[a];
        int B = positions[b];

        //printf("Swapping %d; %d\n", A, B);
        //printf("Gaps are at %d and %d\n", G1, G2);
        for (int i = EXPANDED_BYTES - 1; i >= 0; i--)
        {
            //printf("Byte #%d\n", i);

            int orgBbyte = B + i;
            int Abyte = A + i;
            int Bbyte = B + i;

            //printf("Copying %d -> %d\n", Bbyte, allGaps.back() + 1);
            applyOp(Bbyte, Bbyte, allGaps.back() + 1, copyOp);
            for (int j = (int)allGaps.size() - 2; j >= 0; j--)
            {
                //printf("Copying %d -> %d\n", allGaps[j + 1] + 1, allGaps[j] + 1);
                applyOp(allGaps[j + 1] + 1, allGaps[j + 1] + 1, allGaps[j] + 1, copyOp);
            }
            Bbyte = G + 1;

            //printf("Copied!\n");

            int outp = G;
            if (i != EXPANDED_BYTES - 1)
                outp = Abyte;

            applyOp(Abyte, Bbyte, outp, SWAP1);

            if (i != EXPANDED_BYTES - 1)
            {
                applyOp(Abyte, G, G, SWAP2);
            }

            applyOp(G, Bbyte, Abyte, SWAP3);

            for (int j = 1; j < allGaps.size(); j++)
            {
                applyOp(allGaps[j - 1], allGaps[j - 1], allGaps[j], copyOp);
            }
            applyOp(allGaps.back(), orgBbyte, orgBbyte, SWAP4);
        }
    }
}

void compareAndSwapNormal(int a, int b)
{
    assert(a < b);

    int leftMost = positions[a];
    int rightMost = positions[b] + EXPANDED_BYTES - 1;
    assert(rightMost - leftMost <= MAX_OP_DIST);

    int G = getReachableGap(leftMost, rightMost);

    int A = positions[a];
    int B = positions[b];
    for (int i = EXPANDED_BYTES - 1; i >= 0; i--)
    {
        //printf("Byte #%d\n", i);
        int Abyte = A + i;
        int Bbyte = B + i;

        int outp = G;
        if (i != EXPANDED_BYTES - 1)
            outp = Abyte;

        applyOp(Abyte, Bbyte, outp, SWAP1);

        if (i != EXPANDED_BYTES - 1)
        {
            applyOp(Abyte, G, G, SWAP2);
        }

        applyOp(G, Bbyte, Abyte, SWAP3);
        applyOp(G, Bbyte, Bbyte, SWAP4);
    }
}

void bubbleSort()
{
    for (int i = 1; i < N; i++)
    {
        for (int j = i; j >= 1; j--)
        {
            compareAndSwapNormal(j - 1, j);
        }
    }
}

void sortNumbers(int maxN)
{
    prepareOps();

    N = maxN;
    //printf("N = %d\n", N);
    vector<pair<int, int>> swaps = getSwapsEmo(N);

    shiftExpanded();
    setVarLength();

    //bubbleSort();

    //printf("%d swaps\n", swaps.size());
    for (auto [x, y] : swaps)
    {
        compareAndSwapLong(x, y);
    }

    shiftContracted();
}
