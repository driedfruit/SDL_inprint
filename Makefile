CC=gcc

CFLAGS=-g -Wall `sdl-config --cflags`
LDFLAGS=`sdl-config --libs`

#XBM=php ./bmp2xbm.php
XBM=./bmp2xbm

FONT_FILE=fonts/qbfat8x8.bmp

all: example1 example2

bmp2xbm:
	gcc -Wall -pedantic bmp2xbm.c -o $@

inline_font.h: bmp2xbm
	$(XBM) $(FONT_FILE) $@

font.c: inline_font.h
	echo "#include <SDL.h>" > $@-tmp1
	cat inline_font.h >> $@-tmp1
	cat inprint.c > $@-tmp2
	sed '/#include/d' $@-tmp2 >> $@-tmp1
	rm $@-tmp2
	mv $@-tmp1 $@

example1: font.c
	gcc $(CFLAGS) font.c example.c -o example1 $(LDFLAGS)

example2: inline_font.h
	gcc $(CFLAGS) inprint.c example.c -o example2 $(LDFLAGS)

clean:
	rm -rf *.o *.a example1 example2 font.c inline_font.h
