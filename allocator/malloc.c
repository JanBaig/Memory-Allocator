
#include <unistd.h>
#include "malloc.h"
#define align4(x) (((((x) -1)>>2)<<2)+4)
#define METADATA_SIZE sizeof(struct block)
block* base;

block* extend_heap(block* last, size_t s) {
	
	block* b = sbrk(0); // initial address of break
	if (sbrk(METADATA_SIZE + s) == (void*)-1) { return NULL; }
	b->size = s;
	b->next = NULL;
	b->free = 0;
	if (last) { last->next = b; }
	return b;
}

block* find_block(block* last, size_t size) {
	block* b = base;
	while (b && !(b->free && b->size >= size)) { 
       		last = b;
		b = b->next;
	}
	return b;
}

void split_block(block* b, size_t s) {
	
	block* new;
	new = b->data + s; // starting address of the new block - b->data is the end of metadata 
	new->size = b->size - s - METADATA_SIZE; // size of the new's data portion 
	new->next = b->next;
	new->free = 1;
	b->size = s;
	b->next = new;
}

void* mallocImp(size_t size) {
	
	block* b; 
	block* last;
	size_t s = align4(size);
	
	if (base) {
		// Blocks already exist - need to find a free one 
		last = base;
		b = find_block(last, s);
		if (b) {
			// Check if we can split it 
			if ((b->size - s) >= (METADATA_SIZE + 4)) { split_block(b, s); }
			b->free = 0;

		} else {
			// No empty fitting block found - extend the heap
			b = extend_heap(last, s);
			if (!b) { return NULL; }
		}
	
	} else {
		// First block added to the heap
		b = extend_heap(NULL, s);
		if (!b) { return NULL; }
		base = b; 
	} 
	return b->data;
}

