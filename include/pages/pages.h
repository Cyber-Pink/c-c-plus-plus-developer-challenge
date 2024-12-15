#ifndef PAGES_H
#define PAGES_H

// Application pages (screens)
typedef enum {
    PAGE_MAIN_MENU,
    PAGE_SELECT_MATRIX_SIZE,
    PAGE_CUSTOM_DIMENSIONS,
    PAGE_PERFORM_OPERATION,
    PAGE_EXIT
} AppPage;

/**
 * @brief Run the main application loop that handles transitions between pages.
 */
void run_hmi_loop();

#endif
