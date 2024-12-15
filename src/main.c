#include "matrix.h"
#include "hmi.h"
#include "log.h"
#include "pages/pages.h"


unsigned int Global_matrix_counter = 0;
struct matrix *List_of_matrices[MAXIMUM_MATRICES_ON_GLOBAL]; // for now the code will hold 100 matrices max


int main() {

    // try to open the log
    if (init_log() != 0) {
            fprintf(stderr, "Failed to open log file\n");
            return 1;
        }

    hmi_init();
    run_hmi_loop();
    hmi_end();
    return 0;
}
