#ifndef _MEM_ALLOC_H
#define _MEM_ALLOC_H

#include <stdlib.h>
#include "ssmem.h"
#include "ssalloc.h"
#define MEM_MAX_ALLOCATORS 4

void memalloc_init(int id);
void memalloc_term();
void* memalloc_alloc(size_t size);
void memalloc_free(void *ptr);

/* 
 * Functions to be used when several allocation sizes are required. 
*/
void memalloc_init_idx(unsigned int index, int id);
void memalloc_term_idx(unsigned int index);
void* memalloc_alloc_idx(unsigned int index, size_t size);
void memalloc_free_idx(int index, void *ptr);

void memalloc_unsafe_to_reclaim();
void memalloc_safe_to_reclaim();
#endif
