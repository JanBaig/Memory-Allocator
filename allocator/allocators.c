
#include <unistd.h>
#include "allocators.h"
#define align4(x) (((((x) -1)>>2)<<2)+4)
#define METADATA_SIZE 20
block* base = NULL; // starting point of the heap

void* mallocImp(size_t size) {
	
	block* b; 
	block* last;
	size_t s = align4(size);
	
	if (base) {
		// The base already exists - need to find a free one 
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
		// No base - 'b' is the first block added to the heap
		b = extend_heap(NULL, s);
		if (!b) { return NULL; }
		base = b; 
	} 
	return b->data; // starting address of block's data region
}

void* callocImp(size_t number, size_t size) {
	size_t s4;
	size_t i; 
	size_t* new = mallocImp(number * size);

	if (new) {
		s4 = align4(number * size);
		for (i=0; i<s4; i++) {
			new[i] = 0;
		}
	}
	return new;
}

void* reallocImp(void* p, size_t size) {
	size_t s;
	block* b;
	block* new;
	void* newp;

	if (!p) { return mallocImp(size); }
	if (valid_addr(p)) {
		s = align4(size);
		b = get_block(p);
		if (b->size >= size) {
			// Need to reduce block size
			if (b->size - s >= (METADATA_SIZE + 4)) { split_block(b, s); }
		} 
		else {
			// Need to increase block size
			if (b->next && b->next->free && (b->size + METADATA_SIZE + b->next->size) >= s) {
				fusion(b);
				if (b->size - s >= (METADATA_SIZE + 4)) { split_block(b, s); }

			} else {
				newp = mallocImp(s);
				if (!newp) { return NULL; }
				new = get_block(newp); // we're given the address of the data segment, we need to find the starting address of the block
				copy_block(b, new);
				freeImp(p); // free the old one
				return newp;
			}
		}
		return p;
	}
	return NULL;
}

void freeImp(void* p) {
	block* b;
	if (valid_addr(p)) {
		b = get_block(p);
		b->free = 1;
		// Fuse with the previous block if possible
		if (b->prev && b->prev->free) { b = fusion(b->prev); }
		if (b->next) { fusion(b); }
		else {
			// b does not have a next - we are at the end of the heap
			if (b->prev) { b->prev->next = NULL; }
			else { base = NULL; } // if no prev, there is only one block and we free that 
			brk(b);
		}
	}
}

// Helper Functions

block* extend_heap(block* last, size_t s) {
	
	block* b = sbrk(0); // address returned will hold a block
	int sb = (int)sbrk( METADATA_SIZE + s); // increment program break

	if (sb == -1) { return (NULL); } // error returned
	b->size = s;
	b->next = NULL;
	b->prev = last;
	b->ptr = b->data;
	if (last) { last->next = b; }
	b->free = 0;
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
	new = (block*)(b->data + s); // starting address of the new block - b->data is the end of metadata 
	new->size = b->size - s - METADATA_SIZE; // size of the new's data portion 
	new->next = b->next;
	new->prev = b;
	new->free = 1;
	new->ptr = new->data;
	b->size = s;
	b->next = new;
	if (new->next) { new->next->prev = new; }
}

block* fusion(block* b) {
	if (b->next && b->next->free) {  
		b->size += METADATA_SIZE + b->next->size;
		b->next = b->next->next; 
		if (b->next) { b->next->prev = b; }
	}	
	return b;
}

block* get_block(void* p) {
	char* tmp = p;
	return p = tmp -= METADATA_SIZE;
} 

int valid_addr(void* p) {
	// a blocks->ptr holds the start of the data, subtracting the metadata size from it means that we arrive
	// at the start of the block and check to see if the argument p is equal to the returned address
	if (base) {
		if (p>base && p<sbrk(0)) { return p == (get_block(p))->ptr; }
	}
	return 0;
}

void copy_block(block* src, block* dst) {
	int* sdata;
	int* ddata;
	size_t i;

	sdata = src->ptr;
	ddata = dst->ptr;
	for (i=0; (i*4 < src->size) && (i*4 < dst->size); i++) {
		ddata[i] = sdata[i];
	}
}

