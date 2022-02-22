#include "SDL2/SDL.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include <bits/stdint-intn.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"
#include "memory.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#define STBI_MALLOC SDL_malloc
#define STBI_REALLOC SDL_realloc
#define STBI_FREE SDL_free
#include "stb_image.h"

//global stuff
global char* assetDir = NULL;
global SDL_Renderer* renderer;

// TODO: have memManager point to current scene memManager
//       so asset loading can be tied to scene and not global
global hMemManager assetManager;

char*
getEnvVariableP( const char *variable){
	#if defined (_WIN32)

	char* data;
	size_t rsize = 0;
	
	getenv_s(&rsize, NULL, 0, variable);

	if (!rsize){
		SDL_Log("Env variable[%s] not found", variable);
		return NULL;
	}
	
	data = (char  *)malloc(rsize * sizeof(char));
	getenv_s(&rsize, data, rsize, variable);
	return data;

	#elif defined(__linux__)

	return getenv(variable);
	
	#else
	
	return getenv(variable);
	
	#endif
}



void
assets_init(SDL_Renderer* nRenderer){
	//assetManager = mem_createManager();
	
	renderer = nRenderer;
	char* assetDirectory = NULL;

	// try to get the default asset directory from environment
	assetDirectory = getEnvVariableP("REDASSETDIR");

	
	// if the env variable is NULL default to cwd
	if (!assetDirectory) {
		assetDirectory = getcwd();

		if (!assetDirectory){
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot get a path to assets: %s", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	}
	// if everything succeeds store in global
	assetDir = assetDirectory;
	SDL_Log("assetDir >> %s\n", assetDir);
}

void assets_cleanup(){
	mem_destroyManager(assetManager);
}


const char *
getFilePathP(const char *filename){
	size_t s = strlen(assetDir) + strlen(filename) + 1;//add place for null
	char *dest = SDL_malloc(s * sizeof(uint8_t));
	#if defined (_WIN64)
	strcpy_s(dest, s, assetDir);
	strcat_s(dest, s, filename);
	#else
	strcpy(dest, assetDir);
	strcat(dest, filename);
	#endif
	return dest;
}



SDL_Surface *
loadSurfaceFromStbi(void *rawImageData, int32_t width, int32_t height, int32_t bytesPerPixel){

	// Calculate pitch
	int pitch;
    pitch = width * bytesPerPixel;
    pitch = (pitch + 3) & ~3;

    // Setup relevance bitmask
	int32_t Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    Rmask = 0x000000FF;
    Gmask = 0x0000FF00;
    Bmask = 0x00FF0000;
    Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
#else
    int s = (bytesPerPixel == 4) ? 0 : 8;
    Rmask = 0xFF000000 >> s;
    Gmask = 0x00FF0000 >> s;
    Bmask = 0x0000FF00 >> s;
    Amask = 0x000000FF >> s;
#endif
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
									 rawImageData, width, height, bytesPerPixel*8,
									 pitch, Rmask, Gmask,  Bmask, Amask);
	if (!surface){
		//free the data if it fails
		SDL_free(rawImageData);
		rawImageData = NULL;
		return NULL;
	}
	// todo: check if need to free original data
	return surface;
}


//export
hTexture
assets_loadTexture(const char* filename){
	char *path = (char*)getFilePathP(filename);

	int32_t width, height, bytesPerPixel;
	void*	rawImageData = rawImageData = stbi_load(path, &width, &height, &bytesPerPixel, 0);
	
	SDL_Surface * surface = loadSurfaceFromStbi(rawImageData, width, height, bytesPerPixel);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	// TODO: store textures array for easy free later
	
	SDL_FreeSurface(surface);
	free(path);
	free(rawImageData);
	return (hTexture) texture;
}


// export

rect
assets_getTextureRect(hTexture texture){
	SDL_Texture *sdlTex = (SDL_Texture *) texture;
	int w, h;

	SDL_QueryTexture(sdlTex, NULL, NULL, &w, &h);

	return (rect){0,0,w,h};
}
