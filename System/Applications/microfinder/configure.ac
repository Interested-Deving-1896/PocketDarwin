AC_INIT([microfinder], [0.1])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([NCURSESW], [ncursesw],
                  [],
                  [AC_MSG_ERROR([ncursesw not found via pkg-config])])
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
AC_CHECK_HEADERS([ncursesw/curses.h curses.h])
AC_CHECK_LIB([ncursesw], [initscr], [],
             [AC_MSG_ERROR([ncursesw library not found])])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

