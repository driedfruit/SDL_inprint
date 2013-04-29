#include <SDL.h>

#include "inline_font.h" /* Actual font data */

#define CHARACTERS_PER_ROW    16 /* I like 16 x 8 fontsets. */
#define CHARACTERS_PER_COLUMN 8  /* 128 x 1 is another popular format. */

static SDL_Renderer *selected_renderer = NULL;
static SDL_Texture *inline_font = NULL;
static SDL_Texture *selected_font = NULL;
static Uint16 selected_font_w, selected_font_h;

void prepare_inline_font()
{
	Uint8 *pix_ptr, tmp;
	int i, len, j;
	SDL_Surface *surface, *surface32;
	Uint32 rmask, gmask, bmask, amask;

	selected_font_w = inline_font_width;
	selected_font_h = inline_font_height;

	if (inline_font != NULL) { selected_font = inline_font; return; }

	surface = SDL_CreateRGBSurface(0, inline_font_width, inline_font_height, 8, 0, 0, 0, 0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	surface32 = SDL_CreateRGBSurface(0, inline_font_width, inline_font_height, 32, rmask, gmask, bmask, amask);

	/* Get pointer to pixels and array length */
	pix_ptr = (Uint8 *)surface->pixels;
	len = surface->h * surface->w / 8;

	/* Copy */
	for (i = 0; i < len; i++) 
	{
		tmp = (Uint8)inline_font_bits[i];
		for (j = 0; j < 8; j++)
		{
			Uint8 mask = (0x01 << j);
			pix_ptr[i * 8 + j] = ((tmp & mask) ? 1 : 0);
		}
	}

	SDL_SetColorKey(surface, SDL_TRUE, 1);
	SDL_FillRect(surface32, NULL, 0x00000000);
	SDL_BlitSurface(surface, NULL, surface32, NULL);

	inline_font = SDL_CreateTextureFromSurface(selected_renderer, surface32);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(surface32);

	selected_font = inline_font;
}
void kill_inline_font(void) {
	SDL_DestroyTexture(inline_font);
	inline_font = NULL; 
}
void inrenderer(SDL_Renderer *renderer) 
{
	selected_renderer = renderer;
}
void infont(SDL_Texture *font)
{
	Uint32 format;
	int access;
	int w, h;

	if (font == NULL) return prepare_inline_font();

	SDL_QueryTexture(font, &format, &access, &w, &h);

	selected_font = font;
	selected_font_w = w;
	selected_font_h = h;
}
void incolor1(SDL_Color *color)
{
	SDL_SetTextureColorMod(selected_font, color->r, color->g, color->b);
}
void incolor(Uint32 fore, Uint32 unused) /* Color must be in 0x00RRGGBB format ! */
{
	SDL_Color pal[1];
	pal[0].r = (Uint8)((fore & 0x00FF0000) >> 16); 
	pal[0].g = (Uint8)((fore & 0x0000FF00) >> 8);
	pal[0].b = (Uint8)((fore & 0x000000FF));
	SDL_SetTextureColorMod(selected_font, pal[0].r, pal[0].g, pal[0].b);
}
void inprint(SDL_Renderer *dst, const char *str, Uint32 x, Uint32 y)
{
	SDL_Rect s_rect;
	SDL_Rect d_rect;

	s_rect.w = selected_font_w / CHARACTERS_PER_ROW;
	s_rect.h = selected_font_h / CHARACTERS_PER_COLUMN;

	if (dst == NULL) dst = selected_renderer;

	for (; *str; str++)
	{
		int id = (int)*str;
#if (CHARACTERS_PER_COLUMN != 1)
		int row = id / CHARACTERS_PER_ROW;
		int col = id % CHARACTERS_PER_ROW;
		s_rect.x = col * s_rect.w;
		s_rect.y = row * s_rect.h;
#else
		s_rect.x = id * s_rect.w;
		s_rect.y = 0;
#endif
		d_rect.x = x;
		d_rect.y = y;
		d_rect.w = s_rect.w;
		d_rect.h = s_rect.h;

		SDL_RenderCopy(dst, selected_font, &s_rect, &d_rect);

		x = x + s_rect.w;
	}
}
SDL_Texture *get_inline_font(void) { return selected_font; }
