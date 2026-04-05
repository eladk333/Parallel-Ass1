/* 322587064 Elad Katz */

#include <emmintrin.h>   // SSE2
#include <smmintrin.h>   // SSE4.1
#include <string.h>

#define MAX_STR 256

static inline int simd_strlen(const char *str) {
    __m128i zero = _mm_setzero_si128(); // Creates 128-bit register with 0s
 
    for (int i = 0; i < MAX_STR; i += 16) {
        __m128i v    = _mm_loadu_si128((__m128i *)&str[i]); // Loads 16 bytes from the string into an XMM register

        // Checks if null terminator found
        __m128i cmp  = _mm_cmpeq_epi8(v, zero); 
        int mask = _mm_movemask_epi8(cmp);
 
        if (mask != 0) // If found return counter + index of null terminator
            return i + __builtin_ctz(mask);
    }
    return MAX_STR;
}
 
int hamming_dist(char str1[MAX_STR], char str2[MAX_STR]) {
    // Get lenght of both strings
    int len1    = simd_strlen(str1);
    int len2    = simd_strlen(str2);
    int min_len = (len1 < len2) ? len1 : len2;
    int distance = 0; // How many differences so far
    int i = 0; // Position in strings
     
    for (; i + 16 <= min_len; i += 16) {
        // Loads 16 bytes from each string into XMM
        __m128i v1  = _mm_loadu_si128((__m128i *)&str1[i]);
        __m128i v2  = _mm_loadu_si128((__m128i *)&str2[i]);
 
        // 0xFF where chars are equal, 0x00 where they differ
        __m128i eq  = _mm_cmpeq_epi8(v1, v2);
 
        // Invert mask so 1 = mismatch, then count set bits
        int mismatches = ~_mm_movemask_epi8(eq) & 0xFFFF;
        // Adds number of mismatches we found
        distance += __builtin_popcount(mismatches);
    }
 
    // Handle remaining bytes that didn't fill a full 16-byte chunk
    for (; i < min_len; i++) {
        if (str1[i] != str2[i])
            distance++;
    }
 
    // Add length difference as mismatch
    if (len1 > len2)
        distance += len1 - len2;
    else
        distance += len2 - len1;
 
    return distance;
}