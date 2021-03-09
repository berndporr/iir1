project(IIRDemo)

cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)

if (MSVC)
    add_compile_options(/W4 /WX)
  else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -O3 -march=native )
    # Add -march=native if possible as it speeds
    # up computation by approx factor 2. -O3 brings also
    # a drastic improvement against -O2:
    # add_compile_options(-Wall -Wextra -pedantic -Werror -O3 -march=native)
endif()

add_executable (iirdemo iirdemo.cpp)
target_link_libraries(iirdemo iir_static)
target_include_directories(iirdemo PRIVATE ..)

add_executable (ecg50hzfilt ecg50hzfilt.cpp)
target_link_libraries(ecg50hzfilt iir_static)
target_include_directories(ecg50hzfilt PRIVATE ..)

add_executable (benchmark benchmark.cpp)
target_link_libraries(benchmark iir_static)
target_include_directories(benchmark PRIVATE ..)
