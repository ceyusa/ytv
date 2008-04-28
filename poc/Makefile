CFLAGS=`pkg-config --cflags libsoup-2.4` -I ../ -Wall -O0 -g 
LDFLAGS=`pkg-config --libs libsoup-2.4`

all: fetcher-soup

fetcher-soup: fetcher-soup.c

.PHONY: clean

clean:
	rm -rf *.o fetcher-soup
