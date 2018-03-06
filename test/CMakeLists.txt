cmake_minimum_required(VERSION 3.5)
enable_testing()
include_directories(
  ..
  ../iir
  )

add_executable (test_butterworth butterworth.cpp)
target_link_libraries(test_butterworth iir)
add_test(TestButterworth test_butterworth)

add_executable (test_bessel bessel.cpp)
target_link_libraries(test_bessel iir)
add_test(TestBessel test_bessel)

add_executable (test_chebyshev1 chebyshev1.cpp)
target_link_libraries(test_chebyshev1 iir)
add_test(TestChebyshev1 test_chebyshev1)

add_executable (test_chebyshev2 chebyshev2.cpp)
target_link_libraries(test_chebyshev2 iir)
add_test(TestChebyshev2 test_chebyshev2)

add_executable (test_elliptic elliptic.cpp)
target_link_libraries(test_elliptic iir)
add_test(TestElliptic test_elliptic)

add_executable (test_rbj rbj.cpp)
target_link_libraries(test_rbj iir)
add_test(TestRBJ test_rbj)
