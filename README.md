# SDL_inprint

Add simplest text output to your SDL program. Inline font included. Font collection included.

Has almost no good use cases; however, I found myself reusing this code/font format over
and over, so maybe you would find some use for it too. 

 * If you're serious about inlining your images, use something like
[GRIT][grit] instead.
 * If you're serious about bitmap fonts, a meta-data-less bitmap is not enough,
use [Sfont][sfont] instead.
 * If you're *really* serious about fonts, and want Unicode and italics and
everything, use [SDL_ttf][ttf] instead.

 [grit]: http://www.coranac.com/projects/grit/
 [sfont]: http://www.linux-games.com/sfont/
 [ttf]: http://www.libsdl.org/projects/SDL_ttf/

## how to use

Build `inprint.c` with your project. Bundle `inline_font.h`. 
Include `SDL_inprint.h`.

Now, you can:

```c
    screen = SDL_SetVideoMode(width, height, bpp, flags);

    prepare_inline_font();

    incolor(0xFF0000, 0x333333);
    inprint(screen, "Hello World!", 10, 10);

    kill_inline_font();
```

The whole demo is in 'example.c'.

You can also do something like

```c
    infont(SDL_LoadBMP("fontfile.bmp")); //use bitmap font
```

and

```c
    infont(NULL); //revert to inline font
```

You would also probably want to automate the inlining into the build process,
which shouldn't be hard. See 'Makefile' for an example.

## SDL2

SDL2 version also exists, with a slightly different API. See `SDL2_inprint.h`,
`inprint2.c` and `example2.c`. 

## font collection

All the fonts in the 'fonts/' directory are free (or should be free, correct me
when I'm wrong) and adhere to the convention described below. Feel free to add
your fonts or make old ones prettier (it's actually quite fun to work on bitmap
fonts even if you're not that good at graphics). 

## bitmap fonts

 * First half of ASCII table (128 symbols).
 * Formatted as 16 x 8 table.
 * Color 0 is background.
 * Color 1 is foreground.
 * One Bit Per Pixel. 1BPP. *Bit*map.

## inline fonts

Inline fonts are in [XBM format][xbm].

 [xbm]: http://en.wikipedia.org/wiki/X_BitMap

You can generate them, using the included `bmp2xbm` tool.
