#include "triangle.h"
#include <queue>
#include <vector>
#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>

const int MAXN = 1e3 + 10;
const int INF = 1e9;

std::mt19937 rng(69420);
int order[MAXN];
int build_pos[4];
std::vector <int> answer;
std::vector <std::vector <int>> v;
int split[MAXN][MAXN];
double dp[MAXN][MAXN];
int representA[MAXN];
int representB[MAXN];
bool isFound[MAXN];
bool isPosFound[MAXN];
int posOf[MAXN];

bool hasDpBeenCalced;
void calcDP()
{
    if (hasDpBeenCalced)
    {
        return;
    }

    hasDpBeenCalced = true;
    for (int i = 0 ; i < MAXN ; ++i)
    {
        std::fill(split[i], split[i] + MAXN, -1);
    }
    for (int out = 0 ; out < MAXN ; ++out)
    {
        dp[0][out] = 0;
    }

    for (int in = 1 ; in < MAXN ; ++in)
    {
        for (int out = 0 ; in + out < MAXN ; ++out)
        {
            if (in == 1 && out == 0)
            {
                dp[in][out] = 0;
                continue;
            }
            
            if (out != 0) dp[in][out] = 1.0 + dp[in][0];
            else dp[in][out] = INF;
            for (int k = 1 ; k <= in ; ++k)
            {
                double curr = (k * dp[k][0] + (out + in - k) * dp[in - k][out]) / (double)(in + out) + 1.0;
                if (curr < dp[in][out])
                {
                    dp[in][out] = curr;
                    split[in][out] = k;
                }
            }
        }
    }
}

bool ask(int a, int b, int c)
{
    int cnt = 0;
    if (a == build_pos[1]) cnt++;
    if (b == build_pos[1]) cnt++;
    if (c == build_pos[1]) cnt++;
    if (cnt == 3 || (cnt == 1 && ((a == b) || (a == c) || (b == c)))) return true;
    if (cnt) return false;

    return query(order[a], order[b], order[c]);
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

    calcDP();
    std::fill(build_pos, build_pos + 4, 0);
    std::iota(order + 1, order + 1 + n, 0);

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

    for (int i = 2 ; i <= 2 * answer.size() ; ++i)
    {
        representA[i] = answer[i / 2 - 1];
        representB[i] = answer[i / 2 - 1 + (i & 1)];
    }
    
    int mex = answer.size() + 1;
    int representTO = 2 * answer.size();
    int cntFound = answer.size();

    for (int i = v.size() - 1 ; i >= 0 ; --i)
    {
        std::queue <int> q;
        for (int j = 0 ; j < v[i].size() ; ++j)
        {
            if (!isPosFound[v[i][j]])
            {
                q.push(v[i][j]);
            }
        }
        
        while (q.size())
        {
            int top = q.front();
            q.pop();

            std::vector <int> possible_values;
            for (int j = mex ; j < representTO ; ++j)
            {
                if (!isFound[j])
                {
                    possible_values.push_back(j);
                }
            }

            int lg = 0;
            while ((1 << lg) < cntFound)
            {
                lg++;
            }

            int cntOUT = 0;
            for (int j = representTO ; j <= n ; ++j)
            {
                cntOUT += !isFound[j];
            }
            
            int max_value = representTO;
            while (possible_values.size() > (1 << lg))
            {
                cntOUT++;
                max_value = possible_values.back();
                possible_values.pop_back();
            }

            bool skip = false;
            int topValue = -1;
            while (true)
            {
                int cntIN = possible_values.size();
                // std::cout << "while: " << cntIN << ' ' << cntOUT << ' ' << split[cntIN][cntOUT] << '\n';
                if (cntIN == 0)
                {
                    skip = true;
                    break;
                }

                if (cntIN == 1 && cntOUT == 0)
                {
                    topValue = possible_values[0];
                    break;
                }

                int k = split[cntIN][cntOUT];
                if (k == -1)
                {
                    // std::cout << "ask: " << top << ' ' << representA[representTO] << ' ' << representB[representTO] << '\n';
                    if (!ask(top, representA[max_value], representB[max_value]))
                    {
                        possible_values.clear();
                    } else
                    {
                        cntOUT = 0;
                    }
                    
                    continue;
                }

                int value = possible_values[k - 1] + 1;
                if (ask(top, representA[value], representB[value]))
                {
                    while (possible_values.size() > k)
                    {
                        possible_values.pop_back();
                    }

                    cntOUT = 0;
                } else
                {
                    std::vector <int> new_values;
                    for (int i = k ; i < possible_values.size() ; ++i)
                    {
                        new_values.push_back(possible_values[i]);
                    }

                    possible_values = new_values;
                }
            }

            if (skip)
            {
                q.push(top);
                continue;
            }
            
            // std::cout << "SET!!!!!!!!!1 Here: " << topValue << '\n' << std::flush;
            posOf[topValue] = top;
            isFound[topValue] = 1;
            isPosFound[top] = 1;
            cntFound++;

            while (isFound[mex])
            {
                mex++;
            }

            // std::cout << "done mexing: " << mex << ' ' << representTO << '\n' << std::flush;
            while (representTO <= n)
            {
                bool success = false;
                for (int i = 1 ; i < mex ; ++i)
                {
                    // std::cout << "here? " << i 
                    if (std::min(i, representTO + 1 - i) + mex <= std::max(i, representTO + 1 - i))
                    {
                        // assert(false);
                        continue;
                    }

                    // if (representTO + 1 - i > mex)
                    // {
                    //     continue;
                    // }

                    if (isFound[i] && isFound[representTO + 1 - i])
                    {
                        representTO++;
                        representA[representTO] = posOf[i];
                        representB[representTO] = posOf[representTO - i];
                        success = true;
                        break;
                    }
                }

                if (!success)
                {
                    break;
                }
            }
            
            // std::cout << "done representing: " << representTO << '\n';
        }
    }

    // std::cout << "done?\n" << std::flush;
    answer.clear();
    answer.resize(n);
    for (int i = 1 ; i <= n ; ++i)
    {
        answer[posOf[i] - 1] = i;
    }

    return answer;
}