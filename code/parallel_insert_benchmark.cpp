
#include <vector>

#include <omp.h>

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
        #pragma omp parallel for schedule(guided,1000)
        for(std::size_t i = 0; i<list.size(); ++i) {
            s.insert(list[i]);
        }
    },3);
}

template<typename Set, typename List>
auto eval_btree_with_hints(const List& list) {
    return run_eval([&](){
        Set s;
        #pragma omp parallel
        {
            typename Set::operation_hints hints;
            #pragma omp for
            for(std::size_t i = 0; i<list.size(); ++i) {
                s.insert(list[i],hints);
            }
        }
    },3);
}

template<typename Set, typename List>
auto eval_brie_with_hints(const List& list) {
    return run_eval([&](){
        Set s;
        #pragma omp parallel
        {
            typename Set::op_context hints;
            #pragma omp for
            for(std::size_t i = 0; i<list.size(); ++i) {
                s.insert(list[i],hints);
            }
        }
    },3);
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

    const int N = 10000;
//    const int N = 10;

    std::string setups[] = {
        "btree",
        "brie",
        "btree with context",
        "brie with context"
    };
    

    // --- test sequential inserts of pairs ---

    const int min_num_threads = 1;
    const int max_num_threads = 32;

    EvalResult timesOrdered[4][max_num_threads+1];
    EvalResult timesUnordered[4][max_num_threads+1];

    
    std::cout << "Creating input vector ... \n";
    std::vector<Entry> data;
    data.reserve(N*N);
    for(Value i=0; i<N; i++) {
        for(Value j=0; j<N; j++) {
            data.push_back({i,j});
        }
    }


    std::cout << "Created shuffled input vector ... \n";
    std::vector<Entry> shuffled = data;
    std::random_shuffle(shuffled.begin(),shuffled.end());

    for(int t=min_num_threads; t<=max_num_threads; ++t) {
    
        // fix the number of threads
        omp_set_num_threads(t);
        
        std::cout << "\nFixing number of threads to " << (t) << "\n";

        // measure insertion time
        std::cout << "Testing btree .. \n";
        timesOrdered[0][t] = eval<BTree>(data);
        std::cout << "Testing brie .. \n";
        timesOrdered[1][t] = eval<Brie>(data);
        std::cout << "Testing btree with hints .. \n";
        timesOrdered[2][t] = eval_btree_with_hints<BTree>(data);
        std::cout << "Testing brie with hints .. \n";
        timesOrdered[3][t] = eval_brie_with_hints<Brie>(data);
        
        // try random order
        std::cout << "\nSwitching to shuffled data ...\n";
        std::cout << "Testing btree .. \n";
        timesUnordered[0][t] = eval<BTree>(shuffled);
        std::cout << "Testing brie .. \n";
        timesUnordered[1][t] = eval<Brie>(shuffled);
        std::cout << "Testing btree with hints .. \n";
        timesUnordered[2][t] = eval_btree_with_hints<BTree>(shuffled);
        std::cout << "Testing brie with hints .. \n";
        timesUnordered[3][t] = eval_brie_with_hints<Brie>(shuffled);
    }   
       
    // print summary:
    
    std::cout << "\n";
    std::cout << "DS";
    for(int t=min_num_threads; t<=max_num_threads; t++) {
        std::cout << "," << t;
    }
    std::cout << "\n";
    
    for(int i=0; i<4; i++) {
        std::cout << setups[i] << " ordered";
        for(int t=min_num_threads; t<=max_num_threads; t++) {
            std::cout << "," << timesOrdered[i][t].min;
        }   
        std::cout << "\n";
        std::cout << setups[i] << " unordered";
        for(int t=min_num_threads; t<=max_num_threads; t++) {
            std::cout << "," << timesUnordered[i][t].min;
        }
        std::cout << "\n";  
    }
       
    return 0;
}
