/*
 * CS 2110 Fall 2018
 * Author: Bojun Yang
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"
/* include this for any boolean methods */
#include <stdbool.h>

/*Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */

// static metadata_t* find_right(metadata_t*);
// static metadata_t* find_left(metadata_t*);
// static void merge(metadata_t* left, metadata_t* right);
// static void double_merge(metadata_t* left, metadata_t* middle, metadata_t* right);
static metadata_t* split_block(metadata_t* block, size_t size);
static void add_to_address_list(metadata_t* add_block);
static void add_to_size_list(metadata_t* add_block);
static void remove_from_address_list(metadata_t* remove_block);
static void remove_from_size_list(metadata_t* remove_block);
static void set_canary(metadata_t* block);
static void nullPointers(metadata_t* block);
// static metadata_t* sbrkReturnBlock(size_t size);

/* Our freelist structure - our freelist is represented as two doubly linked lists
 * the address_list orders the free blocks in ascending address
 * the size_list orders the free blocks by size
 */

metadata_t *address_list;
metadata_t *size_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;

/* MALLOC
 * See my_malloc.h for documentation
 */
void *my_malloc(size_t size) {
    if (size == 0) {
        my_malloc_errno = NO_ERROR;
        return NULL;
    }
    size_t totalNeededSize = size + TOTAL_METADATA_SIZE + MIN_BLOCK_SIZE; // total needed size
    size_t totalSize = size + TOTAL_METADATA_SIZE;
    if (totalNeededSize > SBRK_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }

    int iterateThroughSizeList = 1;
    int perfect = 0;
    metadata_t* currBlock = size_list;
    if (address_list == NULL && size_list == NULL) {
        iterateThroughSizeList = 0;
        currBlock = NULL;
    }
    while (iterateThroughSizeList) {
        if (totalSize <= currBlock -> size) { // maybe right block
            if (totalSize == currBlock -> size) { // check if perfect
                iterateThroughSizeList = 0;
                perfect = 1;
            } else if (totalNeededSize <= currBlock -> size) { // if not perfect, check if big enough
                iterateThroughSizeList = 0;
            } else { // not big enough so go on to next
                if (currBlock -> next_size != NULL) {
                    currBlock = currBlock -> next_size;
                } else {
                    iterateThroughSizeList = 0;
                    currBlock = NULL;
                }
            }
        } else {
            if (currBlock -> next_size != NULL) { // if there are still blocks left in the freelist
                currBlock = currBlock -> next_size;
            } else { // if no more blocks
                iterateThroughSizeList = 0;
                currBlock = NULL;
            }
        }
    }
    if (currBlock == NULL) {
        // mysbrk new block
        metadata_t* newBlock = my_sbrk(SBRK_SIZE);
        if (newBlock == NULL) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        }
        newBlock -> size = SBRK_SIZE;
        add_to_address_list(newBlock);
        add_to_size_list(newBlock);
        //merge if possible
        iterateThroughSizeList = 1;
        currBlock = size_list;
        while (iterateThroughSizeList) {
            if (totalNeededSize <= currBlock -> size) { // if not perfect, check if big enough
                iterateThroughSizeList = 0;
            } else {
                if (currBlock -> next_size != NULL) { // if there are still blocks left in the freelist
                    currBlock = currBlock -> next_size;
                }
            }
        }
        // same as split block
        metadata_t* ret_block = split_block(currBlock, size);
        nullPointers(ret_block);
        set_canary(ret_block);
        my_malloc_errno = NO_ERROR;
        return ret_block + 1;
    } else if (perfect) {
        // currBlock is perfect size
        remove_from_address_list(currBlock);
        remove_from_size_list(currBlock);
        nullPointers(currBlock);
        set_canary(currBlock);
        my_malloc_errno = NO_ERROR;
        return currBlock + 1;
    } else {
        // big enough block but need to split it
        metadata_t* ret_block = split_block(currBlock, size);
        nullPointers(ret_block);
        set_canary(ret_block);
        my_malloc_errno = NO_ERROR;
        return ret_block + 1;
    }

    // // if freelist is empty
    // if (address_list == NULL && size_list == NULL) {
    //     metadata_t* newBlock = my_sbrk(SBRK_SIZE);
    //     metadata_t* userBlock = (metadata_t*) ((uint8_t*) newBlock + (SBRK_SIZE - (size + TOTAL_METADATA_SIZE)));
    //     userBlock -> size = totalNeededSize - MIN_BLOCK_SIZE;
    //     userBlock -> prev_addr = NULL;
    //     userBlock -> next_addr = NULL;
    //     userBlock -> next_size = NULL;
    //     userBlock -> prev_size = NULL;
    //     set_canary(userBlock);
    //     //newBlock shit what's under here is probably wrong
    //     newBlock -> prev_addr = NULL;
    //     newBlock -> next_addr = NULL;
    //     newBlock -> prev_size = NULL;
    //     newBlock -> next_size = NULL;
    //     newBlock -> size = SBRK_SIZE - userBlock -> size;
    //     set_canary(newBlock);
    //     add_to_address_list(newBlock);
    //     add_to_size_list(newBlock);
    // } else {
    //     // if freelist not empty
    //     int go = 1;
    //     int found = 0;
    //     metadata_t *sizeBlock = size_list;
    //     while (go) {
    //         if (sizeBlock -> size >= totalNeededSize) {
    //             go = 0;
    //             found = 1;
    //         } else {
    //             sizeBlock = sizeBlock -> next_size;
    //         }
    //     }
    //     if (found) {
    //         // perfect block is sizeBlock
    //         remove_from_size_list(sizeBlock);
    //         remove_from_address_list(sizeBlock);
    //         sizeBlock -> prev_addr = NULL;
    //         sizeBlock -> next_addr = NULL;
    //         sizeBlock -> prev_size = NULL;
    //         sizeBlock -> next_size = NULL;
    //         return sizeBlock + 1;
    //     } else {
    //         // Need to split
    //     }
    // }
    return NULL;
}

/* REALLOC
 * See my_malloc.h for documentation
 */
void *my_realloc(void *ptr, size_t size) {
	UNUSED_PARAMETER(ptr);
	UNUSED_PARAMETER(size);
	return (NULL);
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
	UNUSED_PARAMETER(nmemb);
	UNUSED_PARAMETER(size);
	return (NULL);
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void *ptr) {
	UNUSED_PARAMETER(ptr);
}

// static metadata_t* find_right(metadata_t*) {
//
// }
//
// static metadata_t* find_left(metadata_t*) {
//
// }
//
// static void merge(metadata_t* left, metadata_t* right) {
//
// }
//
// static void double_merge(metadata_t* left, metadata_t* middle, metadata_t* right) {
//
// }
//
/*
 * returns the pointer to the block with size size to be split off from block
*/
static metadata_t* split_block(metadata_t* block, size_t size) {
    metadata_t* ret_block = (metadata_t*) ((uintptr_t*) block + (block -> size - (size + TOTAL_METADATA_SIZE)));
    ret_block -> size = size + TOTAL_METADATA_SIZE;
    // block -> size = 1864;
    remove_from_size_list(block);
    block -> size = (block -> size) - (ret_block -> size);
    add_to_size_list(block);
    // remove_from_address_list(block);
    // add_to_address_list(block);
    return ret_block;
}

static void add_to_address_list(metadata_t* add_block) {
    if (address_list == NULL) {
        address_list = add_block;
    } else {
        int iterateThroughAddressList = 1;
        int tail = 0;
        metadata_t* currBlock = address_list;
        while (iterateThroughAddressList) {
            if ((uintptr_t*) add_block <= (uintptr_t*) currBlock) {
                iterateThroughAddressList = 0;
            } else {
                if (currBlock -> next_addr != NULL) {
                    currBlock = currBlock -> next_addr;
                } else {
                    iterateThroughAddressList = 0;
                    tail = 1;
                }
            }
        }
        if (tail) { // add at tail
            currBlock -> next_addr = add_block;
            add_block -> prev_addr = currBlock;
            add_block -> next_addr = NULL;
        } else if (currBlock == size_list) { // add at head
            currBlock -> prev_addr = add_block;
            add_block -> next_addr = currBlock;
            add_block -> prev_addr = NULL;
        } else { // add in the middle of the list
            currBlock -> prev_addr -> next_addr = add_block;
            add_block -> prev_addr = currBlock -> prev_addr;
            add_block -> next_addr = currBlock;
            currBlock -> prev_addr = add_block;
        }
    }
}

static void add_to_size_list(metadata_t* add_block) {
    if (size_list == NULL) {
        size_list = add_block;
    } else {
        int iterateThroughSizeList = 1;
        int tail = 0;
        metadata_t* currBlock = size_list;
        while (iterateThroughSizeList) {
            if (add_block -> size <= currBlock -> size) {
                iterateThroughSizeList = 0;
            } else {
                if (currBlock -> next_size != NULL) {
                    currBlock = currBlock -> next_size;
                } else {
                    iterateThroughSizeList = 0;
                    tail = 1;
                }
            }
        }
        if (tail) { // add at tail
            currBlock -> next_size = add_block;
            add_block -> prev_size = currBlock;
            add_block -> next_size = NULL;
        } else if (currBlock == size_list) { // add at head
            currBlock -> prev_size = add_block;
            add_block -> next_size = currBlock;
            add_block -> prev_size = NULL;
        } else { // add in the middle of the list
            currBlock -> prev_size -> next_size = add_block;
            add_block -> prev_size = currBlock -> prev_size;
            add_block -> next_size = currBlock;
            currBlock -> prev_size = add_block;
        }
    }
}

static void remove_from_address_list(metadata_t* remove_block) {
    if (address_list == remove_block) {
        // head
        if (remove_block -> next_addr == NULL) {
            address_list = NULL;
        } else {
            address_list = remove_block -> next_addr;
            remove_block -> next_addr -> prev_addr = NULL;
        }
    } else if (remove_block -> next_addr == NULL) {
        // tail
        remove_block -> prev_addr -> next_addr = NULL;
    } else {
        // middle
        remove_block -> prev_addr -> next_addr = remove_block -> next_addr;
        remove_block -> next_addr -> prev_addr = remove_block -> prev_addr;
    }
}


static void remove_from_size_list(metadata_t* remove_block) {
    if (size_list == remove_block) {
        // head
        if (remove_block -> next_size == NULL) {
            size_list = NULL;
        } else {
            size_list = remove_block -> next_size;
            remove_block -> next_size -> prev_size = NULL;
        }
    } else if (remove_block -> next_size == NULL) {
        // tail
        remove_block -> prev_size -> next_size = NULL;
    } else {
        // middle
        remove_block -> prev_size -> next_size = remove_block -> next_size;
        remove_block -> next_size -> prev_size = remove_block -> prev_size;
    }
}

static void set_canary(metadata_t* block) {
    block -> canary = ((uintptr_t) block ^ CANARY_MAGIC_NUMBER) + 1;
    unsigned long* canary_ptr = (unsigned long*)((uint8_t*) block + (block -> size - sizeof(unsigned long)));
    *canary_ptr = block -> canary;
}

static void nullPointers(metadata_t* block) {
    block -> prev_addr = NULL;
    block -> next_addr = NULL;
    block -> prev_size = NULL;
    block -> next_size = NULL;
}

// static metadata_t* sbrkReturnBlock(size_t size) {
//     metadata_t* newBlock = my_sbrk(SBRK_SIZE);
//     if (newBlock == NULL) {
//         return NULL;
//     }
//     add_to_address_list(newBlock);
//     add_to_size_list(newBlock);
//     // merge if possible
//     size_t totalNeededSize = size + TOTAL_METADATA_SIZE + MIN_BLOCK_SIZE; // total needed size
//     size_t totalSize = size + TOTAL_METADATA_SIZE;
//     int iterateThroughSizeList = 1;
//     // int perfect = 0;
//     metadata_t* currBlock = size_list;
//     while (iterateThroughSizeList) {
//         if (totalSize <= currBlock -> size) { // maybe right block
//             if (totalSize == currBlock -> size) { // check if perfect
//                 iterateThroughSizeList = 0;
//                 // perfect = 1;
//             } else if (totalNeededSize < currBlock -> size) { // if not perfect, check if big enough
//                 iterateThroughSizeList = 0;
//             } else { // not big enough so go on to next
//                 if (currBlock -> next_size != NULL) {
//                     currBlock = currBlock -> next_size;
//                 } else {
//                     iterateThroughSizeList = 0;
//                     currBlock = NULL;
//                 }
//             }
//         } else {
//             if (currBlock -> next_size != NULL) { // if there are still blocks left in the freelist
//                 currBlock = currBlock -> next_size;
//             } else { // if no more blocks
//                 iterateThroughSizeList = 0;
//                 currBlock = NULL;
//             }
//         }
//     }
//     if (currBlock == NULL) {
//         return sbrkReturnBlock(size);
//     } else {
//         return currBlock;
//     }
// }
