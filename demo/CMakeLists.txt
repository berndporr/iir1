project(IIRDemo)

cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)

add_executable (iirdemo iirdemo.cpp)
target_link_libraries(iirdemo iir_static)

add_executable (ecg50hzfilt ecg50hzfilt.cpp)
target_link_libraries(ecg50hzfilt iir_static)
