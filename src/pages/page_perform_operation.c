#include "pages/page_perform_operation.h"

extern unsigned int Global_matrix_counter;
extern struct matrix *List_of_matrices[MAXIMUM_MATRICES_ON_GLOBAL];

unsigned int num_options_operation = 3;
static const char *options_operation[] = {
    "0 - Back to menu",
    "1 - Add two matrices",
    "2 - Calculate determinant OF ANY 2D SQUARE MATRIX!!!",
};

enum INTERFACE_CODE menu_option_operation(int option_selected) {
    switch (option_selected) {
    case 0: return BACK_TO_MENU;
    case 1: return ADD_TWO_MATRICES;
    case 2: return CALCULATE_THE_DETERMINANT;
    default:
        return BACK_TO_MENU;
    }
}

/**
 * @brief Prompt the user to select a matrix from the global list via highlight menu.
 * @param prompt The message to display to the user.
 * @return The index of the selected matrix, or -1 if ESC or no matrices.
 */
static int prompt_for_matrix_selection(const char *prompt) {
    clear();
    mvprintw(0,0,"%s", prompt);

    if (Global_matrix_counter == 0) {
        mvprintw(2,0,"No matrices available. Press any key to return...");
        refresh();
        getch();
        return -1;
    }

    // Build an array of strings representing each matrix
    const char **matrix_options = (const char **)malloc(sizeof(char*) * Global_matrix_counter);
    if (!matrix_options) {
        mvprintw(2,0,"Memory allocation error. Press any key...");
        refresh();
        getch();
        return -1;
    }

    for (unsigned int i = 0; i < Global_matrix_counter; i++) {
        struct matrix *m = List_of_matrices[i];
        if (!m) {
            // Just a placeholder if somehow the slot is empty
            char *placeholder = malloc(32);
            snprintf(placeholder, 32, "%u) <NULL>", i);
            matrix_options[i] = placeholder;
            continue;
        }
        // Build shape string
        char shape_str[256] = {0};
        if (m->number_of_dimensions > 0) {
            char temp[32];
            snprintf(temp, sizeof(temp), "%u", m->shape[0]);
            strcat(shape_str, temp);
            for (unsigned int d = 1; d < m->number_of_dimensions; d++) {
                snprintf(temp, sizeof(temp), "x%u", m->shape[d]);
                strcat(shape_str, temp);
            }
        }

        const char *m_name = (m->name) ? m->name : "Unnamed";

        // Build data snippet
        // Show up to first 5 data points
        unsigned int points_to_show = (m->number_of_data_points > 5) ? 5 : m->number_of_data_points;

        char data_snippet[256] = {0};
        strcat(data_snippet, "[");
        for (unsigned int p = 0; p < points_to_show; p++) {
            char val_str[32];
            snprintf(val_str, sizeof(val_str), p == points_to_show-1 ? "%.2f" : "%.2f ", m->data[p]);
            strcat(data_snippet, val_str);
        }
        if (m->number_of_data_points > 5) {
            strcat(data_snippet, "..."); // indicate more data
        }
        strcat(data_snippet, "]");

        // Construct the full option string
        // Format: "Index) Name (DxD...): [data snippet]"
        char *option_str = malloc(512);
        if (!option_str) {
            // memory error
            for (unsigned int k = 0; k < i; k++) {
                free((void*)matrix_options[k]);
            }
            free(matrix_options);
            mvprintw(2,0,"Memory allocation error. Press any key...");
            refresh();
            getch();
            return -1;
        }

        snprintf(option_str, 512, "%u) %s (%uD: %s) %s", 
                 i, m_name, m->number_of_dimensions, shape_str, data_snippet);
        matrix_options[i] = option_str;
    }

    mvprintw(Global_matrix_counter + 3, 0, "Use Up/Down to navigate, Enter to select, ESC to cancel.");
    refresh();

    int start_y = 2; 
    int start_x = 0;
    int selected = HighLight_menu(Global_matrix_counter, start_y, start_x, matrix_options);

    // Clean up allocated strings
    for (unsigned int i = 0; i < Global_matrix_counter; i++) {
        free((void*)matrix_options[i]);
    }
    free(matrix_options);

    if (selected == -1) {
        // ESC pressed or canceled
        return -1;
    }

    return selected; // return the chosen index
}

AppPage run_perform_operation_page() {
    const char *DESC = "Select an operation to perform on the matrices.";
    int logo_bottom = hmi_draw_logo_and_desc(DESC, "Perform Operations");

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    hmi_draw_horizontal_line(logo_bottom + 14, 2, max_x - 2, ACS_HLINE);
    mvprintw(logo_bottom + 16, 2, "Use Up/Down to navigate, Enter to select");

    int start_y = logo_bottom + 6;
    int start_x = 4;

    int selected = HighLight_menu(num_options_operation, start_y, start_x, options_operation);
    if (selected == -1) {
        // ESC pressed
        return PAGE_MAIN_MENU;
    }

    enum INTERFACE_CODE op = menu_option_operation(selected);
    if (op == BACK_TO_MENU) {
        return PAGE_MAIN_MENU;
    }

    if (op == ADD_TWO_MATRICES) {
        int idx1 = prompt_for_matrix_selection("Select the first matrix for addition:");
        if (idx1 < 0) return PAGE_MAIN_MENU;
        int idx2 = prompt_for_matrix_selection("Select the second matrix for addition:");
        if (idx2 < 0) return PAGE_MAIN_MENU;

        struct matrix *m1 = List_of_matrices[idx1];
        struct matrix *m2 = List_of_matrices[idx2];

        struct matrix *result = NULL;
        enum ERRO_CODE err = Matrix_Add(m1, m2, &result);

        clear();
        if (err == NO_ERROR && result) {
            if (Global_matrix_counter < MAXIMUM_MATRICES_ON_GLOBAL) {
                free(result->name);
                result->name = malloc(MATRIX_MAX_NAME_SIZE);
                if(result->name)
                    snprintf(result->name, MATRIX_MAX_NAME_SIZE, "AddResult%u", Global_matrix_counter);

                List_of_matrices[Global_matrix_counter] = result;
                Global_matrix_counter++;

                mvprintw(0,0,"Addition success! Result stored as '%s'", result->name);

                log_message("Added matrices (index %d and %d) -> New matrix '%s' created with dimensions %uD",
                    idx1, idx2, result->name, result->number_of_dimensions);

                // Display the new matrix's information
                // Print dimensions and shape
                mvprintw(2,0,"Dimensions: %uD", result->number_of_dimensions);
                mvprintw(3,0,"Shape: ");
                {
                    char shape_str[256] = {0};
                    if (result->number_of_dimensions > 0) {
                        char temp[32];
                        snprintf(temp, sizeof(temp), "%u", result->shape[0]);
                        strcat(shape_str, temp);
                        for (unsigned int d = 1; d < result->number_of_dimensions; d++) {
                            snprintf(temp, sizeof(temp), "x%u", result->shape[d]);
                            strcat(shape_str, temp);
                        }
                    }
                    mvprintw(3,7,"%s", shape_str);
                }

                // Print the entire data array linearly
                mvprintw(5,0,"Data:");
                unsigned int line = 6; // start printing data from line 6
                unsigned int per_line = 10; // how many data points per line to keep it readable
                unsigned int count = 0;
                for (unsigned int i = 0; i < result->number_of_data_points; i++) {
                    // Print 10 elements per line
                    if (count == 0) {
                        move(line,0);
                        clrtoeol();
                    }
                    printw("%.2f ", result->data[i]);
                    count++;
                    if (count == per_line) {
                        line++;
                        count = 0;
                    }
                }

                // Log the data as well
                {
                    // We'll create a buffer to hold all data in one line
                    // For simplicity, limit to 1000 chars max
                    char log_buffer[1001];
                    size_t pos = 0;
                    pos += snprintf(log_buffer+pos, sizeof(log_buffer)-pos, "Result Matrix Data: ");

                    // Add each data point, stop if we reach buffer capacity
                    for (unsigned int i = 0; i < result->number_of_data_points && pos < sizeof(log_buffer)-1; i++) {
                        int written = snprintf(log_buffer+pos, sizeof(log_buffer)-pos, "%.2f ", result->data[i]);
                        if (written < 0 || (size_t)written >= sizeof(log_buffer)-pos) {
                            // No more space in buffer
                            break;
                        }
                        pos += (size_t)written;
                    }

                    // If we ran out of space, indicate truncation
                    if (pos >= sizeof(log_buffer)-5) {
                        strcpy(log_buffer+pos-4, "...");
                    }

                    log_message("%s", log_buffer);
                }

            } else {
                mvprintw(0,0,"Global array full. Cannot store result.");
                Destroy_Matrix(result);
            }
        } else {
            mvprintw(0,0,"Addition failed. Error code: %d", (int)err);
        }
        mvprintw(2,0,"Press any key to return to main menu...");
        refresh();
        getch();
        return PAGE_MAIN_MENU;
    } else if (op == CALCULATE_THE_DETERMINANT) {
        int idx = prompt_for_matrix_selection("Select matrix for determinant calculation:");
        if (idx < 0) return PAGE_MAIN_MENU;

        struct matrix *m = List_of_matrices[idx];
        double val=0.0;
        enum ERRO_CODE err = Matrix_Determinant(m,&val);
        clear();
        if (err == NO_ERROR) {
            mvprintw(0,0,"Determinant: %.4f", val);
        } else {
            mvprintw(0,0,"Determinant failed. Error code: %d", (int)err);
            log_message("Determinant failed. Error code: %d", (int)err);
        }
        mvprintw(2,0,"Press any key to return to main menu...");

        log_message("determinant calculated. resulting value = %f", val);
        refresh();
        getch();
        return PAGE_MAIN_MENU;
    }

    // Should not reach here
    return PAGE_MAIN_MENU;
}
