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

// globals set to 0 
global memManager masterManager;
global memManager *currentManager;

memManager
mem_createManager() {
	// TODO: convert function to malloc a memManager and return a handle
	//       Then change all the other functions to work with handles
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
mem_store(void* address, freeFunction destroy, memManager* manager) {
	if (!manager)
		// gaurd
		if (!currentManager) return 0;
		
		manager = currentManager;
	
	if (manager->count+1) {
		// TODO: increase stack size
	} 

	// get the tail location of the memManager stack
	memWrap* newSpace = manager->stack[manager->count++];
	SDL_Log("%p", newSpace->memory);
	// *newSpace = (memWrap){
	// 	.memory = address,
	// 	.destroy = destroy
	// };
	// newSpace->memory = address;
	// newSpace->destroy = destroy;
	return (handle)address;//(handle)newSpace->memory;
}

void
mem_test() {
	// make a manager
	memManager man = mem_createManager();
	currentManager = &man;

	// make something to store
	long* thing = malloc(sizeof(long));
	*thing = 999;

	handle hThing = mem_store(thing, &free, NULL);


	//long* thingBack = (long*)hThing;
	SDL_Log("mem -> thing is [%p], hThing is [%p]", thing, (long*)hThing);
}