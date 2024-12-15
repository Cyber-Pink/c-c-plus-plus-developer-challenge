#ifndef PAGE_PERFORM_OPERATION_H
#define PAGE_PERFORM_OPERATION_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "log.h"
#include "hmi.h"
#include "matrix.h"
#include "operations.h"
#include "global_defines_enum.h"

#include "pages.h"

/**
 * @brief Run the page where user selects an operation and performs it on existing matrices.
 */
AppPage run_perform_operation_page();

#endif
