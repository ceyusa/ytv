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