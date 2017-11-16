#include "eliasgamma.h"
#include <linux/net.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/string.h>

unsigned char* encode_uint16_32bit_elias_gamma(const uint16_t value, size_t* array_len)
{
    uint16_t value_copy, num_leading_zero_bits, num_bits_elias_gamma, byte_index;
    uint32_t padded_value;
    unsigned char* eg_array;
    unsigned char* padded_value_array;
    
    if(value == 0) {
        return (unsigned char*)NULL;
    }

    // KLUDGE: This finds the log2 of value, without requiring linking to the standard
    // math library. This method is very inefficient for large values, but in our applications
    // the value will always be 320 or under
    value_copy = value;
    num_leading_zero_bits = 0;
    while(value_copy > 1) {
        num_leading_zero_bits++;
        value_copy >>= 1;
    }

    num_bits_elias_gamma = (2 * num_leading_zero_bits) + 1;
    if(num_bits_elias_gamma > 32) {
        // TODO: Figure out how the handle this case more gracefully.
        return (unsigned char*)NULL;
    }

    (*array_len) = num_bits_elias_gamma / 8;
    if(num_bits_elias_gamma % 8 > 0) {
        (*array_len) = (*array_len) + 1;
    }

    eg_array = (unsigned char*)kmalloc((*array_len), GFP_ATOMIC);
    if(eg_array == NULL) {
        return (unsigned char*)NULL;
    }
    memset(eg_array, 0, *array_len);


    padded_value = (uint32_t)value;
    padded_value_array = (unsigned char*)(&padded_value);
    padded_value = padded_value << (32 - ((2 * num_leading_zero_bits) + 1));
    padded_value = htonl(padded_value); // Note: Assumes machine format is little endian

    for(byte_index = 0; byte_index < (*array_len); byte_index++) {
        eg_array[byte_index] = padded_value_array[byte_index];
    }

    return eg_array;
}

uint16_t decode_uint16_32bit_elias_gamma(const unsigned char* eg_array, const size_t array_len, uint16_t* eg_num_bits)
{
    // TODO: There must be a more efficient way to implement this decoding
    uint16_t byte_index, leading_zeroes, return_val;
    unsigned char eg_uint32_array[4];
    uint32_t* eg_uint32;
    
    if(eg_array == NULL || array_len > 4) {
        return 0;
    }

    for(byte_index = 0; byte_index < 4; byte_index++) {
        if(byte_index < array_len) {
            eg_uint32_array[byte_index] = eg_array[byte_index];
        } else {
            eg_uint32_array[byte_index] = 0;
        }
    }

    // Note: Assumes Elias Gamma array is in big endian format
    eg_uint32 = (uint32_t*)(eg_uint32_array);
    leading_zeroes = 0;
    // Bit shift the array left until a 1 is found in the most significant bit
    // to determine the number of leading zeros
    while(!(eg_uint32_array[0] & 0x80)) {
        leading_zeroes++;

        for(byte_index = 0; byte_index < 4; byte_index++) {
            eg_uint32_array[byte_index] = eg_uint32_array[byte_index] << 1;
            if(byte_index < 3 && eg_uint32_array[byte_index + 1] & 0x80) {
                eg_uint32_array[byte_index] = eg_uint32_array[byte_index] | 0x01;
            }
        }
        if(leading_zeroes > 16) {
            break;
        }
    }
    if(leading_zeroes > 16) {
        return 0;
    }

    // Now that the number of leading zeroes has been determined, get a copy
    // of the array in its original state, perform the endian conversion, and shift
    // the binary number to the end of the array for conversion to uint16
    for(byte_index = 0; byte_index < 4; byte_index++) {
        if(byte_index < array_len) {
            eg_uint32_array[byte_index] = eg_array[byte_index];
        } else {
            eg_uint32_array[byte_index] = 0;
        }
    }
    *eg_num_bits = ((2 * leading_zeroes) + 1);
    (*eg_uint32) = ntohl(*eg_uint32);	// Assumes machine format is little endian
    (*eg_uint32) = (*eg_uint32) >> (32 - (*eg_num_bits));
    return_val = (uint16_t)(*eg_uint32);

    return return_val;
}
