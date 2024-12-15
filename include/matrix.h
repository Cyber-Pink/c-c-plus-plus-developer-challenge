#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_defines_enum.h"

struct matrix
{
    unsigned int number_of_dimensions;
    unsigned int number_of_data_points;
    unsigned int *shape;
    double *data;
    char* name;

};

enum ERRO_CODE Create_Matrix(struct matrix **matrix, unsigned int* shape, double* data, unsigned int number_of_dimensions, unsigned int number_of_data_points, char* name);
enum ERRO_CODE Destroy_Matrix(struct matrix *matrix);

#endif