#ifndef _UTIL_H_
#define _UTIL_H_

#include "shared.h"

typedef struct {
	msg_pak_t *mem;
	size_t size;
	size_t cap;
} msgarr_t;

void msgarr_init(msgarr_t *arr);

#endif // _UTIL_H_
