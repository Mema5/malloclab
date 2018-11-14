/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
#include "config.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Blank",
    /* First member's full name */
    "Mael Madon",
    /* First member's email address */
    "mael.madon@polytechnique.edu",
    /* Second member's full name (leave blank if none) */
    "Palmyra Wurtz",
    /* Second member's email address (leave blank if none) */
    "palmyra.wuertz@polytechnique.edu"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

static int* heapMaxAddress;       // pointer to last legal address in the heap
static int* heapStart;            // pointer to first address in the heap


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    heapMaxAddress = mem_heap_lo() + MAX_HEAP;
    heapStart = mem_heap_lo();
    // initialisation du premier mot pour indiquer toute la place dispo
    *heapStart = (MAX_HEAP / SIZE_T_SIZE) | 1;
    return 0;
}

/* 
 * Naive implementation :
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
// void *mm_malloc(size_t size)
// {
//     int newsize = ALIGN(size + SIZE_T_SIZE);
//     void *p = mem_sbrk(newsize);
//     if (p == (void *)-1)
// 	return NULL;
//     else {
//         *(size_t *)p = size;
//         return (void *)((char *)p + SIZE_T_SIZE);
//     }
// }


/*
 * Find the next free block of size size
 * (first fit implementation)
 */
int *findFreeBlock(int size) {
    int *p = heapStart;

    while((p<heapMaxAddress) 
            && ((*p & 1) || 
                (*p <= size)))
        p = p + (*p & -2); 

    if ( (size < 0) || ((p + size) > heapMaxAddress)) {
        // out of memory
        fprintf(stderr, "ERROR: findFreeBlock failed. Ran out of memory...\n");
        return (int *)-1;
    }
    return p;
}

/* 
 * mm_malloc - Implicit free list implementation
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    int* start = findFreeBlock(newsize);

    // write the size of the block in the first word + bit free = 1
    *start = newsize | 1;

    return (void *) start;
}


/*
 * mm_free - Updating the free bit and coalescing
 */
void mm_free(void *ptr)
{
    int * p = ptr;
    *p = *p & -2;

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














