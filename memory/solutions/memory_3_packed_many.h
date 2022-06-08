constexpr uint CNT_BITS = 15;
constexpr uint PREV_BITS = 10;
constexpr uint WORD_BITS = 32;
constexpr uint TOTAL_BITS = MAX_STATE_SIZE * WORD_BITS;
constexpr uint NUM_PREVS = (TOTAL_BITS - CNT_BITS) / PREV_BITS;

constexpr uint getNum(uint startBit, uint endBit, const State& state)
{
    uint word1 = startBit / WORD_BITS;
    uint word2 = endBit / WORD_BITS;
    uint offset1 = startBit % WORD_BITS;
    uint offset2 = endBit % WORD_BITS;

    if (word1 == word2) return (state[word1] >> offset1) & ((1u << (offset2 - offset1)) - 1);
    else return (state[word1] >> offset1) | ((state[word2] & ((1u << offset2) - 1)) << (WORD_BITS - offset1)); 
}

constexpr void setNum(uint startBit, uint endBit, uint num, State& state)
{
    uint word1 = startBit / WORD_BITS;
    uint word2 = endBit / WORD_BITS;
    uint offset1 = startBit % WORD_BITS;
    uint offset2 = endBit % WORD_BITS;

    if (word1 == word2)
    {
        state[word1] = (state[word1] & ~(((1u << (offset2 - offset1)) - 1) << offset1)) | (num << offset1);
    }
    else
    {
        state[word1] = (state[word1] & ~(((1u << (WORD_BITS - offset1)) - 1) << offset1)) | (num << offset1);
        state[word2] = ((state[word2] >> offset2) << offset2) | (num >> (WORD_BITS - offset1));
    }
}

constexpr uint stateSize(uint n)
{
    return MAX_STATE_SIZE;
}

constexpr void processOne(uint n, uint i, uint curr, State& state)
{
    if (i == 0)
    {
        setNum(0, CNT_BITS, 0, state);
        for (uint j = 0; j < NUM_PREVS; ++j)
        {
            setNum(CNT_BITS + j * PREV_BITS, CNT_BITS + (j + 1) * PREV_BITS, n, state);
        }
    }

    uint cnt = getNum(0, CNT_BITS, state);
    for (uint j = 0; j < NUM_PREVS; ++j)
    {
        uint prev = getNum(CNT_BITS + j * PREV_BITS, CNT_BITS + (j + 1) * PREV_BITS, state);
        if (j > 0) setNum(CNT_BITS + (j - 1) * PREV_BITS, CNT_BITS + j * PREV_BITS, prev, state);
        if (curr == prev) ++cnt;
    }

    setNum(CNT_BITS + (NUM_PREVS - 1) * PREV_BITS, CNT_BITS + NUM_PREVS * PREV_BITS, curr, state);
    setNum(0, CNT_BITS, cnt, state);
}

constexpr uint getAnswer(uint n, const State& state)
{
    ull cnt = getNum(0, CNT_BITS, state);
    ull all = 0;
    for (uint j = 0; j < NUM_PREVS; ++j)
    {
        all += (n - j - 1);
    }
    return (cnt * n * (n - 1) + all / 2) / all + n;
}
