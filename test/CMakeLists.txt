cmake_minimum_required(VERSION 3.8.0)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -pedantic)
endif()

enable_testing()

add_executable (test_butterworth butterworth.cpp)
target_link_libraries(test_butterworth iir_static)
add_test(TestButterworth test_butterworth)
target_compile_features(test_butterworth PRIVATE cxx_std_11)

add_executable (test_chebyshev1 chebyshev1.cpp)
target_link_libraries(test_chebyshev1 iir_static)
add_test(TestChebyshev1 test_chebyshev1)
target_compile_features(test_chebyshev1 PRIVATE cxx_std_11)

add_executable (test_chebyshev2 chebyshev2.cpp)
target_link_libraries(test_chebyshev2 iir_static)
add_test(TestChebyshev2 test_chebyshev2)
target_compile_features(test_chebyshev2 PRIVATE cxx_std_11)

add_executable (test_rbj rbj.cpp)
target_link_libraries(test_rbj iir_static)
add_test(TestRBJ test_rbj)
target_compile_features(test_rbj PRIVATE cxx_std_11)

add_executable (test_custom custom.cpp)
target_link_libraries(test_custom iir_static)
add_test(TestCUSTOM test_custom)
target_compile_features(test_custom PRIVATE cxx_std_11)

add_executable (test_badparam badparam.cpp)
target_link_libraries(test_badparam iir_static)
add_test(TestBadParam test_badparam)
target_compile_features(test_badparam PRIVATE cxx_std_11)

add_executable (test_state state.cpp)
target_link_libraries(test_state iir_static)
add_test(TestState test_state)
target_compile_features(test_state PRIVATE cxx_std_11)

add_executable (test_assignment assignment.cpp)
target_link_libraries(test_assignment iir_static)
add_test(TestOperator= test_assignment)
target_compile_features(test_state PRIVATE cxx_std_11)
