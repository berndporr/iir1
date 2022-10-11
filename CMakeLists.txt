cmake_minimum_required(VERSION 3.1.0)

project(iir VERSION 1.9.4 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

cmake_policy(SET CMP0048 NEW) # set VERSION in project()
cmake_policy(SET CMP0042 NEW) # enable MACOSX_RPATH by default

option(INSTALL_STATIC "Build static library" ON)

include(GNUInstallDirs)
# If including this project in another project, use IIR1_BUILD_TESTING to enable local testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR IIR1_BUILD_TESTING)
    add_subdirectory(test)
    enable_testing ()
endif()
add_subdirectory(demo)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wconversion -Wextra -pedantic)
endif()

set(LIBSRC
  iir/Biquad.cpp
  iir/Butterworth.cpp
  iir/Cascade.cpp
  iir/ChebyshevI.cpp
  iir/ChebyshevII.cpp
  iir/Custom.cpp
  iir/PoleFilter.cpp
  iir/RBJ.cpp)

set(LIBINCLUDE
  iir/Biquad.h
  iir/Butterworth.h
  iir/Cascade.h
  iir/ChebyshevI.h
  iir/ChebyshevII.h
  iir/Common.h
  iir/Custom.h
  iir/Layout.h
  iir/MathSupplement.h
  iir/PoleFilter.h
  iir/RBJ.h
  iir/State.h
  iir/Types.h)

add_library(iir SHARED ${LIBSRC})
add_library(iir::iir ALIAS iir)

target_include_directories(iir
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>      # for Iir.h
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/iir>  # everything else
)

set_target_properties(iir PROPERTIES
  SOVERSION 1
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER Iir.h
  PRIVATE_HEADER "${LIBINCLUDE}")

install(TARGETS iir EXPORT iir-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/iir)

include(JoinPaths)

join_paths(includedir_for_pc_file "\${prefix}" "${CMAKE_INSTALL_INCLUDEDIR}")
join_paths(libdir_for_pc_file "\${prefix}" "${CMAKE_INSTALL_LIBDIR}")

configure_file(iir.pc.in iir.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/iir.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

add_library(iir_static STATIC ${LIBSRC})
add_library(iir::iir_static ALIAS iir_static)

target_include_directories(iir_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>      # for Iir.h
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/iir>  # everything else
)

set_target_properties(iir_static PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER Iir.h
  PRIVATE_HEADER "${LIBINCLUDE}")

if(INSTALL_STATIC)
  install(TARGETS iir_static EXPORT iir-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/iir)
endif()

install(EXPORT iir-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/iir
  NAMESPACE iir::
  FILE iirTargets.cmake
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/iirConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/iir"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/iirConfigVersion.cmake"
  VERSION "${iir_VERSION_MAJOR}.${iir_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/iirConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/iirConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/iir
  )
