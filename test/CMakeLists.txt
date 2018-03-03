cmake_minimum_required(VERSION 3.5)

add_executable (butterworth butterworth.cpp)

target_link_libraries(butterworth iir)

enable_testing()
add_test(ButterworthTest butterworth)
