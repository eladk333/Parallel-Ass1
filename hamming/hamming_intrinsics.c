/* 322587064 Elad Katz */

#include <emmintrin.h>   // SSE2
#include <smmintrin.h>   // SSE4.1
#include <string.h>

#define MAX_STR 256

// Helper function to count the number of 1s in a bitmask (mismatches)
static inline int count_set_bits(int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

// Helper function to find the exact string length using SSE intrinsics
static inline int simd_strlen(const char *str) {
    __m128i zero = _mm_setzero_si128(); // Vector filled with '\0'
    
    for (int i = 0; i < MAX_STR; i += 16) {
        // Load 16 bytes from the string
        __m128i v = _mm_loadu_si128((__m128i*)&str[i]);
        
        // Compare each byte with '\0'. Result is 0xFF if equal, 0x00 if not.
        __m128i z = _mm_cmpeq_epi8(v, zero);
        
        // Extract the most significant bit of each byte into a 16-bit integer mask
        int mask = _mm_movemask_epi8(z);
        
        if (mask != 0) {
            // Find the index of the first 1-bit (which is our null terminator)
            for (int bit = 0; bit < 16; bit++) {
                if ((mask >> bit) & 1) {
                    return i + bit;
                }
            }
        }
    }
    return MAX_STR;
}

int hamming_dist(char str1[MAX_STR], char str2[MAX_STR]) {
    // 1. Find the true lengths of both strings using SIMD
    int len1 = simd_strlen(str1);
    int len2 = simd_strlen(str2);

    int min_len = (len1 < len2) ? len1 : len2;
    int distance = 0;
    int i = 0;

    // 2. Compare 16-byte chunks using intrinsics up to the end of the shorter string
    for (; i + 15 < min_len; i += 16) {
        // Load 16 bytes from both strings
        __m128i v1 = _mm_loadu_si128((__m128i*)&str1[i]);
        __m128i v2 = _mm_loadu_si128((__m128i*)&str2[i]);

        // Compare vectors for equality. Bytes that match become 0xFF, mismatches are 0x00
        __m128i eq = _mm_cmpeq_epi8(v1, v2);
        
        // Create a 16-bit mask from the comparison vector
        int mask = _mm_movemask_epi8(eq);

        // We want to count *mismatches*. 
        // Invert the mask (so mismatches become 1s) and isolate the lower 16 bits.
        int mismatches = ~mask & 0xFFFF;
        
        // Add the number of mismatches to our total distance
        distance += count_set_bits(mismatches);
    }

    // 3. Handle the remaining characters (the "tail") that didn't fit neatly into a 16-byte chunk
    for (; i < min_len; i++) {
        if (str1[i] != str2[i]) {
            distance++;
        }
    }

    // 4. If lengths differ, add the difference to the total distance per assignment instructions
    int len_diff = (len1 > len2) ? (len1 - len2) : (len2 - len1);
    distance += len_diff;

    return distance;
}