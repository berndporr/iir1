cmake_minimum_required(VERSION 3.8.0)

project(iir VERSION 1.9.4 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

option(IIR1_NO_EXCEPTIONS "Disable exception handling" OFF)

option(IIR1_INSTALL_STATIC "Install static library" ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IIR1_BUILD_TESTING_DEFAULT ON)
else()
  set(IIR1_BUILD_TESTING_DEFAULT OFF)
endif()

option(IIR1_BUILD_TESTING "Build tests" ${IIR1_BUILD_TESTING_DEFAULT})
if(IIR1_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

option(IIR1_BUILD_DEMO "Build demo" ON)
if(IIR1_BUILD_DEMO)
  add_subdirectory(demo)
endif()

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -pedantic)
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

target_compile_features(iir PUBLIC cxx_std_11)

target_include_directories(iir
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>      # for Iir.h
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/iir>  # everything else
)

if(IIR1_NO_EXCEPTIONS)
    target_compile_definitions(iir PUBLIC "IIR1_NO_EXCEPTIONS")
endif()

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

target_compile_features(iir_static PUBLIC cxx_std_11)

target_include_directories(iir_static
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>      # for Iir.h
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/iir>  # everything else
)

if(IIR1_NO_EXCEPTIONS)
    target_compile_definitions(iir_static PUBLIC "IIR1_NO_EXCEPTIONS")
endif()

set_target_properties(iir_static PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER Iir.h
  PRIVATE_HEADER "${LIBINCLUDE}")

if(IIR1_INSTALL_STATIC)
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
