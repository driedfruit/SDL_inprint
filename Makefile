CC=gcc

CFLAGS=-g -Wall `sdl-config --cflags`
LDFLAGS=`sdl-config --libs`

CFLAGS2=-g -Wall `sdl2-config --cflags`
LDFLAGS2=`sdl2-config --libs`

XBM=./bmp2xbm

FONT_FILE=fonts/qbfat8x8.bmp

all: sdl1 sdl2

sdl1: example1 example2

sdl2: sdl2-example1 sdl2-example2

bmp2xbm:
	gcc -Wall -pedantic bmp2xbm.c -o $@

inline_font.h: bmp2xbm
	$(XBM) $(FONT_FILE) $@

font.c: inline_font.h
	@echo "#include <SDL.h>" > $@-tmp1
	@cat inline_font.h >> $@-tmp1
	@cat inprint.c > $@-tmp2
	@sed '/#include/d' $@-tmp2 >> $@-tmp1
	@rm $@-tmp2
	@mv $@-tmp1 $@
	@echo "[~cat] inline_font.h inprint.c > font.c"

font2.c: inline_font.h
	@echo "#include <SDL.h>" > $@-tmp1
	@cat inline_font.h >> $@-tmp1
	@cat inprint2.c > $@-tmp2
	@sed '/#include/d' $@-tmp2 >> $@-tmp1
	@rm $@-tmp2
	@mv $@-tmp1 $@
	@echo "[~cat] inline_font.h inprint2.c > font2.c" 

example1: font.c
	gcc $(CFLAGS) font.c example.c -o $@ $(LDFLAGS)

example2: inline_font.h
	gcc $(CFLAGS) inprint.c example.c -o $@ $(LDFLAGS)

sdl2-example1: font2.c example2.c
	gcc $(CFLAGS2) font2.c example2.c -o $@ $(LDFLAGS2)

sdl2-example2: inline_font.h example2.c
	gcc $(CFLAGS2) inprint2.c example2.c -o $@ $(LDFLAGS2)

clean:
	rm -rf *.o *.a example1 example2 sdl2-example1 sdl2-example2 font.c font2.c inline_font.h
