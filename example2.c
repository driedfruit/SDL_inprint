#include <SDL.h>
#include "SDL2_inprint.h"

int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint32 width = 640;
	Uint32 height = 480;
	Uint32 flags = 0;

	int i; for (i = 1; i < argc; i++) if (!strcasecmp("--fullscreen", argv[i])) flags |= SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	if (SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer) < 0) {
		fprintf(stderr, "Window/Renderer initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	/* For SDL2, "inrenderer" is the first function that must be called */
	inrenderer(renderer);
	prepare_inline_font();

	/* Clear screen */
	SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 255);
	SDL_RenderClear(renderer);

	/* Make color white and print "Hello World" */
	incolor(0xffffff, /* unused */ 0);
	inprint(renderer, "Hello World", 10, 10);

	/* Make color red and print some lines */
	incolor(0xFF0000, 0);
	inprint(renderer, "A QUICK BROWN FOX JUMPS OVER A LAZY DOG!", 10, 30);
	inprint(renderer, "a quick brown fox jumps over a lazy dog?", 10, 60);
	inprint(renderer, "`'.,:;0123456789{}()[]|\\/-+*=!@#$%^&", 10, 90);

	/* Wait for a while */
	for (i = 1000; i > 20; i--) { SDL_RenderPresent(renderer); SDL_Delay(10); }

	/* Cleanup */
	kill_inline_font();
	SDL_Quit();
	return 0;
}
