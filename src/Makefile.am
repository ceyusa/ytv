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
	ytv-entry.c 		\
	ytv-entry.h 		\
	ytv-iterator.c		\
	ytv-iterator.h		\
	ytv-list.c		\
	ytv-list.h		\
	ytv-entry-iterator.c	\
	ytv-entry-iterator-priv.h	\
	ytv-entry-list.c	\
	ytv-entry-list-priv.h	\
	main.c
