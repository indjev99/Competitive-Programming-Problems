#include "gcd.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
using namespace std;

ull play(ull MAX_X)
{
    ull a = 0, b = 0, f = 1; /// f = (2 ^ cnt2) * (3 ^ cnt3)
    int cnt2 = 0, cnt3 = 0; /// number of 2s and 3s found in (x + a)
    vector<int> ost = {0, 1, 2, 3, 4, 5}; /// possible options for (x + a) mod 6
    while (f < MAX_X)
    {
        //cerr << "current a: " << a << endl;
        //cerr << "current f: " << f << ' ' << cnt2 << ' ' << cnt3 << endl;

        ull b = f;
        int maxp = 0, maxq = 0; /// maximal number of new 2s and 3s to be found
        for (int i = 1; i <= 3; ++ i)
        {
            if (2 * b < 2 * MAX_X) {b *= 2; maxp++;}
            if (3 * b < 2 * MAX_X) {b *= 3; maxq++;}
        }
        while (2 * b < 2 * MAX_X) {b *= 2; maxp++;}

        //cerr << "b is " << b << " maxp/q " << maxp << ' ' << maxq << endl;

        ull res = query(a, b), tmp = res;

        int ncnt2 = 0, ncnt3 = 0;
        while (tmp % 2 == 0)
        {
            ncnt2++;
            tmp /= 2;
        }
        while (tmp % 3 == 0)
        {
            ncnt3++;
            tmp /= 3;
        }

        int p = ncnt2 - cnt2, q = ncnt3 - cnt3; /// number of new 2s and 3s actually found

        //cerr << "res : " << res << ' ' << p << ' ' << q << endl;

        if (ost == vector<int>{0, 1, 3, 4} and maxp > p and maxq > q and (p == 0 or q == 0)) /// special
        {
            if (p == 0 and q != 0)
            {
                int add = 1;
                for (int i = 1; i <= q; ++ i) add *= 3;
                a += add * f;
                f *= add;
                cnt3 += q;
                f *= 2;
                cnt2++;
            }
            if (p != 0 and q == 0)
            {
                int add = 1;
                for (int i = 1; i <= p; ++ i) add *= 2;
                if (add % 6 != 2) a += 2 * add * f;
                else a += add * f;
                f *= add;
                cnt2 += p;
                f *= 3;
                cnt3++;
            }
            if (p == 0 and q == 0)
            {
                a += 5 * f;
                f *= 6;
                cnt2++;
                cnt3++;
            }
            ost = {0, 1, 2, 3, 4, 5};
            continue;
        }

        while (ncnt2 > cnt2)
        {
            f *= 2;
            cnt2++;
            //p--;
        }
        while (ncnt3 > cnt3)
        {
            f *= 3;
            cnt3++;
            //q--;
        }

        if (p != maxp)
        {
            a += f;
            f *= 2;
            cnt2++;
        }
        if (p != maxp and q != maxq) ost = {0, 1, 3, 4}; /// additional info can be deduced
        else ost = {0, 1, 2, 3, 4, 5};
    }

    if (f - a > MAX_X) return 0;
    else return f - a;
}
