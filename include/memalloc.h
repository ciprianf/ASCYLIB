#ifndef _MEM_ALLOC_H
#define _MEM_ALLOC_H

#include <stdlib.h>
#define MEM_MAX_ALLOCATORS 4

void memalloc_init(unsigned int index, int id);
void memalloc_term(unsigned int index);
void* memalloc_alloc(unsigned int index, size_t size);
void memalloc_free(int index, void *ptr);

void memalloc_unsafe_to_reclaim();
void memalloc_safe_to_reclaim();
#endif
