#include "SDL2/SDL_log.h"
#include "macros.h"
#include "types.h"
#include "memory.h"
#include <stddef.h>
#include <stdint.h>

// TODO: maybe find platform independent way to get the system page size
#define MEM_PAGE_SIZE 1024


typedef struct memWrap {
	void *memory;
	//size_t index;
	freeFunction destroy;
} memWrap;

typedef struct memManager {
   memWrap *stack;
	size_t size;
	size_t count;
} memManager;


global memManager centralManager;
// TOD0: Make custom memory managment with mmap




//// Internal functions

void memManagerDestroy(void *mem){
	memManager *a = (memManager*)mem;

	// free all assets in stack using their specified destructor
	for (size_t i = 0; i < a->count; i++) {
		memWrap *wrapper = a->stack + i;
		wrapper->destroy(wrapper->memory);
	}
	
	free(a->stack);
	a->stack = NULL;

	free(a);
	a = NULL;
}


// TODO: Realloc might change pointers and might mess up pointers of memWraps
//       look into later. Currently resizing the stack will break all memwraps
//       pointing inside it. 
//
// maybe: Make memwrap point to the stack with a pointer to the pointer+offset. It
// becomes more complicated but then the stack can be reallocated without
// problems.

enum errorType
checkAndResize(memManager *a, size_t sizeCheck) {
	
	// check for overflow and resize
	if (sizeCheck > a->size){
		size_t newSize = a->size + MEM_PAGE_SIZE;
		void *result = realloc(a->stack, sizeof(uintptr_t)*newSize);

		//check for error gaurd
		if(!result){
			return allocError;
		}

		//no error
		a->stack = result;
		a->size = newSize;
	}

	//TODO: check if memory can be reduced

	return noError;
}

handle
pushToMem(memManager *mem, void *obj, freeFunction F) {
	memWrap wrapper = (memWrap){
		.memory = obj,
		//.index = mem->count,
		.destroy = F
	};

	memWrap *ptr = mem->stack+mem->count;
	*ptr = wrapper;

	mem->count++;

	return (handle)ptr;
}


// Destroys an old memory wrapper and replaces it's location in the memManager's
// stack with another
//
// WARNING: *oldwrap's .memory pointer should already be freed or a leak will
// result.
void
shiftMemWrap(memWrap *oldWrap, memWrap *reWrap){
	
	oldWrap->memory = reWrap->memory;
	oldWrap->destroy = reWrap->destroy;
	
	// zero out the old wrapper
	*reWrap = (memWrap){0}; 
	
}

errorType
destroyMemoryObj(memManager man, handle hObj){
	// gaurd against using empty manager
	if (man.count == 0) return indexError;
	
	// get the memwrap and destroy its memory with its built in destroyer
	memWrap *m = (memWrap*) hObj;
	m->destroy(m->memory);

	// zeroing out memory is better	
	*m = (memWrap){0};
	
	// shift last asset to this location if more than 1 thing exists in stack
	if (man.count > 1){
		SDL_Log("here");
		shiftMemWrap(m, &man.stack[man.count-1]);
		
	}
	
	// decrement count of allocated objects
	man.count--;
	return noError;
}

//// Exported functions

hMemManager
mem_createManager() {
	checkAndResize(&centralManager, centralManager.count+1);
	
	memManager *a = malloc(sizeof(memManager));
	a->stack = malloc(sizeof(uintptr_t) * MEM_PAGE_SIZE);
	a->size = MEM_PAGE_SIZE;
	a->count = 0;


	handle h = pushToMem(&centralManager, a, memManagerDestroy);
	return (hMemManager)h;
}


errorType 
mem_destroyManager(hMemManager mem){
	return destroyMemoryObj(centralManager, mem);	
}
