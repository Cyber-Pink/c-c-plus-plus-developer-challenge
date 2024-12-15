#ifndef PAGE_MAIN_MENU_H
#define PAGE_MAIN_MENU_H

#include <string.h>
#include <stdio.h>
#include "pages/page_select_matrix_size.h"
#include "global_defines_enum.h"
#include "hmi.h"
#include "matrix.h"

#include "pages.h"

/**
 * @brief Runs the main menu page.
 * @return The next AppPage state based on user choice.
 */
AppPage run_main_menu();

#endif
