/**
 * @file matrix_algorithms.c
 * @author Gonzalo G. Fernandez
 * @brief Implementation of algorithms for matrix operations.
 *
 * - Recursive method for square matrices multiplication.
 * - Strassen method for square matrices multiplication.
 *
 * Implementation of the algorithms following "Introduction to Algorithms" by
 * Cormen
 *
 */

#include "matrix.h"
#include <stdlib.h>

// Data structure with a submatrix representation of a matrix
typedef struct {
  size_t rows_start; // virtual row start for submatrix index calculation
  size_t cols_start; // virtual col start for submatrix index calculation
  size_t rows_size;
  size_t cols_size;
  imatrix_t *matrix;
} isubmatrix;

isubmatrix isubmatrix_from_matrix(imatrix_t *matrix, size_t rows_size,
                                  size_t cols_size, size_t rows_start,
                                  size_t cols_start) {
  isubmatrix submatrix;
  submatrix.matrix = matrix;
  submatrix.rows_size = rows_size;
  submatrix.cols_size = cols_size;
  submatrix.rows_start = rows_start;
  submatrix.cols_start = cols_start;
  return submatrix;
}

isubmatrix isubmatrix_from_submatrix(isubmatrix *submatrix, size_t rows_size,
                                     size_t cols_size, size_t rows_start,
                                     size_t cols_start) {
  isubmatrix subsubmatrix;
  subsubmatrix.matrix = submatrix->matrix;
  subsubmatrix.rows_size = rows_size;
  subsubmatrix.cols_size = cols_size;
  subsubmatrix.rows_start = submatrix->rows_start + rows_start;
  subsubmatrix.cols_start = submatrix->cols_start + cols_start;
  return subsubmatrix;
}

int isubmatrix_get_value(isubmatrix *submatrix, size_t i, size_t j,
                         int *value) {
  if (imatrix_get_value(submatrix->matrix, i + submatrix->rows_start,
                        j + submatrix->cols_start, value) < 0) {
    return -1;
  }
  return 0;
}

int isubmatrix_set_value(isubmatrix *submatrix, size_t i, size_t j, int value) {
  if (imatrix_set_value(submatrix->matrix, i + submatrix->rows_start,
                        j + submatrix->cols_start, value) < 0) {
    return -1;
  }
  return 0;
}

int isubmatrix_set_submatrix(isubmatrix *submatrix,
                             isubmatrix *submatrix_to_set) {
  int value;
  for (size_t i = 0; i < submatrix_to_set->rows_size; i++) {
    for (size_t j = 0; j < submatrix_to_set->cols_size; j++) {
      if (isubmatrix_get_value(submatrix_to_set, i, j, &value) < 0) {
        return -1;
      }
      isubmatrix_set_value(submatrix, i, j, value);
    }
  }
  return 0;
}

/**
 * @brief Integer submatrix addition.
 * @param submatrix_a pointer to the submatrix A (n x n)
 * @param submatrix_b pointer to the submatrix B (n x n)
 * @param submatrix_c pointer to the submatrix C (n x n) result of A + B
 * @returns 0 if success, -1 otherwise
 *
 * Operation O(n^2)
 *
 */
int isubmatrix_add(isubmatrix *submatrix_a, isubmatrix *submatrix_b,
                   isubmatrix *submatrix_c) {
  size_t n = submatrix_a->rows_size;
  if (submatrix_a->cols_size != n || submatrix_b->rows_size != n ||
      submatrix_b->cols_size != n) {
    return -1;
  }

  int aij, bij;
  for (size_t i = 0; i < submatrix_a->rows_size; i++) {
    for (size_t j = 0; j < submatrix_a->cols_size; j++) {
      isubmatrix_get_value(submatrix_a, i, j, &aij);
      isubmatrix_get_value(submatrix_b, i, j, &bij);
      isubmatrix_set_value(submatrix_c, i, j, aij + bij);
    }
  }
  return 0;
}

/**
 * @brief Integer submatrix substraction.
 * @param submatrix_a pointer to the submatrix A (n x n)
 * @param submatrix_b pointer to the submatrix B (n x n)
 * @param submatrix_c pointer to the submatrix C (n x n) result of A - B
 * @returns 0 if success, -1 otherwise
 *
 * Operation O(n^2)
 *
 */
int isubmatrix_substract(isubmatrix *submatrix_a, isubmatrix *submatrix_b,
                         isubmatrix *submatrix_c) {
  size_t n = submatrix_a->rows_size;
  if (submatrix_a->cols_size != n || submatrix_b->rows_size != n ||
      submatrix_b->cols_size != n) {
    return -1;
  }

  int aij, bij;
  for (size_t i = 0; i < submatrix_a->rows_size; i++) {
    for (size_t j = 0; j < submatrix_a->cols_size; j++) {
      isubmatrix_get_value(submatrix_a, i, j, &aij);
      isubmatrix_get_value(submatrix_b, i, j, &bij);
      isubmatrix_set_value(submatrix_c, i, j, aij - bij);
    }
  }
  return 0;
}

// TODO: This functions could be refactores to receive a function pointer
// that would act as lambda. That way I can implement a generic binary
// operation.

/**
 * @brief Integer submatrix multiplication with recursive algorithm.
 * Matrices A, B, and C have same size.
 * As simplification we assume n power of 2.
 * This operation is O(n^3) like regular multiplication (brute_force).
 */
int isubmatrix_multiply_recursive(isubmatrix *submatrix_a,
                                  isubmatrix *submatrix_b,
                                  isubmatrix *submatrix_c) {
  size_t n = submatrix_a->rows_size;

  int aik, bkj;
  if (n == 1) {
    isubmatrix_get_value(submatrix_a, 0, 0, &aik);
    isubmatrix_get_value(submatrix_b, 0, 0, &bkj);
    isubmatrix_set_value(submatrix_c, 0, 0, aik * bkj);
    return 0;
  }

  // Step 1:
  // Divide the input matrices A and B and output matrix C into n/2 * n/2
  // submatrices. This step takes O(1) time by index calculation
  isubmatrix a11 = isubmatrix_from_submatrix(submatrix_a, n / 2, n / 2, 0, 0);
  isubmatrix a12 =
      isubmatrix_from_submatrix(submatrix_a, n / 2, n / 2, 0, (n / 2));
  isubmatrix a21 =
      isubmatrix_from_submatrix(submatrix_a, n / 2, n / 2, (n / 2), 0);
  isubmatrix a22 =
      isubmatrix_from_submatrix(submatrix_a, n / 2, n / 2, (n / 2), (n / 2));

  isubmatrix b11 = isubmatrix_from_submatrix(submatrix_b, n / 2, n / 2, 0, 0);
  isubmatrix b12 =
      isubmatrix_from_submatrix(submatrix_b, n / 2, n / 2, 0, (n / 2));
  isubmatrix b21 =
      isubmatrix_from_submatrix(submatrix_b, n / 2, n / 2, (n / 2), 0);
  isubmatrix b22 =
      isubmatrix_from_submatrix(submatrix_b, n / 2, n / 2, (n / 2), (n / 2));

  isubmatrix c11 = isubmatrix_from_submatrix(submatrix_c, n / 2, n / 2, 0, 0);
  isubmatrix c12 =
      isubmatrix_from_submatrix(submatrix_c, n / 2, n / 2, 0, (n / 2));
  isubmatrix c21 =
      isubmatrix_from_submatrix(submatrix_c, n / 2, n / 2, (n / 2), 0);
  isubmatrix c22 =
      isubmatrix_from_submatrix(submatrix_c, n / 2, n / 2, (n / 2), (n / 2));

  // aux matrices
  imatrix_t *c_t2 = imatrix_new(n, n);

  isubmatrix c11_t2 = isubmatrix_from_matrix(c_t2, n / 2, n / 2, 0, 0);
  isubmatrix c12_t2 = isubmatrix_from_matrix(c_t2, n / 2, n / 2, 0, (n / 2));
  isubmatrix c21_t2 = isubmatrix_from_matrix(c_t2, n / 2, n / 2, (n / 2), 0);
  isubmatrix c22_t2 =
      isubmatrix_from_matrix(c_t2, n / 2, n / 2, (n / 2), (n / 2));

  // C11
  if (isubmatrix_multiply_recursive(&a11, &b11, &c11) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_multiply_recursive(&a12, &b21, &c11_t2) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_add(&c11, &c11_t2, &c11) < 0) {
    imatrix_free(c_t2);
    return -1;
  }

  // C12
  if (isubmatrix_multiply_recursive(&a11, &b12, &c12) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_multiply_recursive(&a12, &b22, &c12_t2) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_add(&c12, &c12_t2, &c12) < 0) {
    imatrix_free(c_t2);
    return -1;
  }

  // C21
  if (isubmatrix_multiply_recursive(&a21, &b11, &c21) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_multiply_recursive(&a22, &b21, &c21_t2) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_add(&c21, &c21_t2, &c21) < 0) {
    imatrix_free(c_t2);
    return -1;
  }

  // C22
  if (isubmatrix_multiply_recursive(&a21, &b12, &c22) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_multiply_recursive(&a22, &b22, &c22_t2) < 0) {
    imatrix_free(c_t2);
    return -1;
  }
  if (isubmatrix_add(&c22, &c22_t2, &c22) < 0) {
    imatrix_free(c_t2);
    return -1;
  }

  imatrix_free(c_t2);

  return 0;
}

// Wrapper for regular matrix representation
imatrix_t *imatrix_multiply_recursive(imatrix_t *matrix_a,
                                      imatrix_t *matrix_b) {
  size_t rows_a, cols_a, rows_b, cols_b;
  if (imatrix_get_size(matrix_a, &rows_a, &cols_a) < 0) {
    return NULL;
  }
  if (imatrix_get_size(matrix_b, &rows_b, &cols_b) < 0) {
    return NULL;
  }

  size_t n = rows_a;
  if (cols_a != n || rows_b != n || cols_b != n) {
    return NULL;
  }
  if (n % 2 != 0) { // algorithm simplification
    return NULL;
  }

  imatrix_t *matrix_c = imatrix_new(n, n);
  if (matrix_c == NULL) {
    return NULL;
  }

  isubmatrix submatrix_a = isubmatrix_from_matrix(matrix_a, n, n, 0, 0);
  isubmatrix submatrix_b = isubmatrix_from_matrix(matrix_b, n, n, 0, 0);
  isubmatrix submatrix_c = isubmatrix_from_matrix(matrix_c, n, n, 0, 0);

  if (isubmatrix_multiply_recursive(&submatrix_a, &submatrix_b, &submatrix_c) <
      0) {
    return NULL;
  }
  return matrix_c;
}
