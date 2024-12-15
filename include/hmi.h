#ifndef HMI_H
#define HMI_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses/ncurses.h>

/**
 * @brief Initialize the ncurses environment and any other HMI setup.
 */
void hmi_init();

/**
 * @brief End ncurses mode and restore the terminal.
 */
void hmi_end();

/**
 * @brief Check terminal size and ensure it's large enough for the UI.
 * @param min_lines Minimum required lines.
 * @param min_cols Minimum required columns.
 * @return 0 if OK, non-zero if too small.
 */
int hmi_check_terminal_size(int min_lines, int min_cols);

/**
 * @brief Draw the logo and description. Also print a subtitle line under the description.
 * @param desc A description text to print under the logo.
 * @param subtitle A subtitle to print centered below the description line.
 * @return The y-coordinate (line) below the logo and description area.
 */
int hmi_draw_logo_and_desc(const char *desc, const char *subtitle);

/**
 * @brief Draw a horizontal line using a specified character.
 */
void hmi_draw_horizontal_line(int y, int x_start, int x_end, chtype ch);

/**
 * @brief Draw a vertical line using a specified character.
 */
void hmi_draw_vertical_line(int x, int y_start, int y_end, chtype ch);

/**
 * @brief standart menu function
 */
int HighLight_menu(unsigned int num_of_options, int start_menu_y, int start_menu_x, const char **options);


/**
 * @brief Prompt the user for a single positive integer input (no leading zeros) until they press ENTER.
 * The user can press ESC to cancel. 
 * @param prompt The prompt message to display.
 * @param out_value Pointer to an unsigned int where the result is stored.
 * @return 1 if a number was read successfully, 0 if ESC was pressed or invalid input.
 */
int prompt_for_unsigned_int(const char *prompt, unsigned int *out_value);

/**
 * @brief Prompt the user to enter multiple double values in a single line separated by spaces.
 * The user can press ESC to cancel. Pressing ENTER parses the input.
 * @param prompt The prompt message to display.
 * @param count How many values to read.
 * @param out_array Pointer to a pre-allocated array of doubles where the values will be stored.
 * @return 1 if success, 0 if ESC or invalid input (e.g., not enough numbers or non-numeric input).
 */
int prompt_for_multiple_doubles(const char *prompt, unsigned int count, double *out_array);

#endif
