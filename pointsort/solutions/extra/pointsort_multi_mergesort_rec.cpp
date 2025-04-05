#include "pointsort.h"
#include <algorithm>
#include <numeric>
#include <map>
#include <cmath>

int makeMask(const std::vector<bool>& cmp)
{
    int mask = 0;
    for (int d = 0; d < (int) cmp.size(); d++)
    {
        mask |= cmp[d] << d;
    }
    return mask;
}

using PointT = std::pair<int, std::vector<int>>;

std::vector<bool> compareInternal(const PointT& left, const PointT& right)
{
    std::vector<bool> ans(left.second.size());
    for (int d = 0; d < (int) left.second.size(); d++)
    {
        ans[d] = left.second[d] < right.second[d];
    }
    return ans;
}

void mergeImpl(
    const std::vector<PointT>& left,
    const std::vector<PointT>& right,
    std::vector<std::map<int, int>>& leftToRight,
    int maxR,
    int k)
{
    if (left.empty()) return;

    auto findCenter = [k](const std::vector<PointT>& points, const std::vector<std::pair<int, int>>& mininMaxVals, bool included)
    {
        int bestI = -1;
        double bestScore = -1;
        for (int i = 0; i < (int) points.size(); i++)
        { 
            const PointT& point = points[i];
            double score = 0;
            for (int d = 0; d < k; d++)
            {
                int left = point.second[d] - mininMaxVals[d].first + (!included);
                int right = mininMaxVals[d].second - point.second[d];

                if (left <= 0 || right <= 0) continue;

                int total = left + right;

                double lp = (double) left / total;
                double rp = (double) right / total;

                score += - lp * log(lp) - rp * log(rp);
            }
            if (score > bestScore)
            {
                bestI = i;
                bestScore = score;
            }
        }
        return bestI;
    };

    std::vector<std::pair<int, int>> leftMinMaxVals(k, {1 << 30, -1});
    for (const PointT& point : left)
    {
        for (int d = 0; d < k; d++)
        {
            leftMinMaxVals[d].first = std::min(leftMinMaxVals[d].first, point.second[d]);
            leftMinMaxVals[d].second = std::max(leftMinMaxVals[d].second, point.second[d]);
        }
    }

    std::vector<std::pair<int, int>> rightMinMaxVals(k);
    for (int d = 0; d < k; d++)
    {
        auto it1 = leftToRight[d].lower_bound(leftMinMaxVals[d].first);
        rightMinMaxVals[d].first = it1 == leftToRight[d].begin() ? 0 : std::prev(it1)->second;
        auto it2 = leftToRight[d].lower_bound(leftMinMaxVals[d].second);
        rightMinMaxVals[d].second = it2 == leftToRight[d].end() ? maxR : it2->second;
    }

    std::vector<PointT> newRight;
    for (const PointT& point : right)
    {
        for (int d = 0; d < k; d++)
        {
            if (point.second[d] >= rightMinMaxVals[d].first && point.second[d] < rightMinMaxVals[d].second)
            {
                newRight.push_back(point);
                break;
            }
        }
    }

    int leftC = findCenter(left, leftMinMaxVals, true);

    std::vector<std::pair<int, int>> searchMinMaxVals(k);
    for (int d = 0; d < k; d++)
    {
        auto it = leftToRight[d].lower_bound(left[leftC].second[d]);
        searchMinMaxVals[d].first = it == leftToRight[d].begin() ? 0 : std::prev(it)->second;
        searchMinMaxVals[d].second = it == leftToRight[d].end() ? maxR : it->second;
    }

    std::vector<PointT> search;
    for (const PointT& point : newRight)
    {
        for (int d = 0; d < k; d++)
        {
            if (point.second[d] >= searchMinMaxVals[d].first && point.second[d] < searchMinMaxVals[d].second)
            {
                search.push_back(point);
                break;
            }
        }
    }

    while (true)
    {
        bool nonCertain = false;
        for (int d = 0; d < k; d++)
        {
            if (searchMinMaxVals[d].first < searchMinMaxVals[d].second)
            {
                nonCertain = true;
                break;
            }
        }
        if (!nonCertain) break;

        int searchC = findCenter(search, searchMinMaxVals, false);

        std::vector<bool> cmp = compare(left[leftC].first, search[searchC].first);

        for (int d = 0; d < k; d++)
        {
            if (cmp[d]) searchMinMaxVals[d].second = std::min(searchMinMaxVals[d].second, search[searchC].second[d]);
            else searchMinMaxVals[d].first = std::max(searchMinMaxVals[d].first, search[searchC].second[d] + 1);
        }
    }

    for (int d = 0; d < k; d++)
    {
        leftToRight[d].insert({left[leftC].second[d], searchMinMaxVals[d].first});
    }

    int numQuads = 1 << k;

    std::vector<std::vector<PointT>> leftParts(numQuads);

    for (const PointT& point : left)
    {
        if (point.first == left[leftC].first) continue;

        int m = makeMask(compareInternal(point, left[leftC]));

        leftParts[m].push_back(point);
    }

    for (int m = 0; m < numQuads; m++)
    {
        mergeImpl(leftParts[m], newRight, leftToRight, maxR, k);
    }
}

std::vector<PointT> merge(
    std::vector<PointT>& left,
    std::vector<PointT>& right,
    int k)
{
    if (left.size() > right.size())
    {
        std::swap(left, right);
    }

    std::vector<std::map<int, int>> leftToRight(k);
    mergeImpl(left, right, leftToRight, right.size(), k);

    std::vector<PointT> points(left.size() + right.size(), {-1, std::vector<int>(k)});

    for (int d = 0; d < k; d++)
    {
        std::vector<std::vector<std::pair<int, int>>> buckets(right.size() + 1);

        for (const PointT& point : right)
        {
            buckets[point.second[d]].push_back({left.size(), point.first});
        }

        for (const PointT& point : left)
        {

            buckets[leftToRight[d][point.second[d]]].push_back({point.second[d], point.first});
        }

        int val = 0;
        std::vector<std::pair<int, int>> pointVals;

        for (auto& bucket : buckets)
        {
            std::sort(bucket.begin(), bucket.end());

            for (auto [_, i] : bucket)
            {
                pointVals.push_back({i, val});
                val++;
            }
        }

        std::sort(pointVals.begin(), pointVals.end());

        for (int i = 0; i < (int) pointVals.size(); i++)
        {
            points[i].first = pointVals[i].first;
            points[i].second[d] = pointVals[i].second;
        }
    }

    return points;
}

std::vector<PointT> mergeSort(const std::vector<int>& is, int k)
{
    if (is.size() == 0) return {};
    if (is.size() == 1) return {{is[0], std::vector<int>(k, 0)}};

    std::vector<int> left(is.begin(), is.begin() + is.size() / 2);
    std::vector<int> right(is.begin() + is.size() / 2, is.end());

    std::vector<PointT> leftPoints = mergeSort(left, k);
    std::vector<PointT> rightPoints = mergeSort(right, k);

    std::vector<PointT> points = merge(leftPoints, rightPoints, k);

    return points;
}

std::vector<std::vector<int>> pointSort(int n, int k)
{
    std::vector<int> is(n);
    std::iota(is.begin(), is.end(), 0);

    std::vector<std::vector<int>> points(n);

    std::vector<PointT> pointsVals = mergeSort(is, k);

    for (int i = 0; i < n; i++)
    {
        points[pointsVals[i].first] = pointsVals[i].second;
    }

    return points;
}
