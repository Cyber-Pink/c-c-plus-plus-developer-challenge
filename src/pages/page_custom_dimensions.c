#include "pages/page_custom_dimensions.h"

// Global variables (extern)
extern unsigned int Global_matrix_counter;
extern struct matrix *List_of_matrices[MAXIMUM_MATRICES_ON_GLOBAL];

static const char *DESC = "Please enter the number of dimensions for your new matrix.";

/**
 * @brief Redraw the initial layout for the custom dimensions page.
 *        Returns the bottom line of the logo area for reference.
 */
static int draw_initial_layout() {
    clear();
    int logo_bottom = hmi_draw_logo_and_desc(DESC, "Input Custom Dimensions");
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    mvprintw(logo_bottom + 6, 2, "For example, '2' for a 2D matrix, '3' for a 3D matrix, etc.");
    mvprintw(logo_bottom + 8, 2, "Type the number and press ENTER. Press ESC to go back to the main menu.");
    mvprintw(logo_bottom + 10, 2, "Number of dimensions: ");
    refresh();

    return logo_bottom;
}

/**
 * @brief Validate the input as a positive integer with no leading zeros.
 */
static int validate_input(const char *input) {
    if (strlen(input) == 0) return 0;
    if (input[0] == '0') return 0;
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isdigit((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Convert validated string to unsigned int.
 */
static int convert_to_unsigned(const char *input, unsigned int *output) {
    unsigned long long temp = strtoull(input, NULL, 10);
    if (temp > UINT_MAX) {
        return 0; // overflow
    }
    *output = (unsigned int)temp;
    return 1;
}

/**
 * @brief Prompt the user for a single positive integer input using prompt_for_unsigned_int.
 * If user presses ESC or enters invalid input, return 0.
 */
static int get_number_of_dimensions(unsigned int *number_of_dimensions) {
    // Just read the input directly from the line below "Number of dimensions: "
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    return prompt_for_unsigned_int("Enter the number of dimensions Example 2 to a 2D matrix, \n or 3 for a 3D matrix the program can work with any number of dimensions bellow 1024:", number_of_dimensions);
}

/**
 * @brief Prompt the user for each dimension size using prompt_for_unsigned_int.
 * Returns 1 if all dims read successfully, 0 otherwise (ESC or error).
 */
static int get_dimensions_sizes(unsigned int number_of_dimensions, unsigned int *shape) {
    for (unsigned int i = 0; i < number_of_dimensions; i++) {
        char prompt_msg[128];
        snprintf(prompt_msg, sizeof(prompt_msg), "Enter the size for dimension %u (ESC to cancel):", i+1);
        unsigned int dim_size;
        if (!prompt_for_unsigned_int(prompt_msg, &dim_size)) {
            return 0; // user pressed ESC or invalid input
        }
        shape[i] = dim_size;
    }
    return 1;
}

/**
 * @brief Prompt the user for data points using prompt_for_multiple_doubles.
 * Returns 1 if successful, 0 if ESC or invalid input.
 */
static int get_data_points(unsigned int number_of_data_points, double *data) {
    char data_prompt[128];
    snprintf(data_prompt, sizeof(data_prompt),
             "Enter %u data values separated by , ", number_of_data_points);
    if (!prompt_for_multiple_doubles(data_prompt, number_of_data_points, data)) {
        return 0;
    }
    return 1;
}

AppPage run_custom_dimensions_page() {
    int logo_bottom = draw_initial_layout();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Step 1: Get number_of_dimensions
    unsigned int number_of_dimensions;
    if (!get_number_of_dimensions(&number_of_dimensions)) {
        // User pressed ESC or invalid input
        return PAGE_MAIN_MENU;
    }

    // Step 2: Allocate shape array and get dimension sizes
    unsigned int *shape = malloc(sizeof(unsigned int)*number_of_dimensions);
    if (!shape) {
        mvprintw(max_y - 2, 2, "Memory allocation error. Press any key...");
        refresh();
        getch();
        return PAGE_MAIN_MENU;
    }

    if (!get_dimensions_sizes(number_of_dimensions, shape)) {
        // ESC or invalid input
        free(shape);
        return PAGE_MAIN_MENU;
    }

    // Step 3: Compute total number_of_data_points
    unsigned int number_of_data_points = 1;
    for (unsigned int i = 0; i < number_of_dimensions; i++) {
        number_of_data_points *= shape[i];
    }

    // Step 4: Allocate data array and get data points
    double *data = malloc(sizeof(double)*number_of_data_points);
    if (!data) {
        free(shape);
        mvprintw(max_y - 2, 2, "Memory allocation error. Press any key...");
        refresh();
        getch();
        return PAGE_MAIN_MENU;
    }

    if (!get_data_points(number_of_data_points, data)) {
        // ESC or invalid input
        free(shape);
        free(data);
        return PAGE_MAIN_MENU;
    }

    // Step 5: Create the matrix
    struct matrix *m = NULL;
    char name[MATRIX_MAX_NAME_SIZE];
    snprintf(name, sizeof(name), "Matrix%u", Global_matrix_counter);

    enum ERRO_CODE err = Create_Matrix(&m, shape, data, number_of_dimensions, number_of_data_points, name);
    if (err == NO_ERROR) {
        if (Global_matrix_counter < MAXIMUM_MATRICES_ON_GLOBAL) {
            List_of_matrices[Global_matrix_counter] = m;
            Global_matrix_counter++;

            mvprintw(max_y - 2, 2, "Matrix '%s' created successfully! Press any key to return to main menu...", name);
            refresh();
            getch();
        } else {
            // If array is full
            mvprintw(max_y - 2, 2, "Global array full. Press any key to return to main menu...");
            refresh();
            getch();
            Destroy_Matrix(m); // free allocated matrix since we can't store it
        }
    } else {
        // Handle error code
        mvprintw(max_y - 2, 2, "Error creating matrix (code %d). Press any key to return...", (int)err);
        refresh();
        getch();
        // free memory if error
        free(shape);
        free(data);
        free(m);
    }

    return PAGE_MAIN_MENU;
}
