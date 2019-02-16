#include <vector>
#include "park.h"
std::pair<int, int> getEdge(int num) {
    int f,t;
    f=num/2+1;
    t=f+1+num%2;
    return {f,t};
}
struct QueryMaker {
    int batchSize, ones, qpb; //queries per batch
    std::vector<int> dirs, queries;
    QueryMaker(int batchSize, int qpb) : batchSize(batchSize), qpb(qpb) {
        ones=(1<<qpb)-1;
        dirs.resize(1<<qpb);
        queries.resize(1<<qpb);
    }
    std::vector<int> genAllPoss(int rem, const std::vector<int>& currPoss) {
        if (rem==0) return currPoss;
        std::vector<int> nextPoss;
        for (int i=0; i<currPoss.size(); ++i) {
            int curr=currPoss[i];
            nextPoss.push_back((curr<<1)+1);
            if (curr & 1) nextPoss.push_back((curr<<1)+0);
        }
        return genAllPoss(rem-1,nextPoss);
    }
    bool xorDigits(int a) {
        bool total=false;
        while (a) {
            total^=a&1;
            a=a>>1;
        }
        return total;
    }
    bool tryQuery(int query, bool opt, int rem, int currCode, const std::vector<int>& currPoss) {
        int maxForNext=1<<(rem-1);
        std::vector<int> xorZeros,xorOnes;
        int cnt=0;
        for (int i=0; i<currPoss.size(); ++i) {
            int curr=currPoss[i];
            if (xorDigits(curr & query)) ++cnt;
        }
        if (cnt>maxForNext || currPoss.size()-cnt>maxForNext) return false;
        if (opt && (cnt*1.0>currPoss.size()*0.55 || currPoss.size()-cnt*1.0>currPoss.size()*0.55)) return false;
        xorZeros.resize(0);
        xorOnes.resize(0);
        for (int i=0; i<currPoss.size(); ++i) {
            int curr=currPoss[i];
            if (xorDigits(curr & query)) xorOnes.push_back(curr);
            else xorZeros.push_back(curr);
        }
        if (!bruteForceQueries(rem-1,(currCode<<1)+0,xorZeros)) return false;
        if (!bruteForceQueries(rem-1,(currCode<<1)+1,xorOnes)) return false;
        queries[currCode & ones]=query;
        return true;
    }
    bool bruteForceQueries(int rem, int currCode, const std::vector<int>& currPoss) {
        if (currPoss.empty()) return true;
        if (rem==0) {
            dirs[currCode & ones]=currPoss[0];
            return true;
        }
        if (rem>13) {
            for (int query=1; query<1<<batchSize; ++query) {
                if (tryQuery(query,true,rem,currCode,currPoss)) return true;
            }
        }
        else if (rem>4) {
            for (int i=1; i<1<<batchSize; i=i<<1) {
                for (int j=1; j<1<<batchSize; j=j<<1) {
                    for (int k=1; k<1<<batchSize; k=k<<1) {
                        if (tryQuery(i | j | k,true,rem,currCode,currPoss)) return true;
                    }
                }
            }
        }
        else if (rem>1) {
            for (int i=1; i<1<<batchSize; i=i<<1) {
                for (int j=1; j<1<<batchSize; j=j<<1) {
                    if (tryQuery(i | j,false,rem,currCode,currPoss)) return true;
                }
            }
        }
        else {
            for (int i=1; i<1<<batchSize; i=i<<1) {
                if (tryQuery(i,false,rem,currCode,currPoss)) return true;
            }
        }
        return false;
    }
    void findDirsAndQueries() {
        std::vector<int> allPoss=genAllPoss(batchSize-1, {1,0});
        bruteForceQueries(qpb,1,allPoss);
    }
    bool makeQuery(const std::vector<std::pair<int, int>>& toState, int curr, int code) {
        int query=queries[code & ones];
        std::vector<std::pair<int, int>> edges;
        for (int i=0; i<batchSize; ++i) {
            if (query & 1<<(batchSize-1-i)) {
                edges.push_back(toState[curr+i]);
            }
        }
        return get_xor(edges);
    }
    void proccessBatch(const std::vector<std::pair<int, int>>& toState, int& curr) {
        int code=1;
        for (int i=0; i<qpb; ++i) {
            std::vector<std::pair<int, int>> edges;
            for (int i=0; i<batchSize; ++i) {
                if (queries[code & ones] & 1<<(batchSize-1-i)) {
                    edges.push_back(toState[curr+i]);
                }
            }
            code=(code<<1) | get_xor(edges);
        }
        int currDirs=dirs[code & ones];
        for (int i=0; i<batchSize; ++i) {
            state_direction(toState[curr+i],currDirs & 1<<(batchSize-1-i));
        }
        curr+=batchSize;
    }
};
QueryMaker QM1(1,1),QM4(4,3),QM22(22,16);
std::vector<std::pair<int, int>> toState;
void run(int n) {
    QM1.findDirsAndQueries();
    QM4.findDirsAndQueries();
    QM22.findDirsAndQueries();
    std::pair<int, int> edge;
    for (int i=0; i<4*n+1; ++i) {
        edge=getEdge(i);
        if (i%2==1 || i==0 || i==4*n) state_direction(edge,true);
        else toState.push_back(edge);
    }
    int curr=0;
    while (curr<toState.size()) {
        if (curr+QM22.batchSize<=toState.size()) QM22.proccessBatch(toState,curr);
        else if (curr+QM4.batchSize<=toState.size()) QM4.proccessBatch(toState,curr);
        else QM1.proccessBatch(toState,curr);
    }
}
