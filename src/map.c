
#include "macros.h"
#include "types.h"
#include <stdint.h>

typedef struct map{
	rect base;
	hTexture tilemap;
	uint32_t *data;
} map;

hMap
map_create(int x, int y, int width, int height, hTexture tilemap){
	map *m = malloc(sizeof(map));
	m->base = (rect){
		x,y,width,height
	};
	m->tilemap = tilemap;
	m->data = malloc(width * height * sizeof(uint32_t));
	
	return (hMap)m;
}


void
map_destroy(hMap oMap){
	// turn back to pointer
	map *m = (map*)oMap;

	// free map data and map itself
	free(m->data);
	free(m);
}

