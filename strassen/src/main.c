#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

imatrix_t *mat_a, *mat_b, *mat_c;

void free_matrices(void) {
  imatrix_free(mat_a);
  imatrix_free(mat_b);
  imatrix_free(mat_c);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Matrix size must be specified.");
    return -1;
  }
  size_t n = (size_t)atoi(argv[1]);
  mat_a = imatrix_new(n, n);
  if (mat_a == NULL) {
    printf("Failed matrix A creation.");
    free_matrices();
    return -1;
  }
  mat_b = imatrix_new(n, n);
  if (mat_b == NULL) {
    printf("Failed matrix B creation.");
    free_matrices();
    return -1;
  }
  for (size_t i = 0; i < n; i++) {
    if (imatrix_set_value(mat_a, i, i, 2) < 0) {
      free_matrices();
      return -1;
    }
    if (imatrix_set_value(mat_b, i, i, 3) < 0) {
      free_matrices();
      return -1;
    }
  }
  printf("A =\r\n%s\r\n", imatrix_dump(mat_a));
  printf("B =\r\n%s\r\n", imatrix_dump(mat_b));
  mat_c = imatrix_multiply_recursive(mat_a, mat_b);
  if (mat_c == NULL) {
    free_matrices();
    return -1;
  }
  printf("C =\r\n%s\r\n", imatrix_dump(mat_c));
  free_matrices();
  return 0;
}
