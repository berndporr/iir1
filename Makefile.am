AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
lib_LTLIBRARIES = libiir.la
libiir_la_LDFLAGS = -version-info $(IIR_SO_VERSION)
iir_includedir = $(includedir)
nobase_iir_include_HEADERS = Iir.h \
	iir/Bessel.h \
	iir/Biquad.h \
	iir/Butterworth.h \
	iir/Cascade.h \
	iir/ChebyshevI.h \
	iir/ChebyshevII.h \
	iir/Common.h \
	iir/Custom.h \
	iir/Elliptic.h \
	iir/Layout.h \
	iir/Legendre.h \
	iir/MathSupplement.h \
	iir/PoleFilter.h \
	iir/RBJ.h \
	iir/RootFinder.h \
	iir/State.h \
	iir/Types.h \
	iir/Utilities.h

iir_libincludedir = $(libdir)/iir/include
nodist_iir_libinclude_HEADERS = iirconfig.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = iir.pc
dist_noinst_SCRIPTS = autogen.sh

libiir_la_SOURCES = Iir.h \
        iir/Bessel.cpp \
	iir/Biquad.cpp \
	iir/Butterworth.cpp \
	iir/Cascade.cpp \
	iir/ChebyshevI.cpp \
	iir/ChebyshevII.cpp \
	iir/Custom.cpp \
	iir/Elliptic.cpp \
	iir/Legendre.cpp \
	iir/PoleFilter.cpp \
	iir/RBJ.cpp \
	iir/RootFinder.cpp \
	iir/State.cpp \
	iir/Bessel.h \
	iir/Biquad.h \
	iir/Butterworth.h \
	iir/Cascade.h \
	iir/ChebyshevI.h \
	iir/ChebyshevII.h \
	iir/Common.h \
	iir/Custom.h \
	iir/Elliptic.h \
	iir/Layout.h \
	iir/Legendre.h \
	iir/MathSupplement.h \
	iir/PoleFilter.h \
	iir/RBJ.h \
	iir/RootFinder.h \
	iir/State.h \
	iir/Types.h \
	iir/Utilities.h
