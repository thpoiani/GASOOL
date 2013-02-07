#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(gasool, 1.06, gasool.anp@live.com)
AC_CONFIG_SRCDIR([src/gasool.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(curl, curl_version, libcurl=yes, libcurl=no) 
if test "x$libcurl" = xno ; then 
  AC_MSG_CHECKING([checking for curl with SSL]) 
  LIBS="$LIBS -lcurl -L/usr/local/ssl/lib -lssl -lcrypto" 
  AC_TRY_LINK([#include <curl/curl.h>], [curl_version();], libcurl=yes, 
              libcurl=no) 
  if test "x$libcurl" = xno ; then 
        AC_MSG_RESULT(no) 
        AC_MSG_ERROR([biblioteca libcurl requerida.. leia o arquivo
CURL antes de prosseguir]) 
  else 
       AC_MSG_RESULT(yes) 
  fi 
fi 

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h curl/curl.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
