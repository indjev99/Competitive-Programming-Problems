#include "triangle.h"
#include <queue>
#include <vector>
#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>

const int MAXN = 1e3 + 10;
std::mt19937 rng(69420);
int build_pos[4];
std::vector <int> answer;
std::vector <std::vector <int>> v;
int representA[MAXN];
int representB[MAXN];
bool isFound[MAXN];
bool isPosFound[MAXN];
int posOf[MAXN];

bool ask(int a, int b, int c)
{
    int cnt = 0;
    if (a == build_pos[1]) cnt++;
    if (b == build_pos[1]) cnt++;
    if (c == build_pos[1]) cnt++;
    if (cnt == 3 || (cnt == 1 && ((a == b) || (a == c) || (b == c)))) return true;
    if (cnt) return false;

    return query(a - 1, b - 1, c - 1);
}

void rec(std::vector <int> &indices)
{
    v.push_back(indices);
    int idx = rng() % indices.size();
    std::vector <int> new_indices;
    std::vector <int> bad_indices;
    for (int i = 0 ; i < indices.size() ; ++i)
    {
        if (i == idx) continue;
        if (!ask(indices[idx], indices[i], indices[i]))
        {
            new_indices.push_back(indices[i]);
        } else
        {
            bad_indices.push_back(indices[i]);
        }
    }
    
    new_indices.push_back(indices[idx]);
    if (new_indices.size() == 1)
    {
        build_pos[1] = new_indices[0];
        rec(indices);
        return;
    }

    if (indices.size() == 2)
    {
        answer.push_back(new_indices[0]);   
        answer.push_back(new_indices[1]);
        return;   
    }

    if (new_indices.size() == 2)
    {
        if (indices.size() == 3)
        {
            build_pos[1] = indices[0];
            build_pos[2] = indices[1];
            build_pos[3] = indices[2];
            if (indices[1] == new_indices[0]) std::swap(build_pos[1], build_pos[2]);
            else if (indices[2] == new_indices[0]) std::swap(build_pos[1], build_pos[3]);

            int ptr = v.size() - 1;
            while (ptr >= 0 && v[ptr].size() == 3)
            {
                ptr--;
            }
            
            std::vector <int> last_with_four = v[ptr];
            std::vector <int> valid;
            
            for (int i = 0 ; i < last_with_four.size() && valid.size() < 2 ; ++i)
            {
                if (last_with_four[i] != build_pos[1] && last_with_four[i] != build_pos[2] && last_with_four[i] != build_pos[3])
                {
                    if (ask(last_with_four[i], build_pos[2], build_pos[2]))
                    {
                        valid.push_back(last_with_four[i]);
                    }
                }
            }

            if (!valid.empty())
            {
                std::swap(build_pos[2], build_pos[3]);
                answer.push_back(build_pos[1]);
                answer.push_back(build_pos[2]);
                answer.push_back(build_pos[3]);
                if (valid.size() == 1)
                {
                    answer.push_back(valid[0]);
                } else
                {
                    assert(valid.size() == 2);
                    if (!ask(build_pos[2], build_pos[3], valid[0]))
                    {
                        std::swap(valid[0], valid[1]);
                    }

                    answer.push_back(valid[0]);
                    answer.push_back(valid[1]);
                }
            } else
            {
                answer.push_back(build_pos[1]);
                answer.push_back(build_pos[2]);
                answer.push_back(build_pos[3]);
            }

            return;
        }

        build_pos[1] = new_indices[0];
        build_pos[2] = new_indices[1];        
        std::vector <int> valid;
        
        for (int i = 0 ; i < bad_indices.size() && valid.size() < 3 ; ++i)
        {
            if (bad_indices[i] != build_pos[1] && bad_indices[i] != build_pos[2] && bad_indices[i] != build_pos[3])
            {
                if (ask(bad_indices[i], build_pos[2], build_pos[2]))
                {
                    valid.push_back(bad_indices[i]);
                }
            }
        }
        
        if (valid.size() == 1)
        {
            build_pos[3] = valid[0];
            answer.push_back(build_pos[1]);
            answer.push_back(build_pos[2]);
            answer.push_back(build_pos[3]);
            return;
        }

        for (int i = 0 ; i < valid.size() ; ++i)
        {
            bool is_ok = true;
            for (int j = 0 ; j < valid.size() ; ++j)
            {
                if (i != j && ask(valid[i], valid[i], valid[j]))
                {
                    is_ok = false;
                    break;
                }
            }

            if (is_ok)
            {
                build_pos[3] = build_pos[2];
                build_pos[2] = valid[i];
                std::swap(valid[i], valid.back());
                valid.pop_back();
                break;
            }
        }

        answer.push_back(build_pos[1]);
        answer.push_back(build_pos[2]);
        answer.push_back(build_pos[3]);

        if (valid.size() == 1)
        {
            answer.push_back(valid[0]);
        } else
        {
            assert(valid.size() == 2);
            if (!ask(build_pos[2], build_pos[3], valid[0]))
            {
                std::swap(valid[0], valid[1]);
            }

            answer.push_back(valid[0]);
            answer.push_back(valid[1]);
        }

        return;
    }

    new_indices.pop_back();
    rec(new_indices);
}

std::vector <int> solve(int n)
{
    answer.clear();
    v.clear();

    for (int i = 1 ; i <= n ; ++i)
    {
        representA[i] = representB[i] = isFound[i] = isPosFound[i] = 0;
    }

    std::fill(build_pos, build_pos + 4, 0);
    std::vector <int> perm(n);
    std::iota(perm.begin(), perm.end(), 1);
    rec(perm);

    int ptr = 0;
    for (const int &pos : answer)
    {
        ptr++;
        isPosFound[pos] = true;
        isFound[ptr] = true;
        posOf[ptr] = pos;
    }

    int representTO = 2 * answer.size();
    for (int i = 2 ; i <= 2 * answer.size() ; ++i)
    {
        representA[i] = answer[i / 2 - 1];
        representB[i] = answer[i / 2 - 1 + (i & 1)];
    }

    while (true)
    {
        bool should_break = true;
        for (int i = 1 ; i <= n ; ++i)
        {
            if (!isPosFound[i])
            {
                should_break = false;
                if (ask(i, representA[representTO], representB[representTO]))
                {
                    int l = 2, r = representTO, mid;
                    while (l < r - 1)
                    {
                        mid = l + r >> 1;
                        if (!ask(i, representA[mid], representB[mid])) l = mid;
                        else r = mid;
                    }
                    
                    posOf[l] = i;
                    isFound[l] = true;
                    isPosFound[i] = true;
                }
            }
        }

        if (should_break)
        {
            break;
        }

        for (int i = representTO + 1 ; i <= std::min(n + 1, 2 * (representTO - 1)) ; ++i)
        {
            representA[i] = posOf[i / 2];
            representB[i] = posOf[i / 2 + (i & 1)];
        }

        representTO = std::min(n + 1, 2 * (representTO - 1));
    }

    answer.clear();
    answer.resize(n);
    for (int i = 1 ; i <= n ; ++i)
    {
        answer[posOf[i] - 1] = i;
    }

    return answer;
}