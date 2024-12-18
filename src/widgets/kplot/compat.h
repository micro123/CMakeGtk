#ifndef COMPAT_H
#define COMPAT_H

#include <sys/types.h>

#if !defined(__BEGIN_DECLS)
#  ifdef __cplusplus
#  define       __BEGIN_DECLS           extern "C" {
#  else
#  define       __BEGIN_DECLS
#  endif
#endif
#if !defined(__END_DECLS)
#  ifdef __cplusplus
#  define       __END_DECLS             }
#  else
#  define       __END_DECLS
#  endif
#endif

__BEGIN_DECLS

void *reallocarray(void *optr, size_t nmemb, size_t size);

__END_DECLS

#endif