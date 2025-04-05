#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

vector<bool> toBinary(int x, int len = 14)
{
    vector<bool> coded;
    for (int i = 0; i < len; i++)
    {
        coded.push_back(x % 2);
        x /= 2;
    }
    reverse(coded.begin(), coded.end());

    return coded;
}

int fromBinary(const vector<bool>& x, int len = 14)
{
    int num = 0;
    for (int i = 0; i < len; i++)
    {
        num = num * 2 + int(x[i]);
    }
    return num;
}

vector<bool> encode(int n, vector<pair<int, int>> edges)
{
    vector<bool> result;

    for (int i = 0; i < edges.size(); i++)
    {
        vector<bool> val = toBinary(edges[i].first);
        result.insert(result.end(), val.begin(), val.end());
        val = toBinary(edges[i].second);
        result.insert(result.end(), val.begin(), val.end());
    }

    return result;
}

vector<pair<int, int>> decode(int n, vector<bool> data)
{
    vector<pair<int, int>> edges;

    for (int i = 0; i < data.size(); i += 28)
    {
        vector<bool> a, b;
        for (int j = 0; j < 14; j++)
        {
            a.push_back(data[i + j]);
            b.push_back(data[i + j + 14]);
        }
        edges.push_back({fromBinary(a), fromBinary(b)});
    }

    return edges;
}