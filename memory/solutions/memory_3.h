constexpr uint stateSize(uint n)
{
    return 2;
}

constexpr void processOne(uint n, uint i, uint curr, State& state)
{
    if (i == 0) 
    {
        state[0] = 0;
        state[1] = n;
    }

    if (curr == state[1]) ++state[0];
    state[1] = curr;
}

constexpr uint getAnswer(uint n, const State& state)
{
    return state[0] * n + n;
}
