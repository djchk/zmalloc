#include <iostream>
#include "zmalloc.h"

#define NEW_ALLOC 0x1
#define RE_ALLOC 0x2
#define FREE 0x3

#define error_exit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

typedef struct header_block {
    size_t block_size;
    int free;
    int marker;
    struct header_block *next;
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)
void *mem_pool = NULL;

/*
    In this function we are going to freshly allocate the memory form the system using sbrk,
    If theer is any error in the allocation just return null
    Otherwise set the respctive values of the block
*/
block_header_t *get_space_from_os(size_t size) {
    block_header_t *block = (block_header_t *) sbrk(0);
    void *block_req = sbrk(size + HEADER_SIZE);
    if (block_req == (void *)-1) {
        error_exit("sbrk failed");
    }
    block->block_size = size;
    block->free = 0;
    block->next = NULL;
    block->marker = NEW_ALLOC;
    return block;
}

/*
    Among the different strategies, first fit is choosen due to ease of implementation
*/
block_header_t *first_fit_free_block_search(size_t size) {
    block_header_t *curr = (block_header_t *) mem_pool;
    while (curr != NULL && !(curr->free == 1 && curr->block_size >= size)) curr = curr->next;

    return curr;
}

/*
    Another implementation could be the best fit strategy where we choose such a block in which we can minimize the internal fragmentation.
*/
block_header_t *best_fit_free_block_search(size_t size) {
    block_header_t *curr = (block_header_t *) mem_pool;
    block_header_t *minBlock = NULL;
    while(curr != NULL) {
        if ((curr->free == 1 && curr->block_size >= size) && (minBlock == NULL || curr->block_size < minBlock->block_size)) {
            minBlock = curr;
        }
        curr = curr->next;
    }  
    return minBlock;
}

block_header_t *search_free_block(size_t size, strategy_t strategy) {
    block_header_t *block = NULL;
    switch(strategy) {
        case FIRST_FIT: 
            block = first_fit_free_block_search(size);
            break;
        case BEST_FIT:
            block = best_fit_free_block_search(size);
            break;
    }
    return block;
} 


/*
    Just like a classic ll we appending the given block at the end of memory pool
*/
void append_block_to_pool(block_header_t *block) {
    if (mem_pool == NULL) {
        mem_pool = block;
        return;
    }
    block_header_t *curr = (block_header_t *) mem_pool;
    while(curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = block;
    return;
}

/* 
    Case 1: What if we have a free-block of random size in the pool then we can use our first-fit method to retrive the block depending upon the size
    case 2: Where we don't have the free memory in the pool of required size then weare going to ask memory from the os using sbrk system calls 
*/
void *zmalloc(size_t size, strategy_t strategy) {
    if (size <= 0) return NULL;
    block_header_t *curr_block = NULL;
    curr_block = search_free_block(size, strategy);
    if (curr_block != NULL) {
        curr_block->free = 0;
        curr_block->marker = RE_ALLOC;
        return (void *)(curr_block + 1);
    }

    curr_block = get_space_from_os(size);
    if(curr_block == NULL) return NULL;
    append_block_to_pool(curr_block);
    return (void *)(curr_block + 1);
}

/*
    This free function is created to deallocate the space that is allocated only using the zmalloc funcn thus block will be makred as free
*/
void zfree(void *data_block) {
    if (data_block == NULL) return;
    block_header_t *block = (block_header_t *)data_block - 1;
    if (block->free == 1) return;
    if (!(block->marker == NEW_ALLOC || block->marker == RE_ALLOC)) return;
    block->free = 1;
    block->marker = FREE;
}