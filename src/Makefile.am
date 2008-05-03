bin_PROGRAMS = ytv

ytv_CFLAGS =			\
	-I. -I$(top_srcdir) 	\
	-I$(top_srcdir)/src 	\
	$(GOBJECT_CFLAGS)   	\
	$(GLIB_CFLAGS)

ytv_LDADD =			\
	$(GOBJECT_LIBS)		\
	$(GLIB_LIBS)

ytv_SOURCES = 			\
	ytv-shared.h		\
	ytv-entry.c 		\
	ytv-entry.h 		\
	ytv-iterator.c		\
	ytv-iterator.h		\
	ytv-list.c		\
	ytv-list.h		\
	ytv-simple-list-iterator.c	\
	ytv-simple-list-iterator-priv.h	\
	ytv-simple-list.c	\
	ytv-simple-list.h	\
	ytv-entry-list-priv.h	\
	main.c
