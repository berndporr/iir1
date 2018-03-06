cmake_minimum_required(VERSION 3.5)

add_executable (iirdemo iirdemo.cpp)
target_link_libraries(iirdemo iir_static)
target_include_directories(iirdemo PRIVATE ..)
