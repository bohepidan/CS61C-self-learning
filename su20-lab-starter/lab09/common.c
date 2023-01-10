#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "common.h"

long long int sum(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for (unsigned int i = 0; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if (vals[i] >= 128) sum += vals[i];
			if (vals[i + 1] >= 128) sum += vals[i + 1];
			if (vals[i + 2] >= 128) sum += vals[i + 2];
			if (vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				   // This is where you should put your final result!
	/* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */
	int* dst = malloc(128);
	for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		__m128i res = _mm_setzero_si128();
		__m128i block;
		for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			block = _mm_loadu_si128(vals + i);
			res = _mm_add_epi32(res, _mm_and_si128(block, _mm_cmpgt_epi32(block, _127)));
		}
		/* You'll need a tail case here. */
		_mm_storeu_si128((__m128*) dst, res);
		for (int i = 0; i < 4; i++)
			result += dst[i];

		for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++)
			if (vals[i] >= 128)
				result += vals[i];
	}
	free(dst);
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	int* dst = malloc(128 * 4);
	for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* COPY AND PASTE YOUR sum_simd() HERE */
		/* MODIFY IT BY UNROLLING IT */
		__m128i res = _mm_setzero_si128();
		__m128i block0, block1, block2, block3;
		for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
			block0 = _mm_loadu_si128(vals + i);
			block1 = _mm_loadu_si128(vals + i + 4);
			block2 = _mm_loadu_si128(vals + i + 8);
			block3 = _mm_loadu_si128(vals + i + 12);

			res = _mm_add_epi32(res, _mm_and_si128(block0, _mm_cmpgt_epi32(block0, _127)));
			res = _mm_add_epi32(res, _mm_and_si128(block1, _mm_cmpgt_epi32(block1, _127)));
			res = _mm_add_epi32(res, _mm_and_si128(block2, _mm_cmpgt_epi32(block2, _127)));
			res = _mm_add_epi32(res, _mm_and_si128(block3, _mm_cmpgt_epi32(block3, _127)));
		}
		/* You'll need a tail case here. */
		_mm_storeu_si128((__m128*) dst, res);
		_mm_storeu_si128((__m128*) (dst + 128), res);
		_mm_storeu_si128((__m128*) (dst + 256), res);
		_mm_storeu_si128((__m128*) (dst + 384), res);
		for (int i = 0; i < 16; i++)
			result += dst[i];

		for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++)
			if (vals[i] >= 128)
				result += vals[i];

	}
	free(dst);
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}