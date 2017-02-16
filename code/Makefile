
CC=g++
#CC=clang++

CFLAGS = -O3 --std=c++14 -DNDEBUG -I ./cpp-btree-1.0.1
#CFLAGS = -O0 -g3 --std=c++14 -I ./cpp-btree-1.0.1

DEPS = BTree.h FineGrainedBTree.h Trie.h Util.h ParallelUtils.h eval.h time.h Makefile


all: insert_benchmark membership_benchmark scan_benchmark memory_usage_benchmark parallel_insert_benchmark

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%_benchmark: %_benchmark.o
	$(CC) -o $@ $< $(CFLAGS)

parallel_%.o: parallel_%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -fopenmp

parallel_%_benchmark: parallel_%_benchmark.o
	$(CC) -o $@ $< $(CFLAGS) -fopenmp


.PHONY: clean

clean:
	rm *.o *_benchmark
