#include "operations.h"
/**
 * The operations library will only perform mathematical operations on the matrices passed to it
 * it is the rensponsability of the caller to put the matrix in the global matrix list.
 */

enum ERRO_CODE Matrix_Add(struct matrix *m1, struct matrix *m2, struct matrix **resulting_matrix){

    // 1 - check if the parameters passed exist.
        // as resulting_memory was not allocated yet it must be NULL.
    if (!m1 || !m2 || !resulting_matrix) return MEMORY_ALLOCATION_FAILED;

    // 2 - check if the number of dimensions match.
    if (m1->number_of_dimensions != m2->number_of_dimensions) return MATRIX_DIMENSION_SIZE_DONT_MATCH;

    // 3 - check if the shapes match.
    for (unsigned int i = 0; i < m1->number_of_dimensions; i++){
        if (m1->shape[i] != m2->shape[i]) return MATRIX_SHAPE_DONT_MATCH;
    }

    // 4 - check if the number of data points match. (don't trust verify)
    if (m1->number_of_data_points != m2->number_of_data_points) return MATRIX_DATA_SIZE_DONT_MATCH;

    // 5- based on the matrix struct. (create a temporary open matrix)
    unsigned int internal_number_of_dimensions = m1->number_of_dimensions;
    unsigned int internal_number_of_data_points = m1->number_of_data_points;
    unsigned int internal_shape[internal_number_of_dimensions];
    double internal_data[internal_number_of_data_points];
    char internal_name[MATRIX_MAX_NAME_SIZE];
    for (unsigned int i = 0; i < internal_number_of_dimensions; i++){
        internal_shape[i] = m1->shape[i];
    }
    snprintf(internal_name, MATRIX_MAX_NAME_SIZE, "AddResult"); // placeholder name

    // 6 - Peform the operation (ADD)
    for (unsigned int i = 0; i < m1->number_of_data_points; i++){
        internal_data[i] = m1->data[i] + m2->data[i];
    }

    // 8 - allocate the memory of the matrix to hold the results
    enum ERRO_CODE matrix_creation_status = COMMON_ERRO;

    matrix_creation_status = Create_Matrix( resulting_matrix,
                                            internal_shape,
                                            internal_data,
                                            internal_number_of_dimensions,
                                            internal_number_of_data_points,
                                            internal_name);

    if (matrix_creation_status != NO_ERROR) return matrix_creation_status;

    return NO_ERROR;
}

enum ERRO_CODE Matrix_Determinant(const struct matrix *m, double *out_value){

    // 1- Check if the marix has no more than two dimensions
        // The determinant don't generalize for matrices of greater dimensions
    if (m->number_of_dimensions > 2) return MATRIX_HAS_TOO_MANY_DIMENSIONS;

    // 2 - check if the matrix is a square matrix.
        // For a matrix to be square all of its dimensions must be equal
        // I will compare all of the matrices dimensions with it's fisrt dimension
    unsigned int first_dimension = m->shape[0];
    for (unsigned int i = 0; i < m->number_of_dimensions; i++){
        if (first_dimension != m->shape[i]) return MATRIX_NOT_SQUARE;
    }


    /* ------------ LU DECOMPOSITION STARTS HERE --------------------- */

    // 3 - create the matrix L and U as 2D arrays (i just need the data for the determinant)
    double U[m->shape[0]][m->shape[0]];
    double L[m->shape[0]][m->shape[0]];

    // 4 - Copy the data from the matrix to U
    memcpy(U, m->data, sizeof(double) * m->number_of_data_points);

    // Keep track of the swaps
    // every time you perform a roll swap, the determinant sign changes
    int swaps_multiplyer = 1;
    unsigned int number_of_swaps_looked = 0;

    // start the process of pivot decomposition
    for (unsigned int r = 0; r < m->shape[0]; r++){ // iterates trough rows

        // check the pivot is zero, if it is perform a row swap
        // Standard LU decomposition moves left-to-right, top-to-bottom and doesn’t “go back”
        if (U[r][r] == 0 ){

            // if the last pivot is a zero the matrix is singular and its determinant is equal to zero
            if (r == m->shape[0] - 1){
                *out_value = 0;
                return NO_ERROR;
            }

            // look for a point in that column that is non zero
            // it is not legal to perform swaps with rows on top of the pivot so the column starts at the pivot that is the row, or r in this case
            unsigned int flag_row_found = 0;
            for (unsigned int c = r; c < m->shape[0] && !flag_row_found; c++){

                // if the point is non zero, perform a row swap
                if (U[r][c] != 0){
                    double temp = 0;
                    for (unsigned int x = 0; x < m->shape[0]; x++){
                        temp = U[r][x];
                        U[r][x] = U[c][x];
                        U[c][x] = temp;
                    }
                    // break flag to get out of the loop
                    flag_row_found = 1;
                }
            }

            // if no new pivot was found this matrix is probably not solvable by only LU decomposition, it might by PLUQ, but implementing that will take some time...
            if (flag_row_found == 0){

                // Do the extensive check to verify if the matrix is singular or not!
                for (unsigned int i= 0; i<m->shape[0]; i++){
                    unsigned int num_of_zeros_in_row = 0;
                    for (unsigned int j = 0; j<m->shape[0]; j++){
                        if (U[i][j] == 0) num_of_zeros_in_row++;
                    }
                    if (num_of_zeros_in_row == m->shape[0]){
                        // matrix is indeed singular
                        *out_value = 0;
                        return NO_ERROR;
                    }
                }
                for (unsigned int j = 0; j<m->shape[0]; j++){
                    unsigned int num_of_zeros_in_row = 0;
                    for (unsigned int i = 0; i<m->shape[0]; i++){
                        if (U[i][j] == 0) num_of_zeros_in_row++;
                    }
                    if (num_of_zeros_in_row == m->shape[0]){
                        // matrix is indeed singular
                        *out_value = 0;
                        return NO_ERROR;
                    }
                }

                return MATRIX_NOT_SOLVABLE_BY_LU_DECOMPOSITION;
                *out_value = 0;
                }
        // keep track of the multiplication value for the determinant
        swaps_multiplyer *= -1;
        }


        // gaussian removal part
        for (unsigned int sr = r+1; sr < m->shape[0]; sr++){
            // first you scale the row
            double temp_scaled_row[m->shape[0]];
            memcpy(temp_scaled_row, U[r], sizeof(double)*m->shape[0]);
            double temp_scale = U[sr][r] / U[r][r]; // as we are iterating diagonally, its ok to use the row in the column
            
            // then the subtraction by the scalar is made
            for (unsigned int c = r; c < m->shape[0]; c++){
                U[sr][c] = U[sr][c] - temp_scaled_row[c] * temp_scale;
            }
        }
    }

    // after all that, just calcuate the determinant 
    // (it could be nested togueter with the loop above, but i separated it for readability)
    double determinat = 1;
    for (unsigned int r = 0; r < m->shape[0]; r++){
        determinat = determinat * U[r][r];
    }

    //correct the signal according to the row swaps
    determinat = determinat * swaps_multiplyer;

    // pass the determinant back via pointer
    *out_value = determinat;

    return NO_ERROR;
}