#ifndef __LIBC_STDDEF_H
#define __LIBC_STDDEF_H

#define NULL ((void *)0)

#define offsetof(st, m) \
    ((size_t) ((char*) & ((st*) 0) -> m - (char*) 0))

#define container_of(ptr, type, member) \
    ((type*) ((char*) (ptr) - offsetof(type, member)))

#define sizeof(type) \
    (char*) (&type + 1) - (char*) (&type) 

typedef int             wchar_t;
typedef unsigned int    wint_t;

#endif
