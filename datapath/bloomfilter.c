#include "bloomfilter.h"
#include <linux/net.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/string.h>


#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

struct bloom_filter* init_bloom_filter(uint16_t num_bits, int num_hash_functions)
{
    size_t num_bytes;
    unsigned char* bloom_filter_byte_array;
    struct bloom_filter* bloom;
    
    // Determine how many bytes must be allocated to store the specified number of bits
    num_bytes = num_bits / 8;
    if(num_bits % 8 > 0) {
        num_bytes += 1;
    }

    // Allocate the byte array which will store the bloom filter
    bloom_filter_byte_array = (unsigned char*)kmalloc(num_bytes, GFP_ATOMIC);
    memset(bloom_filter_byte_array, 0, num_bytes);
    if(bloom_filter_byte_array == NULL) {
        return (struct bloom_filter*)NULL;
    }

    // Allocate the bloom_filter struct, and set its fields accordingly
    bloom = (struct bloom_filter*)kmalloc(sizeof * bloom, GFP_ATOMIC);
    if(bloom == NULL) {
        kfree(bloom_filter_byte_array);
        return (struct bloom_filter*)NULL;
    }

    bloom->filter_byte_array = bloom_filter_byte_array;
    bloom->num_bits = num_bits;
    bloom->num_bytes = num_bytes;
    bloom->num_hash_functions = num_hash_functions;
    bloom->buffer_ownership = 1;
    return bloom;
}

struct bloom_filter* init_bloom_filter_existing_array(unsigned char* filter_byte_array, 
        uint16_t num_bits, uint16_t num_bytes, int num_hash_functions)
{
    struct bloom_filter* bloom;
    bloom = (struct bloom_filter*)kmalloc(sizeof * bloom, GFP_ATOMIC);
    if(bloom == NULL) {
        return (struct bloom_filter*)NULL;
    }
    
    bloom->filter_byte_array = filter_byte_array;
    bloom->num_bits = num_bits;
    bloom->num_bytes = num_bytes;
    bloom->num_hash_functions = num_hash_functions;
    bloom->buffer_ownership = 0;
    return bloom;
}

struct bloom_filter* init_bloom_filter_no_alloc(struct bloom_filter* bloom, unsigned char* filter_byte_array, 
        uint16_t num_bits, int num_hash_functions)
{
    uint16_t num_bytes = num_bits / 8;
    if(num_bits % 8 > 0) {
        num_bytes += 1;
    }

    bloom->filter_byte_array = filter_byte_array;
    bloom->num_bits = num_bits;
    bloom->num_bytes = num_bytes;
    bloom->num_hash_functions = num_hash_functions;
    bloom->buffer_ownership = 0;
    return bloom;
}

int bloom_filter_check_member(struct bloom_filter* bloom, uint16_t key)
{
    uint16_t hits, hash_index, set_bit_index, set_byte_index;
    unsigned char mask;
    // pr_info("BF_DEBUG: Checking key %d against bloom filter", key);
    
    if(bloom == NULL || bloom->filter_byte_array == NULL) {
        return -1;
    }

    hits = 0;
    for(hash_index = 0; hash_index < bloom->num_hash_functions; hash_index++) {
        set_bit_index = bloom_hashcode_uint16key((uint16_t*)(&key), hash_index, bloom->num_bits);
	// pr_info("BF_DEBUG: Hash # = %d, Bit Index = %d", hash_index, set_bit_index);
        set_byte_index = set_bit_index >> 3;	// (x >> 3) == (x / 8)
        mask = 1 << (7 - (set_bit_index % 8));
        if(bloom->filter_byte_array[set_byte_index] & mask) {
            hits++;
        }
    }

    if(hits == bloom->num_hash_functions) {
	// pr_info("BF_DEBUG: Key %d match success", key);
        return 1;
    } else {
	// pr_info("BF_DEBUG: Key %d match failed", key);
        return 0;
    }
}

int bloom_filter_add_member(struct bloom_filter* bloom, uint16_t key)
{
    uint16_t hash_index;
    uint32_t set_bit_index, set_byte_index;
    
    if(bloom == NULL || bloom->filter_byte_array == NULL) {
        return -1;
    }

    for(hash_index = 0; hash_index < bloom->num_hash_functions; hash_index++) {
        set_bit_index = bloom_hashcode_uint16key((uint16_t*)(&key), hash_index, bloom->num_bits);
        set_byte_index = set_bit_index >> 3;	// (x >> 3) == (x / 8)
        bloom->filter_byte_array[set_byte_index] = bloom->filter_byte_array[set_byte_index] | (1 << (7 - (set_bit_index % 8)));
    }
    return 0;
}

int free_bloom_filter(struct bloom_filter* bloom)
{
    if(bloom == NULL) {
        return -1;
    }
    if(bloom->buffer_ownership && bloom->filter_byte_array != NULL) {
        kfree((void*)bloom->filter_byte_array);
    }
    kfree((void*)bloom);
    return 0;
}



