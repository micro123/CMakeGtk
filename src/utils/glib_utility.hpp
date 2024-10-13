//
// Created by tang on 24-10-13.
//

#ifndef GLIB_UTILITY_HPP
#define GLIB_UTILITY_HPP

#include <glibmm.h>

template <typename T>
using g_ptr = Glib::RefPtr<T>;

#endif //GLIB_UTILITY_HPP
