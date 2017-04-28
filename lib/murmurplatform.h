#ifndef MURMUR_PLATFORM_H
#define MURMUR_PLATFORM_H

void SetAffinity(int cpu);
#include <stdint.h>
#include <strings.h>

#define	FORCE_INLINE __attribute__((always_inline))

FORCE_INLINE uint32_t rotl32(uint32_t x, int8_t r);
FORCE_INLINE uint64_t rotl64(uint64_t x, int8_t r);
FORCE_INLINE uint32_t rotr32(uint32_t x, int8_t r);
FORCE_INLINE uint64_t rotr64(uint64_t x, int8_t r);
FORCE_INLINE unsigned long long int rdtsc();

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)
#define	ROTR32(x,y)	rotr32(x,y)
#define ROTR64(x,y)	rotr64(x,y)
#define BIG_CONSTANT(x) (x##LLU)
#define _stricmp strcasecmp

#endif
/* MURMUR_PLATFORM_H */