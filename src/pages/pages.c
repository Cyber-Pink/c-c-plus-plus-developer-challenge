#include "pages/pages.h"
#include "pages/page_main_menu.h"
#include "pages/page_select_matrix_size.h"
#include "pages/page_custom_dimensions.h"
#include "pages/page_perform_operation.h"
#include "hmi.h"


/**
 * Here is where the new pages are added.
 */
void run_hmi_loop() {
    AppPage current_page = PAGE_MAIN_MENU;

    while (current_page != PAGE_EXIT) {
        switch (current_page) {
            case PAGE_MAIN_MENU:
                current_page = run_main_menu();
                break;
            case PAGE_SELECT_MATRIX_SIZE:
                current_page = run_select_matrix_size_page();
                break;
            case PAGE_CUSTOM_DIMENSIONS:
                current_page = run_custom_dimensions_page();
                break;
            case PAGE_PERFORM_OPERATION:
                current_page = run_perform_operation_page();
                break;
            default:
                current_page = PAGE_EXIT;
                break;
        }
    }
}
