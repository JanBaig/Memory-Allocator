
typedef struct block block;

struct block {
	size_t size; // size of the data area - not the whole block's size 
	block* next;
	block* prev;
	int free; // 1 if free, 0 is not
	void* ptr; // points to where data segment of block starts (size attribute)
	char data [1]; // indicates the end of the metadata, address of start of data
};

void* mallocImp(size_t size);

void* callocImp(size_t number, size_t size);

void* reallocImp(void* p, size_t size);

void freeImp(void* p);

// Helper Functions

block* fusion(block* b);

block* get_block(void* p);

int valid_addr(void* p);

void copy_block(block* src, block* dst);

block* extend_heap(block* last, size_t s);

block* find_block(block* last, size_t size); 

void split_block(block* b, size_t s); 

