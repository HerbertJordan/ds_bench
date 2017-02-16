
#include <vector>

#include <set>
#include <unordered_set>
#include "BTree.h"
#include "Trie.h"
#include "btree_set.h"

#include "eval.h"

template<typename Set, typename List>
auto eval(const List& list) {
    return run_eval([&](){
        Set s;
        for(auto cur : list) {
            s.insert(cur);
        }
    });
}

template<typename Set, typename List>
auto eval_btree_with_hints(const List& list) {
    return run_eval([&](){
        Set s;
        typename Set::operation_hints hints;
        for(auto cur : list) {
            s.insert(cur,hints);
        }
    });
}

template<typename Set, typename List>
auto eval_brie_with_hints(const List& list) {
    return run_eval([&](){
        Set s;
        typename Set::op_context hints;
        for(auto cur : list) {
            s.insert(cur,hints);
        }
    });
}


int main() {

    const int D = 2;
    using Value = souffle::RamDomain;
    using Entry = souffle::ram::Tuple<Value, D>;

    using Set = std::set<Entry>;
    using UnorderedSet = std::unordered_set<Entry>;
    using BTree = souffle::btree_set<Entry>;
    using Brie = souffle::Trie<D>;
    using GBTree = btree::btree_set<Entry>;

    const int N = 1000;
//    const int N = 10;

    std::string setups[] = {
        "std::set",
        "std::unordered_set",
        "google btree",
        "btree",
        "brie",
        "btree with context",
        "brie with context"
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
    std::cout << "Testing unordered_set .. \n";
    timesOrdered[1] = eval<UnorderedSet>(data);
    std::cout << "Testing google btree .. \n";
    timesOrdered[2] = eval<GBTree>(data);
    std::cout << "Testing btree .. \n";
    timesOrdered[3] = eval<BTree>(data);
    std::cout << "Testing brie .. \n";
    timesOrdered[4] = eval<Brie>(data);
    std::cout << "Testing btree with hints .. \n";
    timesOrdered[5] = eval_btree_with_hints<BTree>(data);
    std::cout << "Testing brie with hints .. \n";
    timesOrdered[6] = eval_brie_with_hints<Brie>(data);
    
    // try random order
    std::random_shuffle(data.begin(),data.end());

    EvalResult timesUnordered[7];

    std::cout << "Testing std::set .. \n";
    timesUnordered[0] = eval<Set>(data);
    std::cout << "Testing unordered_set .. \n";
    timesUnordered[1] = eval<UnorderedSet>(data);
    std::cout << "Testing google btree .. \n";
    timesUnordered[2] = eval<GBTree>(data);
    std::cout << "Testing btree .. \n";
    timesUnordered[3] = eval<BTree>(data);
    std::cout << "Testing brie .. \n";
    timesUnordered[4] = eval<Brie>(data);
    std::cout << "Testing btree with hints .. \n";
    timesUnordered[5] = eval_btree_with_hints<BTree>(data);
    std::cout << "Testing brie with hints .. \n";
    timesUnordered[6] = eval_brie_with_hints<Brie>(data);
       
       
    // print summary:
    
    std::cout << "\n";
    std::cout << "DS,min ordered,min unordered,avg ordered,avg unordered,max ordered,max unordered\n";
    for(int i=0; i<7; i++) {
        std::cout << setups[i] 
            << "," << timesOrdered[i].min << "," << timesUnordered[i].min
            << "," << timesOrdered[i].avg << "," << timesUnordered[i].avg
            << "," << timesOrdered[i].max << "," << timesUnordered[i].max 
            << "\n";
    }
       
    return 0;
}
