//
// Created by tang on 24-10-13.
//

#ifndef GLIB_UTILITY_HPP
#define GLIB_UTILITY_HPP

#include <glibmm.h>

template <typename T>
using g_ptr = Glib::RefPtr<T>;

void log_err_and_free(const char *domain, GError *err);

#define ERR_LOG(err) \
log_err_and_free(G_LOG_DOMAIN, err)

#endif //GLIB_UTILITY_HPP
