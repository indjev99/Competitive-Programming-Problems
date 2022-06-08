constexpr uint BITS = 16;
constexpr uint LOWER_MASK = (1 << BITS) - 1;

constexpr uint stateSize(uint n)
{
    return 1;
}

constexpr void processOne(uint n, uint i, uint curr, State& state)
{
    if (i == 0) state[0] = curr << BITS;
    if (curr == ((state[0] >> BITS) & LOWER_MASK)) ++state[0];
}

constexpr uint getAnswer(uint n, const State& state)
{
    return (state[0] & LOWER_MASK) * n;
}
