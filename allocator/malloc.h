
typedef struct block block;

struct block {
	size_t size;
	block* next;
	int free;
	char data [1];
};

void* mallocImp(size_t size);

block* extend_heap(block* last, size_t s);

block* find_block(block* last, size_t size); 

void split_block(block* b, size_t s); 


