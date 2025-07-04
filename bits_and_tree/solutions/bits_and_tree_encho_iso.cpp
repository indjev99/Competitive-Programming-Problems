#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <assert.h>
using namespace std;
typedef unsigned long long ullong;

const int MAXN = 210;

struct BigNum
{
    static const ullong B = 20;
    static const ullong BASE = (1ULL << B);
    static const ullong MASK = BASE - 1;

    vector<ullong> vals;

    BigNum(ullong val)
    {
        if (val == 0)
        {
            vals.push_back(0);
            return;
        }

        while(val > 0)
        {
            vals.push_back(val & MASK);
            val >>= B;
        }
    }
    BigNum(): BigNum(0) {}

    BigNum& operator+=(const ullong& other)
    {
        vals[0] += other;

        ullong carry = (vals[0] >> B);
        vals[0] &= MASK;
        for (int i = 1; i < vals.size(); i++)
        {
            vals[i] += carry;
            carry = (vals[i] >> B);
            vals[i] &= MASK;
        }

        while(carry > 0)
        {
            vals.push_back(carry & MASK);
            carry = (carry >> B);
        }

        return *this;
    }

    BigNum& operator*=(const ullong& other)
    {
        vals[0] *= other;

        ullong carry = (vals[0] >> B);
        vals[0] &= MASK;
        for (int i = 1; i < vals.size(); i++)
        {
            vals[i] = vals[i] * other + carry;
            carry = (vals[i] >> B);
            vals[i] &= MASK;
        }

        while(carry > 0)
        {
            vals.push_back(carry & MASK);
            carry = (carry >> B);
        }

        return *this;
    }

    BigNum& operator+=(const BigNum& other)
    {
        if (vals.size() < other.vals.size())
        {
            vals.resize(other.vals.size(), 0);
        }

        ullong carry = 0;
        for (int i = 0; i < vals.size(); i++)
        {
            vals[i] += (i < other.vals.size() ? other.vals[i] : 0) + carry;
            carry = (vals[i] >> B);
            vals[i] &= MASK;

            if (i >= other.vals.size() && carry == 0)
                break;
        }

        while(carry > 0)
        {
            vals.push_back(carry & MASK);
            carry >>= B;
        }

        return *this;
    }

    bool operator<=(const BigNum other) const
    {
        return !(other < *this);
    }

    bool operator>(const BigNum other) const
    {
        return !(*this <= other);
    }

    BigNum& operator-=(const BigNum& other)
    {
        assert(other <= *this);

        ullong need = 0;
        for (int i = 0; i < vals.size(); i++)
        {
            ullong take = need + ( i < other.vals.size() ? other.vals[i] : 0 );
            need = 0;
            while(vals[i] < take)
            {
                vals[i] += BASE;
                need++;
            }
            vals[i] -= take;
        }

        while(vals.size() > 1 && vals.back() == 0)
            vals.pop_back();

        return *this;
    }

    BigNum operator*(const BigNum& other) const
    {
        BigNum result;
        result.vals.resize(vals.size() + other.vals.size(), 0);

        for (int i = 0; i < vals.size(); i++)
        {
            for (int j = 0; j < other.vals.size(); j++)
            {
                result.vals[i + j] += vals[i] * other.vals[j];
            }
        }

        ullong carry = 0;
        for (int i = 0; i < result.vals.size(); i++)
        {
            result.vals[i] += carry;

            carry = (result.vals[i] >> B);
            result.vals[i] &= MASK;
        }

        while(carry > 0)
        {
            result.vals.push_back(carry & MASK);
            carry >>= B;
        }

        while(result.vals.size() > 1 && result.vals.back() == 0)
        {
            result.vals.pop_back();
        }

        return result;
    }

    bool operator<(const BigNum& other) const
    {
        if (vals.size() != other.vals.size())
            return vals.size() < other.vals.size();
        else
        {
            for (int i = (int)vals.size() - 1; i >= 0; i--)
            {
                if (vals[i] != other.vals[i])
                    return vals[i] < other.vals[i];
            }
            return false;
        }
    }

    vector<bool> toBitString(int pad_size) const
    {
        vector<bool> result;

        for (int i = 0; i < vals.size(); i++)
        {
            ullong v = vals[i];
            for (int j = 0; j < B; j++)
            {
                result.push_back(bool(v % 2));
                v /= 2;
            }
        }
        while(result.size() > 1 && !result.back())
            result.pop_back();
        while(result.size() < pad_size)
            result.push_back(false);

        reverse(result.begin(), result.end());

        return result;
    }

    int approxLog()
    {
        ullong last = vals.back();
        int ans = 0;

        while(last > 0)
        {
            last /= 2;
            ans++;
        }

        ans += ((int)vals.size() - 1) * B;

        return ans;
    }

    ullong collapse()
    {
        ullong res = 0;
        for (int i = (int)vals.size() - 1; i >= 0; i--)
        {
            res *= BASE;
            res += vals[i];
        }
        return res;
    }

    BigNum operator/(const BigNum& other)
    {
        BigNum result;
        BigNum finalResult;
        bool started = false;
        for (int i = (int)vals.size() - 1; i >= 0; i--)
        {
            result *= BASE;
            result += vals[i];

            if (started)
            {
                finalResult *= BASE;
            }

            if (other <= result)
            {
                started = true;
                ullong L = 1, R = BASE - 1;
                ullong best = 0;
                while(L <= R)
                {
                    ullong mid = (L + R) / 2;
                    BigNum temp = other;
                    temp *= mid;
                    if (temp <= result)
                    {
                        best = mid;
                        L = mid + 1;
                    }
                    else
                    {
                        R = mid - 1;
                    }
                }

                finalResult += best;
                result -= other * best;
            }
        }

        return finalResult;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigNum& ds)
    {
        os << "[";
        for (int i = 0; i < ds.vals.size(); i++)
        {
            if (i != 0)
                os << ", ";
            os << ds.vals[i];
        }
        os << "]";

        return os;
    }
};

BigNum F[MAXN + 1][MAXN + 1];
bool cachedF[MAXN + 1][MAXN + 1];
BigNum getF(int sz, int minChild)
{
    if (sz == 1)
        return 1;
    if (minChild >= sz)
        return 0;

    if (cachedF[sz][minChild])
        return F[sz][minChild];

    F[sz][minChild] = getF(sz, minChild + 1);

    auto A = getF(minChild, 1);
    auto B = getF(sz - minChild, minChild + 1);
    F[sz][minChild] += A * B;
    cachedF[sz][minChild] = true;

    return F[sz][minChild];
}

BigNum getFexact(int sz, int minChild)
{
    BigNum res = getF(sz, minChild);
    res -= getF(sz, minChild + 1);
    return res;
}

const int HASHBITS = 20;
const ullong HASHMOD = (1ULL << HASHBITS);
const ullong HASHBASE = 31;
const int NODE_MARKERS = 2;
vector<pair<int, int>> edges;
int buildTree(BigNum remEncoding, int& rootNode, int sz)
{
    int myRoot = rootNode;

    //cout << "Building tree of size " << sz << " number " << remEncoding << " and root " << myRoot << endl;

    /// Find smallest child
    int remSz = sz;
    for (int i = 1; i <= sz - 1; i++)
    {
        if (getFexact(remSz, i) <= remEncoding)
        {
            remEncoding -= getFexact(remSz, i);
        }
        else
        {
            BigNum otherChildren = getF(remSz - i, i + 1);

            /// X * otherChildren + rem  = remEncoding

            BigNum thisChild = remEncoding / otherChildren;
            //cout << "Making a new child of size " << i << " for vertex " << myRoot << endl;
            int childRoot = buildTree(thisChild, ++rootNode, i);
            edges.push_back({myRoot, childRoot});

            /*if (myRoot == 0)
            {
                cout << remEncoding << endl;
                cout << remEncoding << " / " << otherChildren << " = " << remEncoding / otherChildren << endl;
                cout << thisChild << " * " << otherChildren << " = " << thisChild * otherChildren << endl;
                exit(0);
            }*/
            remEncoding -= thisChild * otherChildren;
            remSz -= i;

            //cout << "Back to vertex " << myRoot << " and remaining is " << remEncoding << " for a size of " << remSz << endl;
        }
    }

    return myRoot;
}

vector<pair<int, int>> encode(int n, vector<bool> data)
{
    assert(n == 200);
    assert(data.size() == 400);

    edges.clear();

    n -= NODE_MARKERS;
    BigNum totalOptions = getF(n, 1);

    int allowedBits = totalOptions.approxLog() - 1;

    BigNum encodedString;
    ullong hashVal = 0;
    for (int i = 0; i < allowedBits - HASHBITS; i++)
    {
        encodedString *= 2;
        hashVal *= HASHBASE;
        if (data[i])
        {
            encodedString += 1;
            hashVal += 1;
        }

        hashVal %= HASHMOD;
    }

    for (int i = HASHBITS - 1; i >= 0; i--)
    {
        encodedString *= 2;
        if ( (hashVal & (1LL << i)) > 0 )
            encodedString += 1;
    }

    int root = 0;
    /*cout << "ENCODING " << encodedString << endl;
    auto bits = encodedString.toBitString(allowedBits);
    for (int i = 0; i < bits.size(); i++)
    {
        cout << int(bits[i]);
    }
    cout << endl;*/
    buildTree(encodedString, root, n);

    /*for (auto [x, y] : edges)
    {
        cout << x << " - " << y << endl;
    }
    cout << edges.size() << " edges" << endl;*/

    for (int i = n; i < n + NODE_MARKERS; i++)
    {
        edges.push_back({0, i});
    }

    return edges;
}

vector<int> Graph[MAXN];
int sz[MAXN];

void DFS(int ver, int dad)
{
    sz[ver] = 1;
    for (auto nver : Graph[ver])
    {
        if (nver == dad)
            continue;
        DFS(nver, ver);
        sz[ver] += sz[nver];
    }
}

map<pair<int, bool>, vector<BigNum>> cache;
vector<BigNum> findAll(int ver, int dad, bool removeNode)
{
    if (cache.find({ver, removeNode}) != cache.end())
        return cache[{ver, removeNode}];

    vector<pair<int, int>> children;
    for (auto nver : Graph[ver])
    {
        if (nver == dad)
            continue;

        children.push_back({sz[nver], nver});
    }

    if (children.empty()) /// Leaf
    {
        //cout << "It has size " << sz[ver] << endl;
        assert(!removeNode);
        cache[{ver, removeNode}] = {0};
        return {0};
    }

    /// Guess leaf reduction
    vector<BigNum> results;
    for (int i = 0; i < children.size(); i++)
    {
        if (!removeNode && i > 0)
            break;

        int badver = -1;
        vector<pair<int, int>> remainder = children;

        if (removeNode)
        {
            badver = children[i].second;
            remainder[i].first--;
            if (remainder[i].first == 0)
            {
                swap(remainder[i], remainder.back());
                remainder.pop_back();
            }
        }

        sort(remainder.begin(), remainder.end());

        bool badSetup = false;
        for (int j = 1; j < remainder.size(); j++)
        {
            if (remainder[j].first == remainder[j - 1].first)
            {
                badSetup = true;
                break;
            }
        }

        if (badSetup)
            continue;

        vector<BigNum> curValues = {0};
        int remSz = sz[ver];
        if (removeNode)
            remSz--;

        int lastSz = 0;
        for (auto [sz, nv] : remainder)
        {
            for (int skipped = lastSz + 1; skipped < sz; skipped++)
            {
                for (int j = 0; j < curValues.size(); j++)
                {
                    curValues[j] += getFexact(remSz, skipped);
                }
            }
            lastSz = sz;

            vector<BigNum> subcodes = findAll(nv, ver, nv == badver);

            if (subcodes.empty())
            {
                badSetup = true;
                break;
            }

            if (curValues.size() != subcodes.size())
            {
                assert(curValues.size() == 1 || subcodes.size() == 1);
                while(curValues.size() < subcodes.size())
                {
                    curValues.push_back(curValues.back());
                }
                while(subcodes.size() < curValues.size())
                {
                    subcodes.push_back(subcodes.back());
                }
            }

            for (int j = 0; j < subcodes.size(); j++)
            {
                curValues[j] += subcodes[j] * getF(remSz - sz, sz + 1);
            }
            remSz -= sz;
        }

        if (badSetup)
        {
            continue;
        }

        for (auto& v : curValues)
        {
            results.push_back(v);
        }
    }

    if (!removeNode)
    {
        assert(results.size() <= 1);
    }

    for (auto nver : Graph[ver])
    {
        if (nver == dad)
            continue;
        cache.erase({nver, false});
        cache.erase({nver, true});
    }

    cache[{ver, removeNode}] = results;
    return results;
}

int father[500111];
int Find(int ver)
{
    if (father[ver] == -1)
        return ver;
    else
    {
        father[ver] = Find(father[ver]);
        return father[ver];
    }
}
std::vector<bool> decode(int n, vector<pair<int, int>> tree)
{
    for (int i = 0; i <= n; i++)
    {
        father[i] = -1;
    }
    assert(tree.size() == n);
    for (auto [x, y] : tree)
    {
        int r1 = Find(x);
        int r2 = Find(y);

        assert(r1 != r2);
        father[r1] = r2;
    }

    n -= NODE_MARKERS;

    cache.clear();
    for (int i = 0; i < MAXN; i++)
    {
        Graph[i].clear();
    }

    for (auto [x, y] : tree)
    {
        Graph[x].push_back(y);
        Graph[y].push_back(x);
    }

    /*for (auto [x, y] : tree)
    {
        cout << x << " -- " << y << endl;
    }*/

    int TOTAL_BITS = getF(n, 1).approxLog() - 1;

    /// Try roots
    vector<vector<bool>> allCorrect;
    for (int root = 0; root <= n + NODE_MARKERS; root++)
    {
        //cout << "Trying root " << root << endl;
        vector<int> singles;
        vector<int> doubles;
        DFS(root, -1);
        for (int i = 0; i < Graph[root].size(); i++)
        {
            if (Graph[ Graph[root][i] ].size() == 1)
            {
                singles.push_back(i);
            }
            else if (sz[ Graph[root][i] ] == 2)
            {
                doubles.push_back(i);
            }
        }

        if (singles.size() + 1 < NODE_MARKERS)
        {
            continue;
        }

        /// Singles only
        if (singles.size() >= NODE_MARKERS)
        {
            DFS(root, -1);
            //cout << "Its " << sz[root] << " initially" << endl;
            vector<int> backup = Graph[root];
            for (int i = NODE_MARKERS - 1; i >= 0; i--)
            {
                int s = singles[i];
                Graph[root][s] = Graph[root].back();
                Graph[root].pop_back();
            }

            DFS(root, -1);
            //cout << "sz root = " << sz[root] << endl;
            assert(sz[root] == n + 1);
            cache.clear();
            vector<BigNum> results = findAll(root, -1, true);

            for (auto& x : results)
            {
                auto bits = x.toBitString(TOTAL_BITS);
                if (bits.size() != TOTAL_BITS)
                    continue;

                ullong hashVal = 0;
                for (int i = 0; i < TOTAL_BITS - HASHBITS; i++)
                {
                    hashVal *= HASHBASE;
                    if (bits[i])
                    {
                        hashVal += 1;
                    }
                    hashVal %= HASHMOD;
                }

                bool bad = false;
                for (int i = bits.size() - 1; i >= bits.size() - HASHBITS; i--)
                {
                    if ( bool(hashVal % 2) != bits[i] )
                    {
                        bad = true;
                        break;
                    }
                    hashVal /= 2;
                }

                if (bad)
                    continue;

                allCorrect.push_back(bits);

                return bits;
            }

            Graph[root] = backup;
        }

        /// Double and singles
        if (doubles.size() >= 1 && singles.size() >= NODE_MARKERS - 1)
        {
            DFS(root, -1);
            //cout << "DOUBLES IS AT INDEX " << doubles[0] << " -> " << sz[ Graph[root][ doubles[0] ] ] << endl;
            //cout << "ITS NODE " << Graph[root] [ doubles[0] ];
            vector<int> backup = Graph[root];
            for (int i = NODE_MARKERS - 2; i >= 0; i--)
            {
                int s = singles[i];
                //cout << "Removing " << singles[i] << "(" << Graph[root][s] << ")" << endl;
                Graph[root][s] = Graph[root].back();
                Graph[root].pop_back();
            }

            ///DFS(root, -1);
            ///cout << sz[root] << " now" << endl;

            bool found = false;
            for (int i = 0; i < Graph[root].size(); i++)
            {
                //cout << "Node " << Graph[root][i] << " has size " << sz[ Graph[root][i] ] << endl;
                if (sz[ Graph[root][i] ] == 2)
                {
                    found = true;
                    Graph[root][i] = Graph[root].back();
                    Graph[root].pop_back();
                    break;
                }
            }

            assert(found);

            DFS(root, -1);
            //cout << "becomes " << sz[root] << endl;
            assert(sz[root] == n);
            cache.clear();
            vector<BigNum> results = findAll(root, -1, false);

            for (auto& x : results)
            {
                auto bits = x.toBitString(TOTAL_BITS);
                if (bits.size() != TOTAL_BITS)
                    continue;

                ullong hashVal = 0;
                for (int i = 0; i < TOTAL_BITS - HASHBITS; i++)
                {
                    hashVal *= HASHBASE;
                    if (bits[i])
                    {
                        hashVal += 1;
                    }
                    hashVal %= HASHMOD;
                }

                bool bad = false;
                for (int i = bits.size() - 1; i >= bits.size() - HASHBITS; i--)
                {
                    if ( bool(hashVal % 2) != bits[i] )
                    {
                        bad = true;
                        break;
                    }
                    hashVal /= 2;
                }

                if (bad)
                    continue;

                allCorrect.push_back(bits);

                return bits;
            }

            Graph[root] = backup;
        }
    }

    for (int i = 1; i < allCorrect.size(); i++)
    {
        assert (allCorrect[i] == allCorrect[0]);
    }

    assert(allCorrect[0][0]);
    return allCorrect[0];
}

/**
int main()
{
    freopen("test.01.in", "r", stdin);

    int n = 200;
    int t, x;

    cin >> t >> x;
    string s;
    cin >> n;

    mt19937 mt(1337);
    for (int test = 1; test <= t; test++)
    {
        cin >> s;
        vector<bool> data;
        for (int i = 0; i < s.length(); i++)
        {
            data.push_back(s[i] == '1');
        }


        for (int i = 0; ; i++)
        {
            cout << "Decoding " << i << endl;
            auto edges = encode(n, data);
            edges.push_back({mt()%n, n});
            decode(n, edges);
        }
    }

    return 0;
}**/
