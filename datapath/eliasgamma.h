#ifndef ELIAS_GAMMA_H
#define ELIAS_GAMMA_H

#include <linux/types.h>

/**
 * Encodes a value in Elias Gamma format into a newly allocated byte array.
 * The length of the array is the nearest whole byte value that can contain
 * the Elias Gamma encoded number. The length of the byte array is written
 * to the size_t variable pointed to by array_len (the input value of this
 * parameter is not read or used). NULL is returned if the buffer allocation
 * fails, or if the Elias Gamma representation of the number would exceed
 * 32 bits.
 */
unsigned char* encode_uint16_32bit_elias_gamma(const uint16_t value, size_t* array_len);

/**
 * Decodes the specified byte array in Elias Gamma format, and returns the decoded
 * integer if a valid decoding could be performed. Returns 0 if the byte array is not
 * a valid Elias Gamma encoding using 32 or less bits, or if the specified length of
 * the byte array exceeds 4 bytes (32 bits). The number of bits in the Elias Gamma
 * representation is written to the size_t variable pointed to be eg_num_bits.
 */
uint16_t decode_uint16_32bit_elias_gamma(const unsigned char* eg_array, const size_t array_len, uint16_t* eg_num_bits);

#endif
/* ELIAS_GAMMA_H */



