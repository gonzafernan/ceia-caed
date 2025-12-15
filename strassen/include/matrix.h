/**
 * @file matrix.h
 * @author Gonzalo G. Fernandez
 * @brief Header of 2D matrix operations.
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdlib.h>

/// 2D integer matrix type.
typedef struct imatrix_s imatrix_t;

/**
 * @brief Create new 2D matrix of integers.
 * @param rows number of rows
 * @param cols number of cols
 * @returns pointer to the new matrix
 */
imatrix_t *imatrix_new(size_t rows, size_t cols);

/**
 * @brief Delete a 2D matrix of integers.
 * @param matrix pointer to the matrix
 */
void imatrix_free(imatrix_t *matrix);

/** @brief Get the matrix size.
 * @param matrix pointer to the matrix
 * @param rows pointer to size_t where the number of rows will be stored
 * @param cols pointer to size_t where the number of columns will be stored
 * @returns 0 if success, -1 otherwise
 */
int imatrix_get_size(imatrix_t *matrix, size_t *rows, size_t *cols);

/**
 * @brief Get value from a 2D matrix given indexes.
 * @param matrix pointer to the matrix
 * @param i row index to access
 * @param j column index to access
 * @param value pointer to an int where the value will be stored
 * @returns 0 if success, -1 otherwise
 *
 * The operation is O(1)
 *
 */
int imatrix_get_value(imatrix_t *matrix, size_t i, size_t j, int *value);

/**
 * @brief Set value of a 2D matrix given indexes.
 * @param matrix pointer to the matrix
 * @param i row index to write
 * @param j column index to write
 * @param value integer to write in the matrix
 * @returns 0 if success, -1 otherwise
 *
 * The operation is O(1)
 *
 */
int imatrix_set_value(imatrix_t *matrix, size_t i, size_t j, int value);

/**
 * @brief Integer matrix addition O(n^2).
 * Matrices A, B have same size.
 * @param matrix_a pointer to matrix A (n x n)
 * @param matrix_b pointer to matrix B (n x n)
 * @returns pointer to new matrix C result of A + B (n x n)
 *
 * The operation is O(n^2)
 *
 */
imatrix_t *imatrix_add(imatrix_t *matrix_a, imatrix_t *matrix_b);

/**
 * @brief Integer matrix substraction O(n^2).
 * Matrices A, B have same size.
 * @param matrix_a pointer to matrix A (n x n)
 * @param matrix_b pointer to matrix B (n x n)
 * @returns pointer to new matrix C result of A - B (n x n)
 *
 * The operation is O(n^2)
 *
 */
imatrix_t *imatrix_substract(imatrix_t *matrix_a, imatrix_t *matrix_b);

/**
 * @brief Integer matrix scale O(n^2).
 * @param matrix pointer to matrix (n x n)
 * @param scalar scale value
 * @returns pointer to new matrix C result of A * scalar (n x n)
 *
 * The operation is O(n^2)
 *
 */
imatrix_t *imatrix_scale(imatrix_t *matrix, int scalar);

/**
 * @brief Integer matrix multiplication O(n^3).
 * @param matrix_a pointer to matrix A (n x n)
 * @param matrix_b pointer to matrix B (n x n)
 * @returns pointer to new matrix C result of A * B (n x n)
 *
 * Matrices A, B have same size.
 * The operation is O(n^3)
 *
 */
imatrix_t *imatrix_multiply_brute_force(imatrix_t *matrix_a,
                                        imatrix_t *matrix_b);

/**
 * @brief Integer matrix multiplication with recursive algorithm O(n^3).
 * @param matrix_a pointer to matrix A (n x n)
 * @param matrix_b pointer to matrix B (n x n)
 * @returns pointer to new matrix C result of A * B (n x n)
 *
 * Matrices A, B have same size.
 * As simplification we assume n power of 2.
 * The operation is O(n^3)
 *
 */
imatrix_t *imatrix_multiply_recursive(imatrix_t *matrix_a, imatrix_t *matrix_b);

/**
 * @brief Integer submatrix multiplication with Strassen's algorithm.
 * @param matrix_a pointer to matrix A (n x n)
 * @param matrix_b pointer to matrix B (n x n)
 * @returns pointer to new matrix C result of A * B (n x n)
 *
 * Matrices A, B, and C have same size.
 * As simplification we assume n power of 2.
 * The operation is O(n^(lg7))
 *
 */
imatrix_t *imatrix_multiply_strassen(imatrix_t *matrix_a, imatrix_t *matrix_b);

/**
 * @brief Get a string representation of the matrix.
 * @param matrix pointer to the matrix
 * @returns 0 if success, -1 otherwise
 */
char *imatrix_dump(imatrix_t *matrix);

#endif // __MATRIX_H__
