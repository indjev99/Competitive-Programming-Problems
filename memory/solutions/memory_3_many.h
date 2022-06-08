constexpr uint NUM_PREVS = MAX_STATE_SIZE - 1;

constexpr uint stateSize(uint n)
{
    return MAX_STATE_SIZE;
}

constexpr void processOne(uint n, uint i, uint curr, State& state)
{
    if (i == 0)
    {
        state[0] = 0;
        for (uint j = 0; j < NUM_PREVS; ++j)
        {
            state[j + 1] = n;
        }
    }

    for (uint j = 0; j < NUM_PREVS; ++j)
    {
        if (curr == state[j + 1]) ++state[0];
        if (j > 0) state[j] = state[j + 1];
    }

    state[NUM_PREVS] = curr;
}

constexpr uint getAnswer(uint n, const State& state)
{
    ull all = 0;
    for (uint j = 0; j < NUM_PREVS; ++j)
    {
        all += (n - j - 1);
    }
    return ((ull) state[0] * n * (n - 1) + all / 2) / all + n;
}
