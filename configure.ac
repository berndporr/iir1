AC_INIT([LibIIR], [1.0], [berndporr@f2s.com], [iir],
        [http://www.berndporr.me.uk])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])

AC_CONFIG_HEADERS([config.h iirconfig.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX
LT_INIT([disable-static])

AC_SUBST([IIR_SO_VERSION], [1:0:0])

AC_CONFIG_FILES([Makefile
                 iir.pc:iir.pc.in])
AC_OUTPUT
