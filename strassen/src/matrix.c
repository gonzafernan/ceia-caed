/**
 * @file matrix.c
 * @author Gonzalo G. Fernandez
 * @brief Implementation of 2D matrix operations.
 */

#include <stdio.h>
#include <stdlib.h>

// Data structure for 2D matrix matrix of int.
typedef struct imatrix_s {
  size_t rows;
  size_t cols;
  int *data;
} imatrix_t;

imatrix_t *imatrix_new(size_t rows, size_t cols) {
  imatrix_t *matrix = malloc(sizeof(imatrix_t));
  if (matrix == NULL) {
    return NULL;
  }
  matrix->rows = rows;
  matrix->cols = cols;
  matrix->data = calloc(rows * cols, sizeof(int));
  if (matrix->data == NULL) {
    free(matrix);
    return NULL;
  }
  return matrix;
}

void imatrix_free(imatrix_t *matrix) {
  free(matrix->data);
  matrix->data = NULL;
  free(matrix);
  matrix = NULL;
}

int imatrix_get_size(imatrix_t *matrix, size_t *rows, size_t *cols) {
  if (matrix == NULL) {
    return -1;
  }
  *rows = matrix->rows;
  *cols = matrix->cols;
  return 0;
}

int imatrix_get_value(imatrix_t *matrix, size_t i, size_t j, int *value) {
  if (i >= matrix->rows || j >= matrix->cols) {
    return -1;
  }
  *value = matrix->data[i * matrix->cols + j];
  return 0;
}

int imatrix_set_value(imatrix_t *matrix, size_t i, size_t j, int value) {
  if (i >= matrix->rows || j >= matrix->cols) {
    return -1;
  }
  matrix->data[i * matrix->cols + j] = value;
  return 0;
}

imatrix_t *imatrix_scale(imatrix_t *matrix, int scalar) {
  size_t matrix_rows, matrix_cols;
  if (imatrix_get_size(matrix, &matrix_rows, &matrix_cols) < 0) {
    return NULL;
  }
  size_t n = matrix_rows;
  if (matrix_cols != n || matrix_rows != n) {
    return NULL;
  }
  imatrix_t *matrix_scaled = imatrix_new(n, n);
  if (matrix_scaled == NULL) {
    return NULL;
  }

  int aij = 0;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      imatrix_get_value(matrix, i, j, &aij);
      imatrix_set_value(matrix_scaled, i, j, aij * scalar);
    }
  }
  return matrix_scaled;
}

imatrix_t *imatrix_add(imatrix_t *matrix_a, imatrix_t *matrix_b) {
  size_t matrixa_rows, matrixa_cols;
  size_t matrixb_rows, matrixb_cols;
  if (imatrix_get_size(matrix_a, &matrixa_rows, &matrixa_cols) < 0) {
    return NULL;
  }
  if (imatrix_get_size(matrix_b, &matrixb_rows, &matrixb_cols) < 0) {
    return NULL;
  }
  size_t n = matrixa_rows;
  if (matrixa_cols != n || matrixb_rows != n || matrixb_cols != n) {
    return NULL;
  }
  imatrix_t *matrix_c = imatrix_new(n, n);
  if (matrix_c == NULL) {
    return NULL;
  }

  int aij = 0, bij = 0;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      imatrix_get_value(matrix_a, i, j, &aij);
      imatrix_get_value(matrix_b, i, j, &bij);
      imatrix_set_value(matrix_c, i, j, aij + bij);
    }
  }
  return matrix_c;
}

imatrix_t *imatrix_substract(imatrix_t *matrix_a, imatrix_t *matrix_b) {
  size_t matrixa_rows, matrixa_cols;
  size_t matrixb_rows, matrixb_cols;
  if (imatrix_get_size(matrix_a, &matrixa_rows, &matrixa_cols) < 0) {
    return NULL;
  }
  if (imatrix_get_size(matrix_b, &matrixb_rows, &matrixb_cols) < 0) {
    return NULL;
  }
  size_t n = matrixa_rows;
  if (matrixa_cols != n || matrixb_rows != n || matrixb_cols != n) {
    return NULL;
  }
  imatrix_t *matrix_c = imatrix_new(n, n);
  if (matrix_c == NULL) {
    return NULL;
  }

  int aij = 0, bij = 0;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      imatrix_get_value(matrix_a, i, j, &aij);
      imatrix_get_value(matrix_b, i, j, &bij);
      imatrix_set_value(matrix_c, i, j, aij - bij);
    }
  }
  return matrix_c;
}

imatrix_t *imatrix_multiply_brute_force(imatrix_t *matrix_a,
                                        imatrix_t *matrix_b) {
  size_t matrixa_rows, matrixa_cols;
  size_t matrixb_rows, matrixb_cols;
  if (imatrix_get_size(matrix_a, &matrixa_rows, &matrixa_cols) < 0) {
    return NULL;
  }
  if (imatrix_get_size(matrix_b, &matrixb_rows, &matrixb_cols) < 0) {
    return NULL;
  }
  size_t n = matrixa_rows;
  if (matrixa_cols != n || matrixb_rows != n || matrixb_cols != n) {
    return NULL;
  }
  imatrix_t *matrix_c = imatrix_new(n, n);
  if (matrix_c == NULL) {
    return NULL;
  }

  int cij = 0, aik = 0, bkj = 0;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      cij = 0;
      for (size_t k = 0; k < n; k++) {
        imatrix_get_value(matrix_a, i, k, &aik);
        imatrix_get_value(matrix_b, k, j, &bkj);
        cij += aik * bkj;
      }
      imatrix_set_value(matrix_c, i, j, cij);
    }
  }
  return matrix_c;
}

char *imatrix_dump(imatrix_t *matrix) {
  if (matrix == NULL) {
    return NULL;
  }

  // Get required size of the string
  size_t total = 1; // null terminator

  for (size_t i = 0; i < matrix->rows; ++i) {
    for (size_t j = 0; j < matrix->cols; ++j) {
      total += snprintf(NULL, 0, "%d ", matrix->data[i * matrix->cols + j]);
    }
    total += 2; // newline (\r\n)
  }

  char *str = malloc(total);
  if (!str)
    return NULL;

  /* Second pass: write string */
  char *p = str;
  for (size_t i = 0; i < matrix->rows; ++i) {
    for (size_t j = 0; j < matrix->cols; ++j) {
      p += sprintf(p, "%d ", matrix->data[i * matrix->cols + j]);
    }
    *p++ = '\r';
    *p++ = '\n';
  }
  *p = '\0';

  return str;
}
