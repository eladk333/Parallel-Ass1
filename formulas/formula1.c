/* 322587064 Elad Katz */

#include <xmmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pmmintrin.h>

#define MAX_STR 255

float formula1(float *x, unsigned int length) {
    __m128 v_sum = _mm_setzero_ps(); // For the sum filled with 0s 
    __m128 v_prod = _mm_set1_ps(1.0f); // For the prod filled with 1s

    __m128 v_ones = _mm_set1_ps(1.0f); // Constant for the x^2+1 part

    unsigned int i = 0; // Counter for offset

    for (; i + 3 < length; i += 4) {
        // Loads 4 foats from x
        __m128 v_x = _mm_loadu_ps(&x[i]); 

        // The sum part
        __m128 v_sqrt = _mm_sqrt_ps(v_x); // Puts sqrt over all of them like in the equation
        v_sum = _mm_add_ps(v_sum, v_sqrt); // Load the partial sums

        // Prod part
        __m128 v_sq = _mm_mul_ps(v_x, v_x); // Does x^2
        __m128 v_sq_plus_1 = _mm_add_ps(v_sq, v_ones); // adds the 1
        v_prod = _mm_mul_ps(v_prod, v_sq_plus_1); // multiply with what we had so far
    }

    // Extracting the partial sums
    v_sum = _mm_hadd_ps(v_sum, v_sum);
    v_sum = _mm_hadd_ps(v_sum, v_sum);
    float total_sum;
    _mm_store_ss(&total_sum, v_sum);
    
    __m128 v_temp = _mm_movehl_ps(v_prod, v_prod); // Takes the higher half of the vec and moves it into the lower part
    v_prod = _mm_mul_ps(v_prod, v_temp);
    v_temp = _mm_shuffle_ps(v_prod, v_prod, _MM_SHUFFLE(1, 1, 1, 1));
    v_prod = _mm_mul_ps(v_prod, v_temp); // Second multiply 
    float total_prod = _mm_cvtss_f32(v_prod); // Saving it

    // For the trail elements
    for (; i < length; ++i) {
        total_sum += sqrtf(x[i]);
        total_prod *= (x[i] * x[i] + 1.0f);
    }

    float numerator = cbrtf(total_sum);
    float result = sqrtf(1.0f + (numerator / total_prod));

    return result;
}