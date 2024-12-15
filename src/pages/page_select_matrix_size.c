#include "pages/page_select_matrix_size.h"

static const char *DESC = 
"Please select a matrix size for your new matrix.";

static int num_options_matrix = 6;
static const char *options_matrix[] = {
    "0 - Input custom dimensions",
    "1 - [1 X 1] (fast creation not yet implemented)",
    "2 - [2 X 2] (fast creation not yet implemented)",
    "3 - [3 X 3] (fast creation not yet implemented)",
    "4 - [3 X 3 X 3] (fast creation not yet implemented)",
    "5 - Back to menu"
};

AppPage matrix_option(int option_selected){
    switch (option_selected) {
    case 0: return PAGE_CUSTOM_DIMENSIONS; // [1 X 1] not implemented yet
    case 1: return PAGE_MAIN_MENU; // [2 X 2] not implemented yet
    case 2: return PAGE_MAIN_MENU; // [3 X 3] not implemented yet
    case 3: return PAGE_MAIN_MENU; // [3 X 3 X 3] not implemented yet
    case 4: return PAGE_MAIN_MENU; // create a custom matrix
    case 5: PAGE_MAIN_MENU; // back to menu
    default:
        return PAGE_MAIN_MENU;
    }
}

AppPage run_select_matrix_size_page() {
    int logo_bottom = hmi_draw_logo_and_desc(DESC, "Select matrix size");

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    hmi_draw_horizontal_line(logo_bottom + 14, 2, max_x - 2, ACS_HLINE);
    mvprintw(logo_bottom + 16, 2, "Use Up/Down to navigate, Enter to select");

    int start_y = logo_bottom + 6;
    int start_x = 4;

    return(matrix_option(HighLight_menu(num_options_matrix, start_y, start_x, options_matrix)));

    return PAGE_MAIN_MENU;
}
