#include "matrix.h"

// Function that creates and validates a matrix
enum ERRO_CODE Create_Matrix(struct matrix **matrix, 
                            unsigned int* shape, 
                            double* data, 
                            unsigned int number_of_dimensions, 
                            unsigned int number_of_data_points, 
                            char* name){

    //verify if the user did not "forgot" to input the dimensions
    if (number_of_dimensions == 0){
        return MATRIX_DIMENSION_EQUAL_ZERO;
    }
    //verify if the user did not "forgot" to input the data
    if (number_of_data_points == 0){
        return MATRIX_DATA_EMPTY;
    }
    // I see you trying to break the code!
    // There is no particular reason why the max number is set to 1024 except the fact that this is just a demo program and not actually a toolbox
    if (number_of_dimensions > MAX_DIMENSIONS){
        return MATRIX_DIMENSION_OUTSIDE_PERMITED_BOUNDERIES;
    }

    //verify if the number of dimensions matches the size of the matrix
    unsigned int array_total_length = 1;
    for (unsigned int i=0; i!= number_of_dimensions; i++){
        if (shape[i] == 0 || shape[i] > MAX_DIMENSIONS){
            return MATRIX_SHAPE_OUTSIDE_PERMITED_BOUNDERIES;
        }
        array_total_length *= shape[i];
    }
    if (array_total_length != number_of_data_points){
        return MATRIX_DIMENSION_NOT_MATCHING_DATASIZE;
    }


    //alocate memory to a matrix.
    struct matrix *M;
    M = malloc(sizeof(struct matrix));
    if (!M) {
        return MEMORY_ALLOCATION_FAILED;
    }

    double* d;
    d = malloc(sizeof(double) * number_of_data_points);
    if (!d) {
        free(M); // i'm my own garbage collector :D
        return MEMORY_ALLOCATION_FAILED;
    }
    memcpy(d, data, sizeof(double) * number_of_data_points);
    M->data = d;

    unsigned int* s;
    s = malloc(sizeof(unsigned int) * number_of_dimensions);
    if (!s) {
        free(M->data);
        free(M);
        return MEMORY_ALLOCATION_FAILED;
    }
    memcpy(s, shape, sizeof(unsigned int) *number_of_dimensions);
    M->shape = s;

    char* n;
    n = malloc(sizeof(double) * MATRIX_MAX_NAME_SIZE);
    if (!n) {
        free(M->data);
        free(M->shape);
        free(M);
        return MEMORY_ALLOCATION_FAILED;
    }
    memcpy(n, name, sizeof(char) * MATRIX_MAX_NAME_SIZE);
    M->name = n;


    M->number_of_dimensions = number_of_dimensions;
    M->number_of_data_points = number_of_data_points;

    //Pass the alocated memory matrix back
    *matrix = M;

    return NO_ERROR;
}

enum ERRO_CODE Destroy_Matrix(struct matrix *matrix) {
    // I still need to test this function
    
    if (!matrix) {
        // If the matrix is already NULL, there's no error
        return NO_ERROR;
    }

    if (matrix->data) {
        free(matrix->data);
        matrix->data = NULL; // No dangling pointers in this code :)
    }

    if (matrix->shape) {
        free(matrix->shape);
        matrix->shape = NULL;
    }

    // TODO free the name of the matrix

    free(matrix); // Finally, free the matrix structure itself
    matrix = NULL; // Not necessary, but good practice to avoid potential misuse

    return NO_ERROR;
}