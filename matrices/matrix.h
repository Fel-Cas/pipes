#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

typedef struct Vector Vector;
struct Vector {
    int size;
    double* elements;
};

typedef struct Matrix Matrix;
struct Matrix {
    int rows;
    int cols;
    double** elements;
};

Vector* create_vector(int size);
Matrix* create_matrix(int rows, int cols);
Matrix* create_matrix_from_file(const char* file_path, int rows, int cols);
void free_matrix(Matrix* M);
Vector* matrix_col_max(Matrix* M);
Vector* matrix_col_min(Matrix* M);
void print_matrix(const Matrix* M);
void normalize_matrix_column_formula_1(Matrix* M, Vector* mayores, Vector* menores);
#endif