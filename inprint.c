#include "SDL.h"

#include "inline_font.h" /* Actual font data */

SDL_Surface *inline_font = NULL;
SDL_Surface *selected_font = NULL;
void prepare_inline_font(void)
{
	if (inline_font != NULL) { selected_font = inline_font; return; }

	inline_font = SDL_CreateRGBSurface(SDL_HWSURFACE, font_source.width, font_source.height, 8, 0, 0, 0, 0);
	SDL_Surface *dst = inline_font;

	Uint8 *pix_ptr, tmp;

	int i, len, j;

	/* Cache pointer to pixels and array length */
	pix_ptr = (Uint8 *)dst->pixels;		
	len = dst->h * dst->w / font_source.pixels_per_byte;

	/* Lock, Copy, Unlock */
	SDL_LockSurface(dst);
	for (i = 0; i < len; i++) 
	{
		tmp = (Uint8)font_source.pixel_data[i];
		for (j = 0; j < font_source.pixels_per_byte; j++) 
		{
			Uint8 mask = (0x01 << j);
			pix_ptr[i * font_source.pixels_per_byte + j] = ((tmp & mask) ? 1 : 0);
		}
	}
	SDL_UnlockSurface(dst);

	selected_font = inline_font;
}
void kill_inline_font(void) { SDL_FreeSurface(inline_font); inline_font = NULL; }
void infont(SDL_Surface *font) 
{
	selected_font = font;
	if (font == NULL) prepare_inline_font();
}
void incolor(Uint32 fore, Uint32 back) /* Colors must be in 0x00RRGGBB format ! */
{
	SDL_Color pal[2];
	pal[0].r = (Uint8)((back & 0x00FF0000) >> 16); 
	pal[0].g = (Uint8)((back & 0x0000FF00) >> 8);
	pal[0].b = (Uint8)((back & 0x000000FF));
	pal[1].r = (Uint8)((fore & 0x00FF0000) >> 16); 
	pal[1].g = (Uint8)((fore & 0x0000FF00) >> 8);
	pal[1].b = (Uint8)((fore & 0x000000FF));
	SDL_SetColors(selected_font, pal, 0, 2);
}
void inprint(SDL_Surface *dst, const char *str, Uint32 x, Uint32 y)
{
	int i, len, w, h;
	len = strlen(str);
	w = selected_font->w / 16;
	h = selected_font->h / 8;
	for (i = 0; i < len; i++) {
		int id = (char)str[i];
		int row = id / 16;
		int col = id - (row * 16);

		SDL_Rect s_rect = { col * w, row * h, w, h }; 
		SDL_Rect d_rect = { x, y, w, h };
		SDL_BlitSurface(selected_font, &s_rect, dst, &d_rect);
		x = x + w;	
	}
}
