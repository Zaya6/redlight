#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_filesystem.h"
#include "SDL2/SDL_log.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include <stdbool.h>
#include "macros.h"
#include "types.h"
#include "render.h"

global display mainDisplay = {0};

display render_createDisplay(){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init fail!: %s\n", SDL_GetError() );
		exit(EXIT_FAILURE);
	}

	mainDisplay.window = SDL_CreateWindow("Red Light",
									  SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
									  1280,720, SDL_WINDOW_SHOWN);
	if (!mainDisplay.window) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Window fail!: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Get window surface
	// TODO: Consider setting vsync
	mainDisplay .screen = SDL_CreateRenderer( mainDisplay .window, -1,
															SDL_RENDERER_ACCELERATED );

	// get drawable area
	SDL_GetRendererOutputSize(mainDisplay .screen,
									  &mainDisplay.screenRect.w,
									  &mainDisplay.screenRect.h);

	// get the render info
	SDL_GetRendererInfo(mainDisplay.screen, &mainDisplay.renderer);
	SDL_Log("Using: %s", mainDisplay .renderer.name);
	
	//Fill the surface black
	SDL_SetRenderDrawColor(mainDisplay.screen, 0, 0, 0, 255);
	SDL_RenderClear(mainDisplay.screen);
	
	//display screen
	SDL_RenderPresent(mainDisplay .screen);

	return mainDisplay;
}


void
render_texture(hTexture texture, rect destination ){
	SDL_RenderCopy(mainDisplay.screen, (SDL_Texture*)texture,
						NULL, &destination);
}


void
render_sprite(hTexture texture, rect guide, rect dest, int frame){
	rect cutout = guide;
	int width, height;
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, &width, &height);

	// find limit to frams in both axis for wrap around
	int limitx = width / guide.w;
	int limity = height / guide.h;

	// add on frames and wrap around texture
	int framex = guide.x + frame;
	int framey = guide.y + frame / limitx;
	framex %= limitx;
	framey %= limity;
	
	//translate  frames into pixels
	cutout.x = framex * guide.w; 
	cutout.y = framey * guide.h;
	
	//SDL_Log("[%d,%d]",framex,framey);
	SDL_RenderCopy(mainDisplay.screen,
						(SDL_Texture*)texture,
						&cutout, &dest);
	
}


void
render_startDrawing(){
	SDL_SetRenderDrawColor(mainDisplay.screen, 0, 0, 0, 255);
	SDL_RenderClear(mainDisplay.screen);

}


void
render_endDrawing(){
	SDL_RenderPresent(mainDisplay.screen);
	//update the window
		SDL_UpdateWindowSurface(mainDisplay.window);
}
