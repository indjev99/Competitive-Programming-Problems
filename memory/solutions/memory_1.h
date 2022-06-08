constexpr uint stateSize(uint n)
{
    return 1;
}

constexpr void processOne(uint n, uint i, uint curr, State& state)
{
    if (i == 0) state[0] = curr;
    else if (i == 1) state[0] = curr == state[0];
}

constexpr uint getAnswer(uint n, const State& state)
{
    return state[0] * n * (n - 1) + n;
}
