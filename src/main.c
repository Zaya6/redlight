#include "SDL2/SDL.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "macros.h"
#include "types.h"
#include "assets.h"
#include "render.h"

typedef union vec {
	float xy[2];
	double raw;
	struct {
		float x, y;
	};	
} vec;


typedef struct entity {
	vec pos;
	vec velocity;
	hTexture texture;
	rect rect;
} entity;



typedef struct gameState {
	hTexture sprite;
	float frame;
	rect guide;
} gameState;


void
addEntity(entity e, gameState *game) {
	//
}



void drawLoop(display *D, gameState *game, float delta){
	game->frame += delta*12;
	if ( game->frame >= 8)
		game->frame = 0;
	
	render_sprite(game->sprite, game->guide,
					  (rect){100,100,128,128}, game->frame);


}


//**************************************** [ entry ]
int main(int argc, char* argv[]) {
	
	display D = render_createDisplay();

	//get asset dir and check it
	assets_init(D.screen);

	gameState game = {
		.guide = {0,1,64,64}
	};
	
	//get texture
	game.sprite = assets_loadTexture("hero_spritesheet.png");
	
	
	
	int32_t frameTime = (1000 / 60);
	uint64_t oldTicks = SDL_GetTicks64();
	bool exitSignal = false;
	
	//************************************* [ main game loop ]
	while(!exitSignal){
		//time calculations
		uint64_t newTicks = SDL_GetTicks64();
		uint64_t diffTicks = newTicks - oldTicks;
		float delta = diffTicks / 1000.0;
		/* float fps = 1.0 / delta; */

		
		//Event handler
		SDL_Event e;
		//grab all events
		while (SDL_PollEvent(&e)){
			if ( e.type == SDL_QUIT)
				exitSignal = true;
		}

		
		// clear screen
		render_startDrawing();
		
		drawLoop(&D, &game, delta);

		render_endDrawing();

		
		//limit to frame rate of 60 fps
		int32_t frameVariance = diffTicks-frameTime;
		int32_t frameAdjust =  min(max(frameVariance, 0), frameTime);
		
		SDL_Delay( frameTime - frameAdjust );
		oldTicks = newTicks;

		
		
	}
	

	//Cleanup what we're responcible for.
	//let os handle rest
   SDL_DestroyWindow( D.window );
	SDL_DestroyRenderer( D.screen );
	SDL_Quit();

   printf("\n*success*\n");
	return EXIT_SUCCESS;
}




