#include "error.h"

static char str_error[] = "[libimage]: %s\n";
static char *str_error_msg[] = {(char *)"No error", (char *)"Can't open file", (char *)"Unsupported file format"};

static char *str_status_msg[] = {(char *)"STATUS_FAILED", (char *)"STATUS_SUCCESS", (char *)"STATUS_IMG_INCORRECT"};

char *img_status_msg(uint8_t status) { return str_status_msg[status]; }

void img_send_error(const int error) { printf(str_error, str_error_msg[error]); }

void img_module_error(const char *format, ...) {
    va_list list;

    if (format != NULL) {
        va_start(list, format);
        vprintf(format, list);
        va_end(list);
    }
}