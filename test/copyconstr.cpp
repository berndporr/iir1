#include "Iir.h"

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#include "assert_print.h"

#include <iostream>
#include <array>
#include <Iir.h>

using FilterType = Iir::Butterworth::HighPass<8>;
constexpr int test_len {5};

std::array<double, test_len> testFilter(FilterType& filter) {
    std::array <double, test_len> result;
    std::cout << "Test response: [";
    bool first {true};
    for (size_t i = 0; i < test_len; ++i) {
        if (!first) std::cout << ", ";
        first = false;
        result[i] = filter.filter(1.0);
        std::cout << result[i];
    }
    std::cout << "]\n";
    return result;
}

int main(int, char**) {
    // setup & test 1st time
    FilterType filter;
    filter.setupN(0.1);
    std::array<double, test_len> res1 { testFilter(filter) }; 
    // Test response: [0.192873, -0.427308, -0.00869136, 0.266734, 0.2204]
    std::cout << "Next four samples should be: [0.0189142, -0.150791, -0.197797, -0.129944, -0.00990682]\n";
    // std::array<double, test_len> next { testFilter(filter) }; 

    // setup & test 2nd time
    filter = FilterType();  // this assignment seems to be the problem
    filter.setupN(0.1);
    std::array<double, test_len> res2 { testFilter(filter) }; 
    // Test response: [1, 1, 1, 1, 1]
    
    // Calculate Square-error
    double se {0};
    for (size_t i {0} ; i < test_len ; i++) {
        double e { res1[i] - res2[i] };
        se += e*e;
    }
    
    assert_print(se < 1e-10,
                 "Filter produces different results after initialisation by assignment\n");
    
    return 0;
}
