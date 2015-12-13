#ifndef _GCMALLOC_H_
#define _GCMALLOC_H_
#include <stdlib.h>

/* Allocate an object of specified size. */
void* gcmem_malloc(size_t size);

/*
 * Free an object. It is guaranteed that the object is buffered until
 * it is safe to actually reclaim memory.
 */
void gcmem_free(void* ptr);

/*
 * Informs gcmalloc that the thread enters a critical section and no
 * reclamation can be performed, starting from now, on new freed objects.
 */
void gcmem_unsafe_to_reclaim();

/*
 * Informs gcmalloc that the thread leaves a critical section and
 * will not access a shared object. Should not be called without a previous,
 * corresponding gcmem_unsafe_to_reclaim().
 */
void gcmem_safe_to_reclaim();

/* Usage example:

thread_function() {
	...
	gmcmem_unsafe_to_reclaim();
	...
	void *object = extract_object_from_shared_data_strucutre();
	gcmem_free(object);
	...
	gmcmem_safe_to_reclaim();
	...
}
*/

#endif
