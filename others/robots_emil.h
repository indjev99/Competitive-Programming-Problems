#include <array>
#include <cassert>

#define SET_VAL 0
#define ADD_VAL 1

#define CONN_RES 1
#define DISC_RES 2

constexpr unsigned int MAX_N = 1000;

struct MemoryOperation
{
    bool type;
    unsigned int addr, val;
    constexpr MemoryOperation(bool type, unsigned int addr, unsigned int val) : type(type), addr(addr), val(val) {}
};

constexpr MemoryOperation makeMemOp(bool type, unsigned int* loc, unsigned int val, unsigned int* memory)
{
    return MemoryOperation(type, loc - memory, val);
}

constexpr unsigned int findRoot(unsigned int u, unsigned int* memory)
{
    unsigned int* pars = memory + 2;

    while (u != pars[u])
    {
        u = pars[u];
    }

    return u;
}

constexpr std::array<MemoryOperation, 5> work(
    unsigned int u,
    unsigned int* memory,
    unsigned int memorySize,
    unsigned int* edges,
    unsigned int numEdges
) {
    unsigned int n = memory[1];

    unsigned int* result = memory;
    unsigned int* globState = memory + 2;
    unsigned int* pars = memory + 2;
    unsigned int* sizes = pars + n;
    unsigned int* states = sizes + n;
    unsigned int* toWrite = states + n;

    std::array<MemoryOperation, 5> ret = {
        MemoryOperation(ADD_VAL, 0, 0),
        MemoryOperation(ADD_VAL, 0, 0),
        MemoryOperation(ADD_VAL, 0, 0),
        MemoryOperation(ADD_VAL, 0, 0),
        MemoryOperation(ADD_VAL, 0, 0)
    };

    ret[3] = MemoryOperation(SET_VAL, 1, n);

    MemoryOperation* currRet = ret.data();

    unsigned int initVal = n * n;
    if (*globState < initVal)
    {
        *currRet++ = makeMemOp(SET_VAL, globState, initVal, memory);
        *currRet++ = makeMemOp(SET_VAL, pars + u, u, memory);
        *currRet++ = makeMemOp(SET_VAL, sizes + u, 1, memory);
        *currRet++ = makeMemOp(SET_VAL, states + u, 0, memory);
        *currRet++ = makeMemOp(SET_VAL, toWrite + u, 0, memory);
        return ret;
    }

    unsigned int currEdge = states[u] / 2;
    unsigned int currPhase = states[u] % 2;

    if (currEdge == n + 1)
    {
        unsigned int numUnions = (*globState - initVal) / (n + 1);
        unsigned int numFinished = (*globState - initVal) % (n + 1);

        assert(numFinished <= n);

        if (numFinished < n) return ret;

        assert(numUnions <= n);

        if (numUnions == n - 1)
        {
            *currRet++ = makeMemOp(SET_VAL, result, CONN_RES, memory);
        }
        else
        {
            *currRet++ = makeMemOp(SET_VAL, result, DISC_RES, memory);
        }

        return ret;
    }

    unsigned int v = 0;
    unsigned int uRoot = 0;
    unsigned int vRoot = 0;

    while (true)
    {
        if (currEdge == numEdges)
        {
            assert(currPhase == 0);

            *currRet++ = makeMemOp(SET_VAL, states + u, (n + 1) * 2 + 0, memory);
            *currRet++ = makeMemOp(SET_VAL, toWrite + u, 0, memory);
            *currRet++ = makeMemOp(ADD_VAL, globState, 1, memory);
            return ret;
        }

        v = edges[currEdge];

        uRoot = findRoot(u, memory);
        vRoot = findRoot(v, memory);

        if (sizes[uRoot] < sizes[vRoot] || (sizes[uRoot] == sizes[vRoot] && uRoot > vRoot)) std::swap(uRoot, vRoot);

        if (uRoot != vRoot) break;

        assert(currPhase == 0);

        ++currEdge;
    }

    if (currPhase == 0)
    {
        *currRet++ = makeMemOp(SET_VAL, states + u, currEdge * 2 + 1, memory);
        *currRet++ = makeMemOp(SET_VAL, toWrite + u, uRoot * (n + 1) + vRoot, memory);
        return ret;
    }

    assert(toWrite[u] / (n + 1) == uRoot);
    assert(toWrite[u] % (n + 1) == vRoot);

    std::array<bool, MAX_N + 1> usedU{};
    std::array<bool, MAX_N + 1> usedV{};

    for (unsigned int i = 1; i <= n; ++i)
    {
        usedU[i] = false;
        usedV[i] = false;
    }

    for (unsigned int i = 1; i < u; ++i)
    {
        unsigned int otherU = toWrite[i] / (n + 1);
        unsigned int otherV = toWrite[i] % (n + 1);

        if (uRoot == otherU && vRoot == otherV)
        {
            *currRet++ = makeMemOp(SET_VAL, states + u, (currEdge + 1) * 2 + 0, memory);
            *currRet++ = makeMemOp(SET_VAL, toWrite + u, 0, memory);
            return ret;
        }

        if (usedU[otherU] || usedV[otherU] || usedV[otherV] || usedU[otherV]) continue;

        usedU[otherU] = true;
        usedV[otherV] = true;

        if (uRoot == otherU || uRoot == otherV || vRoot == otherV || vRoot == otherU)
        {
            *currRet++ = makeMemOp(SET_VAL, states + u, currEdge * 2 + 0, memory);
            *currRet++ = makeMemOp(SET_VAL, toWrite + u, 0, memory);
            return ret;
        }
    }

    *currRet++ = makeMemOp(SET_VAL, pars + vRoot, uRoot, memory);
    *currRet++ = makeMemOp(ADD_VAL, sizes + uRoot, sizes[uRoot], memory);
    *currRet++ = makeMemOp(SET_VAL, states + u, (currEdge + 1) * 2 + 0, memory);
    *currRet++ = makeMemOp(SET_VAL, toWrite + u, 0, memory);
    *currRet++ = makeMemOp(ADD_VAL, globState, n + 1, memory);

    return ret;
}
