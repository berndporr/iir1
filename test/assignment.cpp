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

double sq_err(const std::array<double, test_len>& a1,
              const std::array<double, test_len>& a2) {
    double se {0};
    
    for (size_t i {0} ; i < test_len ; i++) {
        double e { a1[i] - a2[i] };
        se += e*e;
    }
    
    return se;
}

int main(int, char**) {
    // setup & test 1st time
    std::cout << "Creating and testing filter using automatic decl\n";
    FilterType filter;
    filter.setupN(0.1);
    std::array<double, test_len> res1 { testFilter(filter) }; 
    // Test response: [0.192873, -0.427308, -0.00869136, 0.266734, 0.2204]

    // setup & test 2nd time
    std::cout << "Creating and testing filter using object assignment\n";
    filter = FilterType();  // this assignment seems to be the problem
    filter.setupN(0.1);
    std::array<double, test_len> res2 { testFilter(filter) }; 
    assert_print(sq_err(res1, res2) < 1e-10,
                 "Filter produces different results after initialisation by assignment.\n");
    
    // Copy into new filter and check same result from each.
    FilterType filter2 = filter;
    std::cout << "Calculating continued response using original filter:\n";
    res1 = testFilter(filter);
    std::cout << "Calculating continued response using assigned filter:\n";
    res2 = testFilter(filter2);
    assert_print(sq_err(res1, std::array<double, test_len> {
                          0.0189142, -0.150791, -0.197797, -0.129944, -0.00990682
                        }) < 1e-10,
                 "Incorrect result from original filter.\n");
    assert_print(sq_err(res1, res2) < 1e-10,
                 "Filter iterates inconsistently after copy.\n");
    
    std::cout << "Checking continued responses differ when second filter changes cutoff:\n";
    filter2.setupN(0.3);
    res1 = testFilter(filter);
    res2 = testFilter(filter2);
    assert_print(sq_err(res1, res2) > 1e-3,
                 "Altering assigned filter cutoff also changes original filter.\n");
    
    
    return 0;
}
