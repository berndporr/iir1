cmake_minimum_required(VERSION 3.10.0)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -pedantic)
endif()

enable_testing()

add_executable (test_butterworth butterworth.cpp scipy.cpp)
target_link_libraries(test_butterworth iir_static)
add_test(TestButterworth test_butterworth WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_butterworth PRIVATE cxx_std_11)

add_executable (test_chebyshev1 chebyshev1.cpp scipy.cpp)
target_link_libraries(test_chebyshev1 iir_static)
add_test(TestChebyshev1 test_chebyshev1 WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_chebyshev1 PRIVATE cxx_std_11)

add_executable (test_chebyshev2 chebyshev2.cpp scipy.cpp)
target_link_libraries(test_chebyshev2 iir_static)
add_test(TestChebyshev2 test_chebyshev2 WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_chebyshev2 PRIVATE cxx_std_11)

add_executable (test_rbj rbj.cpp)
target_link_libraries(test_rbj iir_static)
add_test(TestRBJ test_rbj WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_rbj PRIVATE cxx_std_11)

add_executable (test_custom custom.cpp)
target_link_libraries(test_custom iir_static)
add_test(TestCUSTOM test_custom WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_custom PRIVATE cxx_std_11)

add_executable (test_badparam badparam.cpp)
target_link_libraries(test_badparam iir_static)
add_test(TestBadParam test_badparam WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_badparam PRIVATE cxx_std_11)

add_executable (test_state state.cpp)
target_link_libraries(test_state iir_static)
add_test(TestState test_state WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_state PRIVATE cxx_std_11)

add_executable (test_assignment assignment.cpp)
target_link_libraries(test_assignment iir_static)
add_test(TestOperator= test_assignment WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_features(test_assignment PRIVATE cxx_std_11)

file(GLOB TSV_FILES "${CMAKE_SOURCE_DIR}/test/scipy/*.csv")

add_custom_command(TARGET test_butterworth POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TSV_FILES}
            "$<TARGET_FILE_DIR:test_butterworth>"
)

add_custom_command(TARGET test_butterworth POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/test/scipy/signal.txt"
            "$<TARGET_FILE_DIR:test_butterworth>"
)
