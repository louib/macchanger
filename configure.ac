AC_INIT([macchanger], [1.7.0])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_PATH_GTK_3_0([3.10.0],,AC_MSG_ERROR([Gtk+ 3.10.0 or higher required.]))

AM_CONFIG_HEADER(config.h)
AC_SUBST(VERSION)

AC_PROG_INSTALL
AC_PROG_CC

AC_OUTPUT([
Makefile
src/Makefile
data/Makefile
doc/Makefile])
