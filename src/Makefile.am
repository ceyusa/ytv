bin_PROGRAMS = ytv

ytv_CFLAGS =			\
	-I. -I$(top_srcdir) 	\
	-I$(top_srcdir)/src 	\
	$(GOBJECT_CFLAGS)   	\
	$(SOUP_CFLAGS)

ytv_LDADD =			\
	$(GOBJECT_LIBS)		\
	$(SOUP_LIBS)

ytv_SOURCES = 				\
	ytv-shared.h			\
	ytv-entry.c 			\
	ytv-entry.h 			\
	ytv-iterator.c			\
	ytv-iterator.h			\
	ytv-list.c			\
	ytv-list.h			\
	ytv-simple-list-iterator.c	\
	ytv-simple-list-iterator-priv.h	\
	ytv-simple-list.c		\
	ytv-simple-list.h		\
	ytv-entry-list-priv.h		\
	ytv-feed-fetch-strategy.c	\
	ytv-feed-fetch-strategy.h	\
	ytv-feed-parse-strategy.c	\
	ytv-feed-parse-strategy.h	\
	ytv-feed.h			\
	ytv-feed.c			\
	ytv-soup-feed-fetch-strategy.h	\
	ytv-soup-feed-fetch-strategy.c	\
	main.c
