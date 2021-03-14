#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXD = 101;
const int MAXK = 101;

int dayLimit(int d, int k)
{
    return 10 * d;
}

double scoring(int authT, int contT)
{
    return pow( (double)authT / (double)contT, 3.0 );
}

vector< pair<int, int> > shots;
bool isDead[MAXD+1][2*MAXK+1];

int main(int argc, char *argv[])
{
    ifstream in(argv[1]);
    ifstream sol(argv[2]);
    ifstream out(argv[3]);

    if (!in.is_open())
    {
        cerr << "In-file " << argv[1] << " not found." << endl;
        cout << 0 << endl;
        return 0;
    }

    if (!sol.is_open())
    {
        cerr << "Sol-file " << argv[2] << " not found." << endl;
        cout << 0 << endl;
        return 0;
    }

    if (!out.is_open())
    {
        cerr << "Out-file " << argv[3] << " not found." << endl;
        cout << 0 << endl;
        return 0;
    }

    int d, k;
    int authT;
    int contT;

    in >> d >> k;
    sol >> authT;

    out >> contT;

    if (contT > dayLimit(d, k))
    {
        cerr << "Trying to shoot " << contT << " times. Maximum allowed for the test is " << dayLimit(d, k) << endl;
        cout << 0 << endl;
        return 0;
    }

    if (contT < 0)
    {
        cerr << "Invalid number of shots: " << contT << endl;
        cout << 0 << endl;
        return 0;
    }

    for (int i = 1; i <= contT; i++)
    {
        int shotTime, target;

        out >> shotTime >> target;

        if (shotTime < 0)
        {
            cerr << "Shot number " << i << " has invalid time: " << shotTime << endl;
            cout << 0 << endl;
            return 0;
        }

        if (shotTime >= dayLimit(d, k))
        {
            cerr << "Shot number " << i << " has invalid time: " << shotTime << ". The maximum allowed shot time for this test is " << dayLimit(d, k) << endl;
            cout << 0 << endl;
            return 0;
        }

        if (target < 0 || target >= d)
        {
            cerr << "Shot number " << i << " has invalid zone: " << target <<". The zone must be an integer in the interval [0, " << d - 1 << "]" << endl;
            cout << 0 << endl;
            return 0;
        }

        shots.push_back(make_pair(shotTime, target));
    }

    sort(shots.begin(), shots.end());

    for (int i = 1; i < shots.size(); i++)
    {
        if (shots[i].first == shots[i-1].first)
        {
            cerr << "Shooting more than once at second " << shots[i].first << endl;
            cout << 0 << endl;
            return 0;
        }
    }

    memset(isDead, false, sizeof(isDead));

    for (int i = 0; i < shots.size(); i++)
    {
        for (int j = 0; j < d; j++)
        {
            for (int in = -k; in <= k; in++)
            {
                if (in == 0)
                    continue;
                if (isDead[j][in + k])
                    continue;

                int realSpeed = in;
                while(realSpeed < 0)
                    realSpeed += d;

                int posNow = (j + shots[i].first * realSpeed) % d;

                if (posNow == shots[i].second)
                    isDead[j][in + k] = true;
            }
        }
    }

    int livingCount = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = -k; j <= k; j++)
        {
            if (j == 0)
                continue;

            if (!isDead[i][j + k])
            {
                cerr << "Solution does not guarantee the submarine is destroyed" << endl;
                cout << 0 << endl;
                return 0;
            }
        }
    }

    double score = min(scoring(authT, contT), 1.0);

    cout << score << endl;

    return 0;
}
