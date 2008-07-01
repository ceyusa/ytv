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
	ytv-base-feed.h			\
	ytv-base-feed.c			\
	ytv-soup-feed-fetch-strategy.h	\
	ytv-soup-feed-fetch-strategy.c	\
	ytv-error.c			\
	ytv-error.h			\
	ytv-json-feed-parse-strategy.h	\
	ytv-json-feed-parse-strategy.c	\
	ytv-uri-builder.h		\
	ytv-uri-builder.c		\
	ytv-youtube-uri-builder.h	\
	ytv-youtube-uri-builder.c	\
	ytv-star.h			\
	ytv-star.c			\
	ytv-rank.h			\
	ytv-rank.c			\
	ytv-entry-view.h		\
	ytv-entry-view.c		\
	ytv-gtk-entry-view.h		\
	ytv-gtk-entry-view.c		\
	ytv-thumbnail.h			\
	ytv-thumbnail.c			\
	ytv-entry-text-view.h		\
	ytv-entry-text-view.c		\
	ytv-marshal.h			\
	ytv-marshal.c			\
	ytv-browser.h			\
	ytv-browser.c			\
	ytv-gtk-browser.h		\
	ytv-gtk-browser.c		\
	main.c

BUILT_SOURCES=ytv-marshal.c ytv-marshal.h

ytv-marshal.h: ytv-marshal.list
	@GLIB_GENMARSHAL@ \
	        --prefix=ytv_cclosure_marshal $(srcdir)/ytv-marshal.list --header > xgen-gmc \
	&& cp xgen-gmc $(@F) \
	&& rm -f xgen-gmc

ytv-marshal.c: ytv-marshal.h ytv-marshal.list
	echo "#include \"ytv-marshal.h\"" > xgen-gmc \
	&& @GLIB_GENMARSHAL@ \
	        --prefix=ytv_cclosure_marshal $(srcdir)/ytv-marshal.list --body >> xgen-gmc \
	&& cp xgen-gmc $(@F) \
	&& rm -f xgen-gmc

