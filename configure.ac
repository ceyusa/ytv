AC_INIT([ytv], [0.0.1], [http://www.igalia.com])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AM_CONFIG_HEADER([config.h])

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_HEADER_STDC

dnl versions of GLib
GLIB_MAJORMINOR=2.0
GLIB_REQUIRED=2.0.0

dnl #####################################
dnl ### Basic dependencies - Required ###
dnl #####################################
PKG_CHECK_MODULES([GOBJECT], [gobject-$GLIB_MAJORMINOR >= $GLIB_REQUIRED])
PKG_CHECK_MODULES([GLIB], [glib-$GLIB_MAJORMINOR >= $GLIB_REQUIRED])

dnl #####################################
dnl ### Optional dependencies         ###
dnl #####################################
dnl GTK_DOC_CHECK([1.4])

AC_OUTPUT([
Makefile
src/Makefile
])