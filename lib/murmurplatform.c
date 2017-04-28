#include "murmurplatform.h"

uint32_t rotl32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

uint64_t rotl64(uint64_t x, int8_t r)
{
    return (x << r) | (x >> (64 - r));
}

uint32_t rotr32(uint32_t x, int8_t r)
{
    return (x >> r) | (x << (32 - r));
}

uint64_t rotr64(uint64_t x, int8_t r)
{
    return (x >> r) | (x << (64 - r));
}

unsigned long long int rdtsc()
{
    unsigned long long int x;
    __asm__ volatile("rdtsc" : "=A"(x));
    return x;
}