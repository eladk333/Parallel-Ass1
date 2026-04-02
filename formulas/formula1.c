/* 123456789 Elad Katz */

#include <xmmintrin.h> // SSE
#include <pmmintrin.h> // SSE3 (for _mm_hadd_ps)
#include <smmintrin.h> // SSE4.1
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float formula1(float *x, unsigned int length) {
    // Initialize our tracking vectors
    // v_sum starts at 0.0, v_prod starts at 1.0
    __m128 v_sum = _mm_setzero_ps();
    __m128 v_prod = _mm_set1_ps(1.0f);
    
    // Vector constant filled with 1.0f for the "+ 1" in the product formula
    __m128 v_ones = _mm_set1_ps(1.0f);

    unsigned int i = 0;
    
    // Process 4 floats at a time
    for (; i + 3 < length; i += 4) {
        // Load 4 floats from the array
        __m128 v_x = _mm_loadu_ps(&x[i]);

        // --- SUMMATION PART ---
        // Calculate square roots of the 4 elements
        __m128 v_sqrt = _mm_sqrt_ps(v_x);
        // Add the roots to our running sum vector
        v_sum = _mm_add_ps(v_sum, v_sqrt);

        // --- PRODUCT PART ---
        // Square the 4 elements (x[k] * x[k])
        __m128 v_sq = _mm_mul_ps(v_x, v_x);
        // Add 1.0 to each squared element
        __m128 v_sq_plus_1 = _mm_add_ps(v_sq, v_ones);
        // Multiply by our running product vector
        v_prod = _mm_mul_ps(v_prod, v_sq_plus_1);
    }

    // --- HORIZONTAL REDUCTION ---
    
    // 1. Reduce v_sum (Horizontal Add)
    // [A, B, C, D] -> [A+B, C+D, A+B, C+D] -> [A+B+C+D, ...]
    v_sum = _mm_hadd_ps(v_sum, v_sum); 
    v_sum = _mm_hadd_ps(v_sum, v_sum); 
    float total_sum = _mm_cvtss_f32(v_sum); // Extract the lowest 32 bits into a standard float

    // 2. Reduce v_prod (Horizontal Multiply)
    // SSE lacks a direct horizontal multiply, so we shift and multiply manually
    __m128 v_temp = _mm_movehl_ps(v_prod, v_prod); // Move high half to low half
    v_prod = _mm_mul_ps(v_prod, v_temp);           // Multiply halves
    v_temp = _mm_shuffle_ps(v_prod, v_prod, _MM_SHUFFLE(1, 1, 1, 1)); // Shift down by 1 float
    v_prod = _mm_mul_ps(v_prod, v_temp);           // Final multiply
    float total_prod = _mm_cvtss_f32(v_prod);      // Extract the scalar product

    // --- TAIL HANDLING ---
    // If the length wasn't a perfect multiple of 4, handle the remaining elements sequentially.
    // (Note: main.c tests use multiples of 4, but this ensures a completely robust function).
    for (; i < length; ++i) {
        total_sum += sqrtf(x[i]);
        total_prod *= (x[i] * x[i] + 1.0f);
    }

    // --- FINAL SCALAR MATH ---
    float numerator = cbrtf(total_sum);
    float result = sqrtf(1.0f + (numerator / total_prod));

    return result;
}