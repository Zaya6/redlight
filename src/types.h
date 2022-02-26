#pragma once
#include "SDL2/SDL_rect.h"
#include <stdint.h>
#include <stddef.h>

typedef enum assetType{
	allocatorType,
	textureType,
	mapType,
	rectType,
	soundType
} assetType;

typedef enum errorType {
	noError,
	allocError,
	indexError,
	nullError
} errorType;

// TODO: find way to remove all sdl stuff
// from redlight game code

typedef void (*freeFunction)(void *memory);

typedef SDL_Rect rect;
typedef void* handle;
typedef handle hTexture;
typedef handle hMap;
typedef handle hMemManager;


