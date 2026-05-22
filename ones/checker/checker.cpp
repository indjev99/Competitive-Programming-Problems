#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace std;

typedef long long ll;
template<class T> void check_min(T &a, const T &b){ a = (a < b) ? a : b; }

const long long HASH_BASE = 713;
const long long HASH_MOD = 7246427;

static long long hsh = 0;
static void update_hash(long long x){
    hsh *= HASH_BASE;
    hsh += x;
    hsh %= HASH_MOD;
}

std::ifstream in, sol, out;
int main(int argc, char *argv[])
{

    in.open(argv[1]);
    out.open(argv[3]);

    if (!in)
    {
        std::cout << 0 << "\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }

    if (!out)
    {
        std::cout << 0 << "\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    int t;
    in >> t;
    while(t--){
        int n;
        in >> n;
        for(int i = 0; i < n; ++i){
            bool val;
            in >> val;
            update_hash(val);
        }
    }

    ll qs, correct;
    out >> correct;
    if(!correct){
        cout << "0\n";
        return 0;
    }

    ll out_hsh;
    out >> qs >> out_hsh;

    if(out_hsh != hsh){
        cout << "0\n";
        return 0;
    }

    if(qs <= 40)
        cout << 1 << "\n";
    else{
        if(qs > 60){
            const double exp = 0.215;
            cout << fixed << setprecision(2) << pow((double)60 / qs, exp) * (double)7 / (double)10 << "\n";
        }
        else{
            if(qs <= 40){
                cout << 1 << "\n";
            }
            else{
                cout << fixed << setprecision(2) << ((double)-3 * (double)max(40ll, qs) / (double)2 + (double)160) / (double)100 << "\n";
            }
        }
        
    }
    cerr << "Program returned successfully and used at most " << qs << " queries." << "\n";
}
