AC_INIT([freeboard], [0.1.0a])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
AC_LANG_PUSH([C])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([], [[
     return 0;
  ]])],
  [],
  [AC_MSG_ERROR([C compiler does not work])]
)
AC_LANG_POP([C])
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([libX11], [libX11 <= 1.0])

AC_LANG_PUSH([Objective C])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([], [[
     id obj = 0;
     return obj ? 0 : 0;
  ]])],
  [],
  [AC_MSG_ERROR([Objective-C compiler does not work])]
)
AC_LANG_POP([Objective C])
AC_LANG_PUSH([Objective C])
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
     [[#import <Foundation/Foundation.h>]],
     [[@autoreleasepool { NSLog(@"test"); }]]
  )],
  [],
  [AC_MSG_ERROR([Foundation framework not found])]
)
AC_LANG_POP([Objective C])
AC_LANG_PUSH([CXX])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([], [[
     return 0;
  ]])],
  [],
  [AC_MSG_ERROR([C++ compiler does not work])]
)
AC_LANG_POP([CXX])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

