#pragma once
#include "SDL2/SDL.h"
#include "macros.h"
#include "types.h"

typedef struct display_s {
	SDL_Window *window;
	SDL_Renderer *screen;
	SDL_RendererInfo renderer;
	SDL_Rect screenRect;
} display;

void render_startDrawing();

void render_endDrawing();

void
render_sprite(hTexture texture, rect guide, rect dest, int frame);

void render_texture(hTexture texture, rect destination);

display render_createDisplay();

