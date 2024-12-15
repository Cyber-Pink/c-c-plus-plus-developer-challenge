#include "hmi.h"

void hmi_init() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    refresh();
}

void hmi_end() {
    endwin();
}


void hmi_draw_horizontal_line(int y, int x_start, int x_end, chtype ch) {
    for (int i = x_start; i <= x_end; i++) {
        mvaddch(y, i, ch);
    }
}

void hmi_draw_vertical_line(int x, int y_start, int y_end, chtype ch) {
    for (int i = y_start; i <= y_end; i++) {
        mvaddch(i, x, ch);
    }
}

int hmi_draw_logo_and_desc(const char *desc, const char *subtitle) {
    /* 
       Prints the LOGO and the description.
       Returns the line number after printing logo and desc.
    */

    clear(); //as always clean the previus terminal
    //Takes the width and the heigth of the terminal and store in the variables max_y, and max_x
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);


    // Adds the lofo to the inerface and the description to the interface. (i used a online generator to create the ascii art)
    // And the distances were calculated by hand using trial and error :)
    unsigned int logo_bottom = 6;
    unsigned int lateral_desc_start = 2;
    printw("___  ___      _        _    _____           _ _               \n|  \\/  |     | |      (_)  |_   _|         | | |              \n| .  . | __ _| |_ _ __ ___  _| | ___   ___ | | |__   _____  __\n| |\\/| |/ _` | __| '__| \\ \\/ / |/ _ \\ / _ \\| | '_ \\ / _ \\ \\/ /\n| |  | | (_| | |_| |  | |>  <| | (_) | (_) | | |_) | (_) >  < \n\\_|  |_/\\__,_|\\__|_|  |_/_/\\_\\_/\\___/ \\___/|_|_.__/ \\___/_/\\_\\\n");
    mvprintw(logo_bottom, lateral_desc_start, "%s", desc);
    // Draw a line below the logo and the description of the page
    hmi_draw_horizontal_line(logo_bottom + 3, 2, max_x - 2, ACS_HLINE);
    
    
    //This prints the name of the page the user is currently in.
    mvprintw(logo_bottom + 2, (max_x / 2) - ((int)strlen(subtitle)/2), "%s", subtitle);

    refresh();
    // the logo botton is returned, this way every functions that drawns the logo again knows where the logo is, and where it should start drawing.
    return logo_bottom;
}


int HighLight_menu(unsigned int num_of_options, int start_menu_y, int start_menu_x, const char **options){

    refresh(); // updates changes on screen
    int highlight = 0; // highlight tracks what is the current selected option in the menu.
    int c; // c is a placeholder for the key pressed by the user.
    while (1) {

        // This is how you just draw the options on the screen without highlight
        /*for (int i = 0; i < num_options; i++) {
        *    mvprintw(start_y + i, start_x, "%s", options[i]);
        *}
        *refresh(); */

        // Draw options with highlight
        for (int i = 0; i < num_of_options; i++) {
            if (i == highlight) {
                // Highlight the selected option
                attron(A_REVERSE);
                mvprintw(start_menu_y + i, start_menu_x, "%s", options[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(start_menu_y + i, start_menu_x, "%s", options[i]);
            }
        }
        refresh(); // updates the screen

        // Handle user input for menu navigation
        c = getch();
        switch (c) {
            case KEY_UP:
                highlight = (highlight == 0) ? num_of_options - 1 : highlight - 1; // Wrap the list around
                break;
            case KEY_DOWN:
                highlight = (highlight == num_of_options - 1) ? 0 : highlight + 1; // Wrap the list around
                break;
            case 10: // Enter
                // Return next page based on selection
                return highlight;
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Internal helper to validate a positive integer input (no leading zeros).
 */
static int validate_positive_int(const char *input) {
    if (strlen(input) == 0) return 0;
    if (input[0] == '0') return 0;
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isdigit((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

int prompt_for_unsigned_int(const char *prompt, unsigned int *out_value) {
    // Clear screen and show prompt
    clear();
    mvprintw(0,0,"%s", prompt);
    mvprintw(2,0,"Press ESC to cancel or ENTER to confirm after typing the number.");
    mvprintw(4,0,"Number: ");
    refresh();

    char buffer[32];
    memset(buffer,0,sizeof(buffer));
    int pos = 0;

    int c;
    int y=4, x=8;
    keypad(stdscr, TRUE);

    while (1) {
        move(y,x);
        clrtoeol();
        mvprintw(y,x,"%s", buffer);
        refresh();
        c = getch();
        if (c == 27) {
            // ESC -> go back
            return 0;
        } else if (c == 10) {
            // ENTER
            if (validate_positive_int(buffer)) {
                unsigned long long temp = strtoull(buffer, NULL, 10);
                if (temp > UINT_MAX) {
                    mvprintw(6,0,"Number too large. Press any key...");
                    refresh();
                    getch();
                    memset(buffer,0,sizeof(buffer));
                    pos=0;
                    continue;
                }
                *out_value = (unsigned int)temp;
                return 1;
            } else {
                mvprintw(6,0,"Invalid input. Only positive integers allowed. Press any key...");
                refresh();
                getch();
                memset(buffer,0,sizeof(buffer));
                pos=0;
            }
        } else if (c == KEY_BACKSPACE || c == 127 || c == '\b') {
            if (pos > 0) {
                pos--;
                buffer[pos]='\0';
            }
        } else if (isprint(c) && pos < (int)(sizeof(buffer)-1)) {
            if (isdigit((unsigned char)c)) {
                buffer[pos++] = (char)c;
                buffer[pos]='\0';
            }
        }
    }
    
    // it's not supposed to get here
    return 0;
}

int prompt_for_multiple_doubles(const char *prompt, unsigned int count, double *out_array) {
    clear();
    mvprintw(0,0,"%s", prompt);
    mvprintw(2,0,"Enter %u values separated by , Press ESC to cancel or ENTER to confirm.", count);
    refresh();

    // Enable keypad for ESC detection:
    keypad(stdscr, TRUE);

    // A line buffer
    char line[1024];
    memset(line,0,sizeof(line));

    // Build input line until ENTER or ESC
    int ch;
    int idx=0;
    move(4,0);
    refresh();
    nodelay(stdscr, FALSE);
    while ((ch = getch()) != 10) { // ENTER
        if (ch == 27) {
            // ESC
            return 0;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (idx > 0) {
                idx--;
                line[idx]='\0';
                move(4,0);
                clrtoeol();
                mvprintw(4,0,"%s", line);
                refresh();
            }
        } else if (isprint(ch) && idx < (int)(sizeof(line)-1)) {
            line[idx++] = (char)ch;
            line[idx]='\0';
            move(4,0);
            clrtoeol();
            mvprintw(4,0,"%s", line);
            refresh();
        }
    }

    // Now parse the line
    unsigned int parsed_count=0;
    char *token = strtok(line, ",");
    while (token && parsed_count < count) {
        char *endptr;
        double val = strtod(token, &endptr);
        if (*endptr != '\0') {
            // Not a valid number
            mvprintw(6,0,"Invalid input detected. Press any key...");
            refresh();
            getch();
            return 0;
        }
        out_array[parsed_count++] = val;
        token = strtok(NULL, ",");
    }

    if (parsed_count != count) {
        mvprintw(6,0,"Incorrect number of values. Press any key...");
        refresh();
        getch();
        return 0;
    }

    return 1;
}
