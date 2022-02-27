#include "SDL2/SDL_log.h"
#include "macros.h"
#include "types.h"
#include "mem.h"

// TODO: maybe find platform independent way to get the system page size
#define MEM_STACK_SIZE 1024


typedef struct memWrap {
	handle memory;
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


hMemManager
mem_createManager() {
	memManager* man = malloc(sizeof(memManager));
	man->stack = malloc(sizeof(memWrap*) * MEM_STACK_SIZE);
	man->size = MEM_STACK_SIZE;
	man->count = 0;

   return (hMemManager)(void*)man;
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
	if (!manager){
		// gaurd
		if (!currentManager) return 0;
		manager = currentManager;
	}
	
	if (manager->count+1) {
		// TODO: increase stack size
	} 

	// get the tail location of the memManager stack
	memWrap* newSpace = manager->stack[manager->count++];
	// create new memwrap for 
	newSpace = malloc(sizeof(memWrap));
	newSpace->memory = address;
	newSpace->destroy = destroy;
	return (handle)newSpace;
}

void
mem_test() {
	// make a manager
	hMemManager man = mem_createManager();
	currentManager = (memManager*)man;

	// make something to store
	long* thing = malloc(sizeof(long));
	*thing = 999;

	handle hThing = mem_store(thing, &free, NULL);


	//long* thingBack = (long*)hThing;
	SDL_Log("mem -> thing is [%p], hThing is [%p]", thing, (long*)hThing);
	SDL_Log("mem -> it stores [%ld]", *((long*)hThing));
}