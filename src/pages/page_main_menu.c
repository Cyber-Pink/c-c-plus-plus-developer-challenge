#include "pages/page_main_menu.h"

// Extern globals
extern unsigned int Global_matrix_counter;
extern struct matrix *List_of_matrices[MAXIMUM_MATRICES_ON_GLOBAL];

static const char *TOOLBOX_DESC = 
"A toolbox to matrix operations created by Sara Raizer as a showcase of C programming abilities.";

unsigned int num_options_menu = 5;
static const char *options_menu[] = {
    "0 - Create matrix",
    "1 - List matrices (not implemented yet)",
    "2 - Perform operation",
    "3 - Help / documentation (not implemented yet)",
    "4 - Exit"
};

AppPage menu_option(int option_selected){
    switch (option_selected) {
    case 0: return PAGE_SELECT_MATRIX_SIZE;
    case 1: /* List matrices could trigger a page that just prints them or refreshes */ return PAGE_MAIN_MENU;
    case 2: return PAGE_PERFORM_OPERATION;
    case 3: /* Help not implemented yet */ return PAGE_MAIN_MENU;
    case 4: return PAGE_EXIT;
    default:
        return PAGE_EXIT;
    }
}

AppPage run_main_menu() {
    // Draw the main menu layout
    int logo_bottom = hmi_draw_logo_and_desc(TOOLBOX_DESC, "Menu");

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int half_x = max_x / 2; // Quarter point in the width.

    mvprintw(logo_bottom + 5, 4, "options");
    mvprintw(logo_bottom + 5, half_x + 5, "Matrices");

    // Middle vertical line of the interface.
    hmi_draw_vertical_line(half_x + 2, logo_bottom + 4, logo_bottom + 14, ACS_VLINE);
    // Bottom line of the interface.
    hmi_draw_horizontal_line(logo_bottom + 14, 2, max_x - 2, ACS_HLINE);
    // Instructions at the bottom
    mvprintw(logo_bottom + 16, 2, "Use Up/Down to navigate, Enter to select");

    int start_menu_y = logo_bottom + 7;
    int start_menu_x = 4;

    // Display the matrices from the global list
    // We'll print them starting from half_x + 5, one per line
    int start_mat_y = logo_bottom + 7;
    int start_mat_x = half_x + 5;

    // Clear the area where the matrices are shown (optional)
    // For safety, ensure there's enough space from logo_bottom+7 to logo_bottom+14
    for (int clear_y = start_mat_y; clear_y < logo_bottom + 14; clear_y++) {
        move(clear_y, start_mat_x);
        clrtoeol();
    }

    // Print each matrix's info
    // For each matrix, print something like: "MatrixName (#dim: D, shape: XxYxZ...)"
    for (unsigned int i = 0; i < Global_matrix_counter && i < MAXIMUM_MATRICES_ON_GLOBAL; i++) {
        struct matrix *mat = List_of_matrices[i];
        if (mat) {
            // Construct a shape string: "XxYxZ..."
            char shape_str[256] = {0};
            if (mat->number_of_dimensions > 0) {
                char temp[32];
                snprintf(temp, sizeof(temp), "%u", mat->shape[0]);
                strcat(shape_str, temp);
                for (unsigned int d = 1; d < mat->number_of_dimensions; d++) {
                    snprintf(temp, sizeof(temp), "x%u", mat->shape[d]);
                    strcat(shape_str, temp);
                }
            }

            // If there is a name in the matrix structure, print it. Otherwise, just print "Unnamed"
            const char *name_to_print = (mat->name) ? mat->name : "Unnamed";

            mvprintw(start_mat_y + i, start_mat_x, "%s (%uD: %s)", 
                     name_to_print,
                     mat->number_of_dimensions,
                     shape_str);
        }
    }

    return menu_option(HighLight_menu(num_options_menu, start_menu_y, start_menu_x, options_menu));

    // Should never reach here, but who knows ...
    return PAGE_MAIN_MENU;
}
