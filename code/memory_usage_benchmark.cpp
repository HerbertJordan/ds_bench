
#include <vector>

#include <set>
#include "BTree.h"
#include "Trie.h"

template<typename Set>
std::vector<std::size_t> eval(int limit, int gap) {

    std::vector<std::size_t> res;

    int step = limit / 10;

    Set s;
    int counter = 0;
    std::cout << counter << "," << s.getMemoryUsage() << "\n";
    res.push_back(s.getMemoryUsage());
    for(int i = 0; i<100000; ++i) {
        for(int j = 0; j<1000000; j+=gap) {
            s.insert({i,j});
            counter++;
            if (counter % step == 0 || counter == limit) {
                auto msize = s.getMemoryUsage();
                std::cout << counter << "," << msize << "\n";
                res.push_back(msize);
            }
            if (counter == limit) return res;    
        }
    }
}


int main() {

    const int max = 100000000;

    std::vector<std::vector<std::size_t>> data;

    const int D = 2;
    using Value = souffle::RamDomain;
    using Entry = souffle::ram::Tuple<Value, D>;

    using BTree = souffle::btree_set<Entry>;
    using Brie = souffle::Trie<D>;

    std::cout << "\nFilling Btree 100% ... \n";
    data.push_back(eval<BTree>(max,1));
    
    std::cout << "\nFilling Btree 10% ... \n";
    data.push_back(eval<BTree>(max,10));
    
    std::cout << "\nFilling Btree 1% ... \n";
    data.push_back(eval<BTree>(max,100));
    
    std::cout << "\nFilling Btree 0.1% ... \n";
    data.push_back(eval<BTree>(max,1000));
    
    std::cout << "\nFilling Brie 100% ... \n";
    data.push_back(eval<Brie>(max,1));
    
    std::cout << "\nFilling Brie 10% ... \n";
    data.push_back(eval<Brie>(max,10));
    
    std::cout << "\nFilling Brie 5% ... \n";
    data.push_back(eval<Brie>(max,20));
    
    std::cout << "\nFilling Brie 2% ... \n";
    data.push_back(eval<Brie>(max,50));
    
    std::cout << "\nFilling Brie 1% ... \n";
    data.push_back(eval<Brie>(max,100));

    std::cout << "\nFilling Brie 0.5% ... \n";
    data.push_back(eval<Brie>(max,200));

    std::cout << "\nFilling Brie 0.1% ... \n";
    data.push_back(eval<Brie>(max,1000));
     
    std::cout << "\nResults:\n";
    for(int i=0; i<11; i++) {
        for(int j=0; j<data.size(); ++j) {
            std::cout << data[j][i] << ",";
        }
        std::cout << "\n";
    } 
     
    return 0;
}
