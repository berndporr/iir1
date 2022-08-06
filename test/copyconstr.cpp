#include "Iir.h"

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#include "assert_print.h"

#include <iostream>
#include <Iir.h>

using FilterType = Iir::Butterworth::HighPass<8>;

void testFilter(FilterType& filter) {
    std::cout << "Test response: ";
    for (size_t i = 0; i < 5; ++i) {
        std::cout << filter.filter(1.0) << ", ";
    }
    std::cout << std::endl;
}

int main(int, char**) {
    // setup & test 1st time
    FilterType filter;
    filter.setupN(0.1);
    testFilter(filter);  // Test response: 0.192873, -0.427308, -0.00869136, 0.266734, 0.2204,

    // setup & test 2nd time
    filter = FilterType();  // this assignment seems to be the problem
    filter.setupN(0.1);
    testFilter(filter);  // Test response: 1, 1, 1, 1, 1,
    return 0;
}
