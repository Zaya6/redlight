#include "SDL2/SDL_render.h"
#include "macros.h"
#include "types.h"

void
assets_init(SDL_Renderer *renderer);

hTexture
assets_loadTexture(const char *filename);

rect
assets_getTextureRect(hTexture texture);
