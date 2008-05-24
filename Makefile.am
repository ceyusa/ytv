ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

if ENABLE_GTK_DOC
SUBDIRS += docs
endif

DIST_SUBDIRS = 	\
	src 	\
	docs	\
	m4

CLEANFILES = 	\
	*~ 	\
	semantic.cache*

DISTCHECK_CONFIGURE_FLAGS = \
	--enable-gtk-doc

MAINTAINERCLEANFILES = \
	Makefile.in aclocal.m4 configure config.guess config.sub \
	depcomp install-sh ltmain.sh missing mkinstalldirs config.h.in \
	stamp-h.in

DISTCLEANFILES = \
	cscope.files cscope.out

cscope.files:
	find src -name '*.[ch]' > $@

cscope.out: cscope.files
	cscope -b