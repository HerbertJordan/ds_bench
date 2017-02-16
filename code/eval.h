#pragma once

#include <iostream>

#include "time.h"

struct EvalResult {
    long min;
    long avg;
    long max;
};

template<typename Case>
EvalResult run_eval(const Case& run, int N = 10) {
    long total = 0;
    long min = 100000000;
    long max = 0;
    for(int i=0; i<N; i++) {

        auto start = now();
        run();
        auto time = now() - start;
        std::cout << "Time: " << time << "ms\n";

        total += time;

        min = (time < min) ? time : min;
        max = (time > max) ? time : max;
    }

    std::cout << "Average Time: " << (total / N) << "ms\n";
    std::cout << "Minimal Time: " << min << "ms\n";
    std::cout << "Maximal Time: " << max << "ms\n";
    
    return {
        min,
        total / N,
        max
    };
}

