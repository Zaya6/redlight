#pragma once
// helpers
#include <stdint.h>
#define forRange(am) for (size_t i = 0; i < (am); i++)

#define getcwd SDL_GetBasePath
#define malloc SDL_malloc
#define free SDL_free
#define realloc SDL_realloc

#define global static
