
#include <vector>

#include <set>
#include <unordered_set>
#include "BTree.h"
#include "Trie.h"
#include "btree_set.h"

#include "eval.h"


template<typename Set, typename List>
auto eval(const List& list) {

    // fill the set
    Set s;
    for(auto cur : list) {
        s.insert(cur);
    }

    // scan once through the full data structure
    return run_eval([&](){
        int count = 0;
        for(auto cur : s) {
            count++;
        }
        if (count != list.size()) exit(1);
    });
}


int main() {

    const int D = 2;
    using Value = souffle::RamDomain;
    using Entry = souffle::ram::Tuple<Value, D>;

    using Set = std::set<Entry>;
    using BTree = souffle::btree_set<Entry>;
    using Brie = souffle::Trie<D>;
    using GBTree = btree::btree_set<Entry>;

    const int N = 5000;
//    const int N = 10;

    std::string setups[] = {
        "std::set",
        "google btree",
        "btree",
        "brie"
    };
    

    // --- test sequential inserts of pairs ---

    std::cout << "Creating input vector ... \n";
    std::vector<Entry> data;
    data.reserve(N*N);
    for(Value i=0; i<N; i++) {
        for(Value j=0; j<N; j++) {
            data.push_back({i,j});
        }
    }

    EvalResult timesOrdered[7];

    // measure insertion time
    std::cout << "Testing std::set .. \n";
    timesOrdered[0] = eval<Set>(data);
    std::cout << "Testing google btree .. \n";
    timesOrdered[1] = eval<GBTree>(data);
    std::cout << "Testing btree .. \n";
    timesOrdered[2] = eval<BTree>(data);
    std::cout << "Testing brie .. \n";
    timesOrdered[3] = eval<Brie>(data);
    
       
    // print summary:
    
    std::cout << "\n";
    std::cout << "DS,min,avg,max\n";
    for(int i=0; i<4; i++) {
        std::cout << setups[i] 
            << "," << timesOrdered[i].min
            << "," << timesOrdered[i].avg
            << "," << timesOrdered[i].max
            << "\n";
    }
       
    return 0;
}
