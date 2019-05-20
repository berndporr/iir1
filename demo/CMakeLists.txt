cmake_minimum_required(VERSION 2.8)

add_executable (iirdemo iirdemo.cpp)
target_link_libraries(iirdemo iir_static)
target_include_directories(iirdemo PRIVATE ..)

add_executable (ecg50hzfilt ecg50hzfilt.cpp)
target_link_libraries(ecg50hzfilt iir_static)
target_include_directories(ecg50hzfilt PRIVATE ..)
