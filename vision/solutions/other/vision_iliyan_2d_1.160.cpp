#include "vision.h"
#include<iostream>
#include<algorithm>
#include<assert.h>
using namespace std;

vector <int> getVisionPattern1d () {
    return {1};
}

int getMove1d (vector <int> v) {
    return 0;
}

const int num=4;
vector <vector <int>> patterns2d[num] = {
    {
        { 2, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1 },
        { 1, 1, 2, 1, 2 },
        { 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 2 },
    },
    {
        { 1, 1, 1, 1, 1 },
        { 1, 2, 1, 1, 1 },
        { 1, 1, 2, 1, 1 },
        { 1, 1, 1, 1, 1 },
        { 1, 2, 1, 1, 2 },
    },
    {
        { 1, 1, 1, 2, 1 },
        { 1, 1, 1, 1, 1 },
        { 2, 1, 2, 1, 1 },
        { 1, 1, 1, 1, 1 },
        { 1, 1, 2, 1, 1 },
    },
    {
        { 2, 1, 2, 1, 1 },
        { 1, 1, 1, 1, 1 },
        { 1, 1, 2, 1, 1 },
        { 1, 1, 1, 2, 1 },
        { 1, 1, 1, 1, 1 },
    }
};
vector <vector <int>> patterns[num*4];
vector <vector <int>> flip (vector <vector <int>> orig, bool rows, bool cols) {
    if (rows==true) reverse(orig.begin(),orig.end());
    if (cols==true) {
        for (auto& row : orig) {
            reverse(row.begin(),row.end());
        }
    }
    return orig;
}
vector <vector <int>> getVisionPattern2d () {
    for (int i=0; i<num; i++) {
        patterns[i*4]=patterns2d[i];
        patterns[i*4+1]=flip(patterns[i*4],0,1);
        patterns[i*4+2]=flip(patterns[i*4],1,0);
        patterns[i*4+3]=flip(patterns[i*4],1,1);
    }
    /*for (int i=0; i<num*4; i++) {
        for (auto row : patterns[i]) {
            for (auto v : row) {
                cout << v << " ";
            }
            cout << endl ;
        }
        cout << endl ;
        for (int j=i-1; j>=0; j--) {
            assert(patterns[i]!=patterns[j]);
        }
    }*/
    return patterns2d[0];
}

pair <int, int> getMove2d (vector <vector <int>> vals) {
    int len=vals.size();
    int center=vals[len/2][len/2];
    if (center==1) {
        for (int i=0; i<len; i++) {
            for (int j=0; j<len; j++) {
                if (vals[i][j]==2) return {i, j};
            }
        }
        return {1, 0};
    }
    for (int i=0; i<num*4; i++) {
        if (patterns[i]==vals) {
            int type=i%4;
            if (type==0) return {4, 4};
            if (type==1) return {4, 0};
            if (type==2) return {0, 4};
            if (type==3) return {0, 0};
        }
    }
    cout << "Wrong:\n";
    for (auto row : vals) {
        for (auto val : row) {
            cout << val << " ";
        }
        cout << endl ;
    }
    exit(1);
}
