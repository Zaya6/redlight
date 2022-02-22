#include "SDL2/SDL_log.h"
#include "macros.h"
#include "types.h"
#include "mem.h"

// TODO: maybe find platform independent way to get the system page size
#define MEM_PAGE_SIZE 1024


typedef struct memWrap {
	void *memory;
	//size_t index;
	freeFunction destroy;
} memWrap;

typedef struct memManager {
   memWrap **stack;
	size_t size;
	size_t count;
} memManager;

global memManager *currentManager;

memManager
mem_createManager() {
    return (memManager) {
		.stack = malloc(sizeof(uintptr_t) * MEM_PAGE_SIZE),	
		.size = MEM_PAGE_SIZE
		};	
}

errorType
mem_destroyManager(memManager *mem){
	forRange(mem->count) {
		memWrap *w = mem->stack[i];
		if (w->memory)
			w->destroy(w->memory);
	}
	free(mem);
	return noError;
}

handle
mem_store(void * address, freeFunction destroy) {
	if (!currentManager) return 0;
	
	if (currentManager->count+1) {
		// increase stack size
	} 

	memWrap *newSpace = currentManager->stack[currentManager->count++];
	newSpace->memory = address;
	newSpace->destroy = destroy;
	return (handle)newSpace->memory;
}