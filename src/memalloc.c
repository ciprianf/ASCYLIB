#include "memalloc.h"
#include <assert.h>

#if GC == 1
	#include "ssmem.h"
	extern __thread ssmem_allocator_t* allocs[MEM_MAX_ALLOCATORS];
#elif GC == 2
	#include "gcmalloc.h"
#else
	#include "ssalloc.h"
#endif

void memalloc_init(int id) {
	memalloc_init_idx(0, id);
}

void memalloc_term() {
	memalloc_term_idx(0);
}

void* memalloc_alloc(size_t size) {
	return memalloc_alloc_idx(0, size);
}

void memalloc_free(void *ptr) {
	memalloc_free_idx(0, ptr);
}

void memalloc_init_idx(unsigned int index, int id) {
	#if GC == 1
		allocs[index] = (ssmem_allocator_t*) malloc(sizeof(ssmem_allocator_t));
		assert(allocs[index] != NULL);
		ssmem_alloc_init_fs_size(allocs[index], SSMEM_DEFAULT_MEM_SIZE,
				SSMEM_GC_FREE_SET_SIZE, id);
	#elif GC == 2
		;
	#else
		ssalloc_init();
	#endif
}


void memalloc_term_idx(unsigned int index) {
	#if GC == 1
		ssmem_term();
		free(allocs[index]);
	#elif GC == 2
		;
	#else
		;
	#endif
}

void* memalloc_alloc_idx(unsigned int index, size_t size) {
	#if GC == 1
		return ssmem_alloc(allocs[index], size);
	#elif GC == 2
	 	return gcmem_malloc(size);
	#else
		return ssalloc(size);
	#endif
}

void memalloc_free_idx(int index, void *ptr) {
	#if GC == 1
		ssmem_free(allocs[index], ptr);
	#elif GC == 2
		gcmem_free(ptr);
	#else
		//ssfree(ptr);
	#endif
}

void memalloc_unsafe_to_reclaim() {
	#if GC == 2
		gcmem_unsafe_to_reclaim();
	#endif
}
void memalloc_safe_to_reclaim() {
	#if GC == 2
		gcmem_safe_to_reclaim();
	#endif
}
