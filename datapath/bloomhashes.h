#ifndef BLOOM_HASHES_H
#define BLOOM_HASHES_H

#include <linux/types.h>

/**
 * Generates a hash value for use in bloom filtering via the following formula:
 * g(key) = (h1(key) + [hash_index * h2(key)) mod filter_len
 * where h1 is the murmurhash2_neutral hash function, and
 * h2 is the lookup3_little hash function
 * key - Pointer to the start of the array to be hashed
 * key_len - Length of the array to be hashed (as an integer number of bytes)
 * hash_index - Hash index to be used in the above formula
 * filter_len - Length of the bloom filter for which this hash will be used (in bits)
 */
uint32_t bloom_hashcode(const void* key, size_t key_len, uint16_t hash_index, uint16_t filter_len);

/**
 * Convenience function to apply bloom_hashcode to a unsigned 16 bit integer key
 */
uint32_t bloom_hashcode_uint16key(const uint16_t* key, uint16_t hash_index, uint16_t filter_len);

#endif
/* BLOOM_HASHES_H */



