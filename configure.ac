
AC_INIT([ytv], [0.0.1], [http://www.igalia.com])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AM_CONFIG_HEADER([config.h])

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_HEADER_STDC

AM_MAINTAINER_MODE

if test "x$GCC" = "xyes"; then CFLAGS="$CFLAGS -Wall -pedantic"; fi

dnl #####################################
dnl ### Basic dependencies - Required ###
dnl #####################################
gobject_modules="   gobject-2.0 >= 2.6.0
		    glib-2.0 >= 2.6.0
		    gtk+-2.0 >= 2.6.0"

PKG_CHECK_MODULES([GOBJECT], [$gobject_modules])

dnl #####################################
dnl ### Optional dependencies         ###
dnl #####################################
libsoup_modules="    libsoup-2.4 >= 2.23.0
 		     gconf-2.0 >= 2.22.0
		     json-glib-1.0 >= 0.5.0"

PKG_CHECK_MODULES([SOUP], [$libsoup_modules])

GTK_DOC_CHECK([1.4])

AC_OUTPUT([
Makefile
src/Makefile
docs/Makefile
m4/Makefile
])