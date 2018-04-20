cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release")
endif()

project(iir VERSION 1.2.1 LANGUAGES CXX)
include(GNUInstallDirs)
add_subdirectory(test)
add_subdirectory(demo)
enable_testing ()

set(LIBSRC
  iir/Bessel.cpp
  iir/Biquad.cpp
  iir/Butterworth.cpp
  iir/Cascade.cpp
  iir/ChebyshevI.cpp
  iir/ChebyshevII.cpp
  iir/Custom.cpp
  iir/Elliptic.cpp
  iir/Legendre.cpp
  iir/PoleFilter.cpp
  iir/RBJ.cpp
  iir/RootFinder.cpp
  iir/State.cpp)

set(LIBINCLUDE
  iir/Bessel.h
  iir/Biquad.h
  iir/Butterworth.h
  iir/Cascade.h
  iir/ChebyshevI.h
  iir/ChebyshevII.h
  iir/Common.h
  iir/Custom.h
  iir/Elliptic.h
  iir/Layout.h
  iir/Legendre.h
  iir/MathSupplement.h
  iir/PoleFilter.h
  iir/RBJ.h
  iir/RootFinder.h
  iir/State.h
  iir/Types.h
  iir/Utilities.h) 

add_library(iir
  SHARED
  ${LIBSRC}
  )

target_include_directories(iir PRIVATE iir)

set_target_properties(iir PROPERTIES
  SOVERSION 1
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER Iir.h
  PRIVATE_HEADER "${LIBINCLUDE}")

install(TARGETS iir
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/iir)

configure_file(iir.pc.in iir.pc @ONLY)

add_library(iir_static
  STATIC
  ${LIBSRC}
  )

target_include_directories(iir_static PRIVATE iir)

set_target_properties(iir_static PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER Iir.h
  PRIVATE_HEADER "${LIBINCLUDE}")

install(TARGETS iir_static
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/iir)

