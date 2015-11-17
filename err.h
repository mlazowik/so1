#ifndef ERR_H
#define ERR_H

#include <stddef.h>

/* wypisuje informacje o blednym zakonczeniu funkcji systemowej
i konczy dzialanie */
extern void syserr(const char *fmt, ...);

/* wypisuje informacje o bledzie i konczy dzialanie */
extern void fatal(const char *fmt, ...);

/* malloc z obsluga bledow */
extern void* emalloc(size_t size);

#endif
