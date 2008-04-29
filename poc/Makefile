CFLAGS=`pkg-config --cflags libsoup-2.4 json-glib-1.0` -I ../ -Wall -O0 -g 
LDFLAGS=`pkg-config --libs libsoup-2.4 json-glib-1.0`

all: fetcher-soup json-parse

fetcher-soup: fetcher-soup.c

json-parse: json-parse.c

.PHONY: clean

clean:
	rm -rf *.o fetcher-soup json-parse
