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

// Data structure with a submatrix representation of a matrix (matrix view)
typedef struct {
  size_t parent_rows_size;
  size_t parent_cols_size;
  size_t view_rows_size;
  size_t view_cols_size;
  size_t view_rows_offset; // virtual row start for submatrix index calculation
  size_t view_cols_offset; // virtual col start for submatrix index calculation
  size_t pad;
  int *data;
} imatrix_view_t;

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
  if (!matrix)
    return;
  free(matrix->data);
  matrix->data = NULL;
  free(matrix);
}

int imatrix_get_size(imatrix_t *matrix, size_t *rows, size_t *cols) {
  if (matrix == NULL) {
    return -1;
  }
  *rows = matrix->rows;
  *cols = matrix->cols;
  return 0;
}

int imatrix_get_value(imatrix_t *matrix, size_t i, size_t j) {
  return matrix->data[i * matrix->cols + j];
}

void imatrix_set_value(imatrix_t *matrix, size_t i, size_t j, int value) {
  matrix->data[i * matrix->cols + j] = value;
}

imatrix_view_t imatrix_view_from_view(imatrix_view_t parent, int row_off,
                                      int col_off, int rows, int cols) {
  return (imatrix_view_t){
      .data = parent.data,
      .parent_rows_size = parent.parent_rows_size,
      .parent_cols_size = parent.parent_cols_size,
      .view_rows_offset = parent.view_rows_offset + row_off,
      .view_cols_offset = parent.view_cols_offset + col_off,
      .view_rows_size = rows,
      .view_cols_size = cols,
      .pad = 1 // bottom/right padding allowed
  };
}

int imatrix_view_get_value(const imatrix_view_t mat_view, size_t i, size_t j) {
  size_t pi = mat_view.view_rows_offset + i;
  size_t pj = mat_view.view_cols_offset + j;
  if (pi >= mat_view.parent_rows_size || pj >= mat_view.parent_cols_size) {
    return 0; // zero padding
  }
  return mat_view.data[pi * mat_view.parent_cols_size + pj];
}

void imatrix_view_set_value(imatrix_view_t mat_view, size_t i, size_t j,
                            int value) {
  size_t pi = mat_view.view_rows_offset + i;
  size_t pj = mat_view.view_cols_offset + j;
  if (pi >= mat_view.parent_rows_size || pj >= mat_view.parent_cols_size) {
    return;
  }
  mat_view.data[pi * mat_view.parent_cols_size + pj] = value;
}

void imatrix_view_fill(imatrix_view_t view, int value) {
  for (size_t i = 0; i < view.view_rows_size; ++i) {
    size_t pi = view.view_rows_offset + i;

    if (pi >= view.parent_rows_size)
      break; // bottom padding reached

    for (size_t j = 0; j < view.view_cols_size; ++j) {
      size_t pj = view.view_cols_offset + j;

      if (pj >= view.parent_cols_size)
        break; // right padding reached

      view.data[pi * view.parent_cols_size + pj] = value;
    }
  }
}

void imatrix_view_copy(imatrix_view_t src, imatrix_view_t dst) {
  size_t rows = src.view_rows_size;
  size_t cols = src.view_cols_size;

  for (size_t i = 0; i < rows; ++i) {
    size_t di = dst.view_rows_offset + i;

    if (di >= dst.parent_rows_size)
      break;

    for (size_t j = 0; j < cols; ++j) {
      size_t dj = dst.view_cols_offset + j;

      if (dj >= dst.parent_cols_size)
        break;

      int value = imatrix_view_get_value(src, i, j);
      dst.data[di * dst.parent_cols_size + dj] = value;
    }
  }
}

imatrix_t *imatrix_scale(imatrix_t *matrix, int scalar) {
  size_t matrix_rows, matrix_cols;
  if (imatrix_get_size(matrix, &matrix_rows, &matrix_cols) < 0) {
    return NULL;
  }
  imatrix_t *matrix_scaled = imatrix_new(matrix_rows, matrix_cols);
  if (matrix_scaled == NULL) {
    return NULL;
  }

  int aij;
  for (size_t i = 0; i < matrix_rows; i++) {
    for (size_t j = 0; j < matrix_cols; j++) {
      aij = imatrix_get_value(matrix, i, j);
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
  if (matrixa_rows != matrixb_rows || matrixa_cols != matrixb_cols) {
    return NULL;
  }
  imatrix_t *matrix_c = imatrix_new(matrixa_rows, matrixa_cols);
  if (matrix_c == NULL) {
    return NULL;
  }

  int aij, bij;
  for (size_t i = 0; i < matrixa_rows; i++) {
    for (size_t j = 0; j < matrixa_cols; j++) {
      aij = imatrix_get_value(matrix_a, i, j);
      bij = imatrix_get_value(matrix_b, i, j);
      imatrix_set_value(matrix_c, i, j, aij + bij);
    }
  }
  return matrix_c;
}

void imatrix_view_add(const imatrix_view_t mat_view_a,
                      const imatrix_view_t mat_view_b,
                      imatrix_view_t mat_view_c) {

  for (size_t i = 0; i < mat_view_a.view_rows_size; i++) {
    for (size_t j = 0; j < mat_view_a.view_cols_size; j++) {
      imatrix_view_set_value(mat_view_c, i, j,
                             imatrix_view_get_value(mat_view_a, i, j) +
                                 imatrix_view_get_value(mat_view_b, i, j));
    }
  }
}

void imatrix_view_sub(const imatrix_view_t mat_view_a,
                      const imatrix_view_t mat_view_b,
                      imatrix_view_t mat_view_c) {
  for (size_t i = 0; i < mat_view_a.view_rows_size; i++) {
    for (size_t j = 0; j < mat_view_a.view_cols_size; j++) {
      imatrix_view_set_value(mat_view_c, i, j,
                             imatrix_view_get_value(mat_view_a, i, j) -
                                 imatrix_view_get_value(mat_view_b, i, j));
    }
  }
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

  int cij;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      cij = 0;
      for (size_t k = 0; k < n; k++) {
        cij += imatrix_get_value(matrix_a, i, k) *
               imatrix_get_value(matrix_b, k, j);
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

void imatrix_view_split_x4(imatrix_view_t input, imatrix_view_t *a11,
                           imatrix_view_t *a12, imatrix_view_t *a21,
                           imatrix_view_t *a22) {
  int R = input.view_rows_size;
  int C = input.view_cols_size;

  int r_mid = R / 2;
  int c_mid = C / 2;

  int r_pad = R % 2;
  int c_pad = C % 2;

  int block_rows = r_mid + r_pad;
  int block_cols = c_mid + c_pad;

  *a11 = imatrix_view_from_view(input, 0, 0, block_rows, block_cols);
  *a12 = imatrix_view_from_view(input, 0, c_mid, block_rows, block_cols);
  *a21 = imatrix_view_from_view(input, r_mid, 0, block_rows, block_cols);
  *a22 = imatrix_view_from_view(input, r_mid, c_mid, block_rows, block_cols);
}

void imatrix_view_multiply_recursive(const imatrix_view_t A,
                                     const imatrix_view_t B, imatrix_view_t C) {
  size_t n = A.view_rows_size;

  // base case condition
  if (n == 1) {
    imatrix_view_set_value(C, 0, 0,
                           imatrix_view_get_value(A, 0, 0) *
                               imatrix_view_get_value(B, 0, 0));
    return;
  }

  // split A, B, and C in 4 (n/2 x n/2) matrices
  imatrix_view_t A11, A12, A21, A22;
  imatrix_view_t B11, B12, B21, B22;
  imatrix_view_t C11, C12, C21, C22;

  imatrix_view_split_x4(A, &A11, &A12, &A21, &A22);
  imatrix_view_split_x4(B, &B11, &B12, &B21, &B22);
  imatrix_view_split_x4(C, &C11, &C12, &C21, &C22);

  size_t block = (n / 2) + (n % 2);

  // temp matrices
  imatrix_t *T1 = imatrix_new(block, block);
  imatrix_t *T2 = imatrix_new(block, block);
  if (!T1 || !T2) {
    imatrix_free(T1);
    imatrix_free(T2);
    return;
  }

  imatrix_view_t V1 = {.parent_rows_size = block,
                       .parent_cols_size = block,
                       .data = T1->data,
                       .view_rows_offset = 0,
                       .view_cols_offset = 0,
                       .view_rows_size = block,
                       .view_cols_size = block,
                       .pad = 0};

  imatrix_view_t V2 = V1;
  V2.data = T2->data;

  // C11 = A11*B11 + A12*B21
  imatrix_view_fill(V1, 0);
  imatrix_view_fill(V2, 0);
  imatrix_view_multiply_recursive(A11, B11, V1);
  imatrix_view_multiply_recursive(A12, B21, V2);
  imatrix_view_add(V1, V2, C11);

  // C12 = A11*B12 + A12*B22
  imatrix_view_fill(V1, 0);
  imatrix_view_fill(V2, 0);
  imatrix_view_multiply_recursive(A11, B12, V1);
  imatrix_view_multiply_recursive(A12, B22, V2);
  imatrix_view_add(V1, V2, C12);

  // C21 = A21*B11 + A22*B21
  imatrix_view_fill(V1, 0);
  imatrix_view_fill(V2, 0);
  imatrix_view_multiply_recursive(A21, B11, V1);
  imatrix_view_multiply_recursive(A22, B21, V2);
  imatrix_view_add(V1, V2, C21);

  // C22 = A21*B12 + A22*B22
  imatrix_view_fill(V1, 0);
  imatrix_view_fill(V2, 0);
  imatrix_view_multiply_recursive(A21, B12, V1);
  imatrix_view_multiply_recursive(A22, B22, V2);
  imatrix_view_add(V1, V2, C22);

  imatrix_free(T1);
  imatrix_free(T2);
}

void imatrix_view_multiply_strassen(const imatrix_view_t A,
                                    const imatrix_view_t B, imatrix_view_t C) {
  size_t n = A.view_rows_size;

  // base case condition
  if (n == 1) {
    imatrix_view_set_value(C, 0, 0,
                           imatrix_view_get_value(A, 0, 0) *
                               imatrix_view_get_value(B, 0, 0));
    return;
  }

  // split A, B, and C in 4 (n/2 x n/2) matrices
  imatrix_view_t A11, A12, A21, A22;
  imatrix_view_t B11, B12, B21, B22;
  imatrix_view_t C11, C12, C21, C22;

  imatrix_view_split_x4(A, &A11, &A12, &A21, &A22);
  imatrix_view_split_x4(B, &B11, &B12, &B21, &B22);
  imatrix_view_split_x4(C, &C11, &C12, &C21, &C22);

  size_t block = (n / 2) + (n % 2);

  // temp matrices
  imatrix_t *T1 = imatrix_new(block, block);
  imatrix_t *T2 = imatrix_new(block, block);
  imatrix_t *P1 = imatrix_new(block, block);
  imatrix_t *P2 = imatrix_new(block, block);
  imatrix_t *P3 = imatrix_new(block, block);
  imatrix_t *P4 = imatrix_new(block, block);
  imatrix_t *P5 = imatrix_new(block, block);
  imatrix_t *P6 = imatrix_new(block, block);
  imatrix_t *P7 = imatrix_new(block, block);

  imatrix_view_t vT1 = {block, block, block, block, 0, 0, 0, T1->data};
  imatrix_view_t vT2 = vT1;
  vT2.data = T2->data;

  imatrix_view_t vP1 = vT1;
  vP1.data = P1->data;
  imatrix_view_t vP2 = vT1;
  vP2.data = P2->data;
  imatrix_view_t vP3 = vT1;
  vP3.data = P3->data;
  imatrix_view_t vP4 = vT1;
  vP4.data = P4->data;
  imatrix_view_t vP5 = vT1;
  vP5.data = P5->data;
  imatrix_view_t vP6 = vT1;
  vP6.data = P6->data;
  imatrix_view_t vP7 = vT1;
  vP7.data = P7->data;

  // P1 = (A11 + A22)(B11 + B22)
  imatrix_view_add(A11, A22, vT1);
  imatrix_view_add(B11, B22, vT2);
  imatrix_view_multiply_strassen(vT1, vT2, vP1);

  // P2 = (A21 + A22)B11
  imatrix_view_add(A21, A22, vT1);
  imatrix_view_multiply_strassen(vT1, B11, vP2);

  // P3 = A11(B12 − B22)
  imatrix_view_sub(B12, B22, vT2);
  imatrix_view_multiply_strassen(A11, vT2, vP3);

  // P4 = A22(B21 − B11)
  imatrix_view_sub(B21, B11, vT2);
  imatrix_view_multiply_strassen(A22, vT2, vP4);

  // P5 = (A11 + A12)B22
  imatrix_view_add(A11, A12, vT1);
  imatrix_view_multiply_strassen(vT1, B22, vP5);

  // P6 = (A21 − A11)(B11 + B12)
  imatrix_view_sub(A21, A11, vT1);
  imatrix_view_add(B11, B12, vT2);
  imatrix_view_multiply_strassen(vT1, vT2, vP6);

  // P7 = (A12 − A22)(B21 + B22)
  imatrix_view_sub(A12, A22, vT1);
  imatrix_view_add(B21, B22, vT2);
  imatrix_view_multiply_strassen(vT1, vT2, vP7);

  // C11 = P1 + P4 − P5 + P7
  imatrix_view_add(vP1, vP4, vT1);
  imatrix_view_sub(vT1, vP5, vT2);
  imatrix_view_add(vT2, vP7, C11);

  // C12 = P3 + P5
  imatrix_view_add(vP3, vP5, C12);

  // C21 = P2 + P4
  imatrix_view_add(vP2, vP4, C21);

  // C22 = P1 − P2 + P3 + P6
  imatrix_view_sub(vP1, vP2, vT1);
  imatrix_view_add(vT1, vP3, vT2);
  imatrix_view_add(vT2, vP6, C22);

  imatrix_free(T1);
  imatrix_free(T2);
  imatrix_free(P1);
  imatrix_free(P2);
  imatrix_free(P3);
  imatrix_free(P4);
  imatrix_free(P5);
  imatrix_free(P6);
  imatrix_free(P7);
}

size_t next_power2(size_t n) {
  if (n <= 1)
    return 1;

  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  return n + 1;
}

imatrix_t *imatrix_pad_to_p(const imatrix_t *src, size_t p) {
  if (!src || src->rows != src->cols)
    return NULL;

  size_t n = src->rows;

  imatrix_t *dst = imatrix_new(p, p);
  if (!dst)
    return NULL;

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      dst->data[i * p + j] = src->data[i * n + j];
    }
  }

  return dst;
}

imatrix_t *imatrix_top_left(const imatrix_t *src, size_t n) {
  if (!src || n > src->rows || n > src->cols)
    return NULL;

  imatrix_t *dst = imatrix_new(n, n);
  if (!dst)
    return NULL;

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      dst->data[i * n + j] = src->data[i * src->cols + j];
    }
  }

  return dst;
}

imatrix_t *imatrix_multiply_recursive(imatrix_t *mat_a, imatrix_t *mat_b) {
  size_t n = mat_a->rows;
  size_t n2 = next_power2(n);
  imatrix_t *mat_a_tmp = imatrix_pad_to_p(mat_a, n2);
  imatrix_t *mat_b_tmp = imatrix_pad_to_p(mat_b, n2);
  imatrix_view_t mat_view_a = {.parent_rows_size = n2,
                               .parent_cols_size = n2,
                               .data = mat_a_tmp->data,
                               .view_rows_offset = 0,
                               .view_cols_offset = 0,
                               .view_rows_size = n2,
                               .view_cols_size = n2,
                               .pad = 1};
  imatrix_view_t mat_view_b = {.parent_rows_size = n2,
                               .parent_cols_size = n2,
                               .data = mat_b_tmp->data,
                               .view_rows_offset = 0,
                               .view_cols_offset = 0,
                               .view_rows_size = n2,
                               .view_cols_size = n2,
                               .pad = 1};

  imatrix_t *mat_c_tmp = imatrix_new(n2, n2);
  imatrix_view_t mat_view_c = {.parent_rows_size = n2,
                               .parent_cols_size = n2,
                               .data = mat_c_tmp->data,
                               .view_rows_offset = 0,
                               .view_cols_offset = 0,
                               .view_rows_size = n2,
                               .view_cols_size = n2,
                               .pad = 1};
  imatrix_view_multiply_strassen(mat_view_a, mat_view_b, mat_view_c);
  imatrix_t *mat_c = imatrix_top_left(mat_c_tmp, n);
  imatrix_free(mat_a_tmp);
  imatrix_free(mat_b_tmp);
  imatrix_free(mat_c_tmp);
  return mat_c;
}
