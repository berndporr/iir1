project(IIRDemo)

cmake_minimum_required(VERSION 3.8.0)

if (MSVC)
    add_compile_options(/W4)
  else()
    add_compile_options(-Wall)
endif()

add_executable (iirdemo iirdemo.cpp)
target_link_libraries(iirdemo iir_static)
target_compile_features(iirdemo PRIVATE cxx_std_11)

add_executable (rbj_update rbj_update.cpp)
target_link_libraries(rbj_update iir_static)
target_compile_features(rbj_update PRIVATE cxx_std_11)

add_executable (ecg50hzfilt ecg50hzfilt.cpp)
target_link_libraries(ecg50hzfilt iir_static)
target_compile_features(ecg50hzfilt PRIVATE cxx_std_11)
