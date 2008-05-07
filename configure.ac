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

dnl #####################################
dnl ### Basic dependencies - Required ###
dnl #####################################
GLIB_MAJORMINOR=2.0
GLIB_REQUIRED=2.0.0
PKG_CHECK_MODULES([GOBJECT], [gobject-$GLIB_MAJORMINOR >= $GLIB_REQUIRED])
PKG_CHECK_MODULES([GLIB], [glib-$GLIB_MAJORMINOR >= $GLIB_REQUIRED])

dnl #####################################
dnl ### Optional dependencies         ###
dnl #####################################
GTK_DOC_CHECK([1.4])

LIBSOUP_MAJORMINOR=2.4
LIBSOUP_REQUIRED=2.20.0
PKG_CHECK_MODULES([SOUP], [libsoup-$LIBSOUP_MAJORMINOR > $LIBSOUP_REQUIRED])

AC_OUTPUT([
Makefile
src/Makefile
docs/Makefile
m4/Makefile
])