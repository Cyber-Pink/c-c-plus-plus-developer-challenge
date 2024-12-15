#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "matrix.h"
#include "global_defines_enum.h"

/**
 * @brief Adds two matrices with the same dimensions.
 * @param m1 First matrix
 * @param m2 Second matrix
 * @param out_result Pointer to a struct matrix* that will hold the new allocated result matrix.
 * @return NO_ERROR if success, otherwise an error code.
 */
enum ERRO_CODE Matrix_Add(struct matrix *m1, struct matrix *m2, struct matrix **resulting_matrix);




/**
 * @brief Calculates the determinant of a nxn matrix.
 * @param m The matrix
 * @param out_value Pointer to a double that will hold the determinant value
 * @return NO_ERROR if success, otherwise an error code.
 */
enum ERRO_CODE Matrix_Determinant(const struct matrix *m, double *out_value);


#endif
