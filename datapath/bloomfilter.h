#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <linux/types.h>
#include "bloomhashes.h"

struct bloom_filter {
    unsigned char* filter_byte_array;
    uint16_t num_bits;
    uint16_t num_bytes;
    uint16_t num_hash_functions;

    // If non-zero, the bloom_filter is considered
    // to own the filter_byte_array buffer, and will
    // free the buffer when free_bloom_filter() is
    // called.
    unsigned char buffer_ownership;
};

/**
 * Allocates memory for a new bloom filter, with the specified number of bits and
 * number of hash functions.
 *
 * Returns NULL if allocation failed. Otherwise, a pointer to the newly allocated
 * struct is returned.
 */
struct bloom_filter* init_bloom_filter(uint16_t num_bits, int num_hash_functions);

/**
 * Initializes a bloom filter with the specified number of bits and number of hash 
 * functions. The memory in which the bloom filter is stored must already be
 * allocated when using this function.
 *
 * Returns NULL if allocation failed. Otherwise, a pointer to the newly allocated
 * struct is returned.
 */
struct bloom_filter* init_bloom_filter_existing_array(unsigned char* filter_byte_array, uint16_t num_bits, uint16_t num_bytes, int num_hash_functions);

/**
 * Checks if the specified key is present in the bloom filter
 *
 * Returns:
 * 0: Element is not present
 * 1: Element is present (or false positive due to hash collision)
 * -1: Bloom filter is not initialized (i.e. passed struct pointer is NULL)
 */
int bloom_filter_check_member(struct bloom_filter* bloom, uint16_t key);

/**
 * Adds to specified key to the bloom filter
 *
 * Returns:
 * 0: Element was added succesfully
 * -1: Bloom filter is not initialized (i.e. passed struct pointer is NULL)
 */
int bloom_filter_add_member(struct bloom_filter* bloom, uint16_t key);

/**
 * Deallocates memory associated with the passed bloom filter
 *
 * Returns:
 * 0: Bloom filter successfully dealocated
 * -1: Bloom filter is not initialized (i.e. passed struct pointer is NULL)
 */
int free_bloom_filter(struct bloom_filter* bloom);

/**
 * Prints the binary contents of the bloom filter, along with its configuration parameters.
 */
void bloom_filter_debug_print(struct bloom_filter* bloom);

#endif
/* BLOOM_FILTER_H */



