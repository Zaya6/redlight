#pragma once
#include "SDL2/SDL.h"
#include "macros.h"
#include "types.h"

typedef struct renderContext {
	SDL_Window *window;
	SDL_Renderer *screen;
	SDL_RendererInfo renderer;
	SDL_Rect screenRect;
} renderContext;


void 
render_startDrawing();

void 
render_endDrawing();

void
render_sprite(hTexture texture, rect guide, int frame, rect dest);

void render_texture(hTexture texture, rect destination);

renderContext render_createDisplay();

