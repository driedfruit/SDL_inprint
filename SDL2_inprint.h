#ifndef SDL2_inprint_h
#define SDL2_inprint_h

#include <SDL.h>

extern void prepare_inline_font(SDL_Renderer *renderer);
extern void kill_inline_font(void);

extern void infont(SDL_Renderer *renderer, SDL_Surface *font);
extern void incolor(SDL_Color *color);
extern void incolor24(Uint32 fore); /* Colors must be in 0x00RRGGBB format ! */
extern void inprint(SDL_Renderer *dst, const char *str, Uint32 x, Uint32 y);

extern SDL_Texture *get_inline_font(void);

#endif /* SDL2_inprint_h */
