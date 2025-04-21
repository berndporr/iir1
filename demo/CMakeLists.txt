project(IIRDemo)

cmake_minimum_required(VERSION 3.8.0)

if (MSVC)
    add_compile_options(/W4)
  else()
    add_compile_options(-Wall)
endif()

# Copy scripts and data to build dir	
function(copy_demo_file TARGET FILE)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
            "$<TARGET_FILE_DIR:${TARGET}>/${FILE}"
    )
endfunction()

add_executable (iirdemo iirdemo.cpp)
target_link_libraries(iirdemo iir_static)
target_compile_features(iirdemo PRIVATE cxx_std_11)
copy_demo_file(iirdemo plot_impulse_fresponse.py)

add_executable (rbj_update rbj_update.cpp)
target_link_libraries(rbj_update iir_static)
target_compile_features(rbj_update PRIVATE cxx_std_11)

add_executable (ecg50hzfilt ecg50hzfilt.cpp)
target_link_libraries(ecg50hzfilt iir_static)
target_compile_features(ecg50hzfilt PRIVATE cxx_std_11)
copy_demo_file(ecg50hzfilt plot_ecg_filt.py)
copy_demo_file(ecg50hzfilt ecg50hz.dat)
