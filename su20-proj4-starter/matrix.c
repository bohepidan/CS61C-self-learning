#include "matrix.h"
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
 */

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix* result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data
 * array and initialize all entries to be zeros. `parent` should be set to NULL
 * to indicate that this matrix is not a slice. You should also set `ref_cnt`
 * to 1. You should return -1 if either `rows` or `cols` or both have invalid
 * values, or if any call to allocate memory in this function fails. Return 0
 * upon success.
 */
int allocate_matrix(matrix** mat, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (rows <= 0 || cols <= 0)
        return -1;
    long long total = rows * cols;
    matrix* mt = malloc(sizeof(matrix));
    if (NULL == mt)
        return -1;
    mt.data = malloc(sizeof(double) * total);
    if (NULL == mt.data)
        return -1;
    for (long long i = 0; i < total; i++)
        mt.data[i] = 0;
    mt->rows = rows;
    mt->cols = cols;
    mt->parent = NULL;
    mt->ref_cnt = 1;
    *mat = mt;
    return 0;
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and
 * `cols` columns. Its data should point to the `offset`th entry of `from`'s
 * data (you do not need to allocate memory) for the data field. `parent` should
 * be set to `from` to indicate this matrix is a slice of `from`. You should
 * return -1 if either `rows` or `cols` or both are non-positive or if any call
 * to allocate memory in this function fails. Return 0 upon success.
 */
int allocate_matrix_ref(matrix** mat,
                        matrix* from,
                        int offset,
                        int rows,
                        int cols) {
    /* TODO: YOUR CODE HERE */
    if (rows <= 0 || cols <= 0)
        return -1;
    matrix* mt = malloc(sizeof(matrix));
    if (NULL == mt)
        return -1;
    mt->data = from.data + offset;
    mt->rows = rows;
    mt->cols = cols;
    mt->parent = from;
    mt->ref_cnt = 1;
    *mat = mt;

    (from->ref_cnt)++;

    reutrn 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice
 * and has no existing slices, or if `mat` is the last existing slice of its
 * parent matrix and its parent matrix has no other references (including
 * itself). You cannot assume that mat is not NULL.
 */
void deallocate_matrix(matrix* mat) {
    /* TODO: YOUR CODE HERE */
    if (mat->ref_cnt != 1)
        return;
    if (mat->parent != NULL) {
        if (mat->parent->ref_cnt != 1)
            return;
        else {
        }
    }
    free(mat->data);
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix* mat, int row, int col) {
    /* TODO: YOUR CODE HERE */
    return (mat->data)[row * (mat->cols) + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix* mat, int row, int col, double val) {
    /* TODO: YOUR CODE HERE */
    (mat->data)[row * (mat->cols) + col] = val;
}

/*
 * Sets all entries in mat to val
 */
void fill_matrix(matrix* mat, double val) {
    /* TODO: YOUR CODE HERE */
    long long total = (mat->cols) * (mat->rows);
    for (long long i = 0; i < total; i++) {
        (mat->data)[i] = val;
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix* result, matrix* mat1, matrix* mat2) {
    /* TODO: YOUR CODE HERE */
    if (mat1->data == NULL || mat2->data == NULL || result->data == NULL)
        return 1;
    if (mat1->cols != mat2->cols || mat1->rows != mat2->rows ||
        mat1->cols != result->cols || mat1->rows != result->rows)
        return 2;
    long long total = (mat1->cols) * (mat1->rows);
    for (long long i = 0; i < total; i++)
        (result->data)[i] = (mat1->data)[i] + (mat2->data)[i];
    return 0;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix* result, matrix* mat1, matrix* mat2) {
    /* TODO: YOUR CODE HERE */
    if (mat1->data == NULL || mat2->data == NULL || result->data == NULL)
        return 1;
    if (mat1->cols != mat2->cols || mat1->rows != mat2->rows ||
        mat1->cols != result->cols || mat1->rows != result->rows)
        return 2;
    long long total = (mat1->cols) * (mat1->rows);
    for (long long i = 0; i < total; i++)
        (result->data)[i] = (mat1->data)[i] - (mat2->data)[i];
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual
 * elements.
 */
int mul_matrix(matrix* result, matrix* mat1, matrix* mat2) {
    /* TODO: YOUR CODE HERE */
    if (mat1->data == NULL || mat2->data == NULL || result->data == NULL)
        return 1;
    if (mat1->cols != mat2->rows)
        return 3;
    if (result->rows != mat1.rows || result->cols != mat2->cols)
        return 4;

    fill_matrix(result, 0);

    for (int i = 0; i < mat1->rows; i++)
        for (int k = 0; k < mat1->cols;
             k++)  // '->' symble access memory, reduce use may be a good idea
            for (int j = 0; j < mat2->cols; j++)
                (result->data)[i * (result->cols) + j] +=
                    (mat1->data)[i * (mat1->cols) + k] *
                    (mat2->data)[j + k * (mat2->cols)];

    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise
 * multiplication.
 */
int pow_matrix(matrix* result, matrix* mat, int pow) {
    /* TODO: YOUR CODE HERE */
    if (mat->data == NULL || result->data == NULL)
        return 1;
    if (mat->cols != mat->rows || result->cols != result->rows)
        return 5;
    if (pow <= 0)
        return 6;
    if (mat->cols != result->cols)
        return 7;
    if (pow == 1) {
        long long total = (result->cols) * (result->rows);
        for (long long i = 0; i < total; i++) {
            (result->data)[i] = (mat->data)[i];
        }
        return 0;
    }
    mul_matrix(result, mat, mat) if (pow == 2) return 0;
    int n = mat->cols;
    matrix** tmp;
    {
        int err;
        err = allocate_matrix(tmp, n, n);
        if (err != 0)
            return err;
    }
    pow = pow - 2;
    matrix* mt = *tmp;
    int reptime = pow / 2;
    for (int i = 0; i < reptime; i++) {
        mul_matrix(mt, result, mat);
        mul_matrix(result, mt, mat);
    }
    if (pow % 2 != 0) {
        mul_matrix(mt, result, mat);
        for (int j = 0; j < n * n; j++)
            (result->data)[j] = (mt->data)[j];
    }

    deallocate_matrix(mt);
    return 0
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix* result, matrix* mat) {
    /* TODO: YOUR CODE HERE */
    if (result->cols != mat->cols || result->rows != mat->rows)
        return 2;
    long long total = (mat->cols) * (mat->rows);
    for (long long i = 0; i < total; i++)
        (result->data)[i] = 0 - (mat->data)[i];
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix* result, matrix* mat) {
    /* TODO: YOUR CODE HERE */
    if (result->cols != mat->cols || result->rows != mat->rows)
        return 2;
    long long total = (mat->cols) * (mat->rows);
    for (long long i = 0; i < total; i++) {
        if ((mat->data)[i] < 0) {
            (result->data)[i] = 0 - (mat->data)[i];
        } else {
            (result->data)[i] = (mat->data)[i];
        }
    }
    return 0;
}
