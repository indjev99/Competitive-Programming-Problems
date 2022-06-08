// Sample program that does nothing sensible but demonstrates how to use constexpr

template <uint N>
struct ZeroArray
{
    uint vals[N];

    constexpr ZeroArray(): vals()
    {
        for (uint j = 0; j < N; ++j)
        {
            vals[j] = 0;
        }
    }
};

struct InitializedArray
{
    uint vals[1000];

    constexpr InitializedArray(): vals()
    {
        for (uint j = 0; j < 1000; ++j)
        {
            vals[j] = j * 3;
        }
    }
};

constexpr InitializedArray globArr;

constexpr uint stateSize(uint n)
{
    return 2;
}

constexpr void processOne(uint n, uint i, uint curr, State& state)
{
    state[0] = curr;
    if (i % 3 != 0) state[1] += globArr.vals[state[0]];
}

constexpr uint getAnswer(uint n, const State& state)
{
    ZeroArray<50> arr;
    for (uint i = 0; i < n; ++i)
    {
        arr.vals[i % 50] += 2;
        arr.vals[(i + globArr.vals[i] + state[i % 2]) % 50] += arr.vals[i % 50];
    }
    return arr.vals[49];
}
