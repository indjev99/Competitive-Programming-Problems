#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>

using namespace std;

typedef unsigned int uint;

uint n, s, t;

#define RANDOM 0
#define ROOT 1
#define SHIFTED 2
#define RANDOM_SHIFT 3
#define ONE_ONE 4
#define SHIFTED_ONE 5

uint maxn, maxst;
bool finish_enable;

int type;

struct Student
{
    uint l, m;
};

bool operator==(const Student& a, const Student& b)
{
    return a.l == b.l && a.m == b.m;
}

struct StudentHash
{
	std::size_t operator()(const Student& student) const
	{
		std::size_t h1 = std::hash<uint>()(student.l);
		std::size_t h2 = std::hash<uint>()(student.m);
		return h1 ^ h2;
	}
};

std::vector<Student> students;
std::unordered_set<Student, StudentHash> used;
std::vector<uint> util;

long long rand_num()
{
    return rand() * (RAND_MAX + 1ll) + rand();
}

void add_student(uint l, uint m)
{
    Student student = {l + 1, m + 1};
    if (used.find(student) == used.end())
    {
        used.insert(student);
        students.push_back(student);
    }
}

void gen_test(ofstream& outfile)
{
    srand(time(0));

    students.clear();
    used.clear();

    bool swapped = false;

    if (type / 10) swapped = true;
    if (swapped) std::swap(s, t);

    if (type % 10 == RANDOM)
    {
        for (int i = 0; i < n * 2; ++i)
        {
            add_student(rand_num() % s, rand_num() % t);
        }
    }
    else if (type % 10 == ROOT)
    {
        util.resize(t);
        std::iota(util.begin(), util.end(), 0);
        std::random_shuffle(util.begin(), util.end());

        for (int i = 0; i < t; ++i)
        {
            add_student(i % s, util[i]);
        }

        int root = rand_num() % t;

        for (int i = 0; i < s; ++i)
        {
            add_student(i, util[root]);
        }

        for (int i = 0; i < t; ++i)
        {
            add_student(root % s, i);
        }
    }
    else if (type % 10 == SHIFTED)
    {
        util.resize(t);
        std::iota(util.begin(), util.end(), 0);
        std::random_shuffle(util.begin(), util.end());

        for (int i = 0; i < t; ++i)
        {
            add_student(i % s, util[i]);
        }

        for (int i = 0; i < t; ++i)
        {
            add_student((i + 1) % s, util[i]);
        }
    }
    else if (type % 10 == RANDOM_SHIFT)
    {
        util.resize(t);
        std::iota(util.begin(), util.end(), 0);
        std::random_shuffle(util.begin(), util.end());

        for (int i = 0; i < t; ++i)
        {
            add_student((i + rand_num() % 10) % s, util[i]);
            add_student((i + rand_num() % 10) % s, util[i]);
        }
    }
    else if (type % 10 == ONE_ONE)
    {
        util.resize(t);
        std::iota(util.begin(), util.end(), 0);
        std::random_shuffle(util.begin(), util.end());

        for (int i = 0; i < t; ++i)
        {
            add_student(i % s, util[i]);
        }
    }
    else if (type % 10 == SHIFTED_ONE)
    {
        util.resize(t);
        std::iota(util.begin(), util.end(), 0);
        std::random_shuffle(util.begin(), util.end());

        for (int i = 0; i < t; ++i)
        {
            add_student(i % s, util[i]);
        }

        for (int i = 0; i < t - 1; ++i)
        {
            add_student((i + 1) % s, util[i]);
        }
    }

    std::random_shuffle(students.begin(), students.end());

    if (n < students.size()) students.resize(n);

    if (finish_enable)
    {
        add_student(s, t);
        add_student(s, t + 1);
        add_student(s + 1, t + 1);
        add_student(s + 2, t + 1);
        s += 4;
        t += 3;

        std::random_shuffle(students.begin(), students.end());
    }

    if (students.size() > maxn || s > maxst || t > maxst)
    {
        std::cerr << "out of range: " << students.size() << " " << s << " " << t << std::endl;
        return;
    }

    if (swapped) std::swap(s, t);

    outfile << students.size() << ' ' << s << ' ' << t << '\n';
    for (Student& student: students)
    {
        if (swapped) std::swap(student.m, student.l);
        if (student.l < 1 || student.l > s || student.m < 1 || student.m > t)
        {
            std::cerr << "student out of range: " << student.l << " " << student.m << " / " << s << " " << t << std::endl;
            return;
        }
        outfile << student.l << ' ' << student.m << '\n';
    }
}

int main()
{
    int num;
    while (true)
    {
        cout << "Test num? ";
        cin >> num;
        cout << "MAX_N MAX_ST? ";
        cin >> maxn >> maxst;

        if (maxn < 30)
        {
            for (int i = 0; i < 5; ++i)
            {
                n = maxn;
                s = maxn / 2;
                t = maxn / 2;

                if (i < 2)
                {
                    type = RANDOM;
                }
                else if (i < 3)
                {
                    type = ROOT;
                }
                else if (i < 5)
                {
                    type = SHIFTED;
                    if (i >= 4)
                    {
                        n = maxn * 4 / 5;
                    }
                }

                string nums = "";
                nums += (num / 10 + '0');
                nums += (num % 10 + '0');
                ofstream outfile("olympiad." + nums + ".in");
                gen_test(outfile);

                ++num;
            }
        }
        else
        {
            finish_enable = true;

            for (int i = 0; i < 15; ++i)
            {
                uint my_maxn = maxn - 8 - rand_num() % 10;

                n = my_maxn;
                s = my_maxn / 2;
                t = my_maxn / 2;

                if (i < 4)
                {
                    type = RANDOM;
                    if (i >= 2)
                    {
                        s -= rand_num() % my_maxn / 4;
                        t -= rand_num() % my_maxn / 4;
                    }
                }
                else if (i < 7)
                {
                    type = ROOT;
                    if (i >= 5)
                    {
                        s = my_maxn / 3 - 2;
                        t = my_maxn / 3 - 2;
                    }
                }
                else if (i < 11)
                {
                    type = SHIFTED;
                    if (i == 8)
                    {
                        n = my_maxn * 12 / 13;
                    }
                    else if (i == 9)
                    {
                        n = my_maxn * 6 / 7;
                    }
                    else if (i == 10)
                    {
                        n = my_maxn * 3 / 4;
                    }
                }
                else if (i < 13)
                {
                    type = RANDOM_SHIFT;
                }
                else if (i < 15)
                {
                    type = ONE_ONE;
                    s = my_maxn / 4;
                    n = my_maxn / 2;
                }

                if (i % 2 == 0) type += 10;

                string nums = "";
                nums += (num / 10 + '0');
                nums += (num % 10 + '0');
                ofstream outfile("olympiad." + nums + ".in");
                gen_test(outfile);

                ++num;
            }
        }
    }
    return 0;
}
