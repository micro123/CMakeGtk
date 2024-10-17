#include "glib_utility.hpp"

void log_err_and_free(const char *domain, GError *err)
{
    if (err) {
        g_log(domain, G_LOG_LEVEL_WARNING, "error code: %d, message: %s", err->code, err->message);
        g_error_free(err);
    }
}
