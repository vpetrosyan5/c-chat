#include "util.h"

#include <stdlib.h>
#include <string.h>

void msgarr_init(msgarr_t *arr) {
	arr->mem = NULL;
	arr->size = 0;
	arr->cap = 1;
}

void msgarr_append(msgarr_t *arr, const msg_pak_t *val) {
	if (arr->size >= arr->cap)
		arr->mem = realloc(arr->mem, arr->cap * 2);		
	memcpy(arr->mem[arr->size++], val, sizeof(msg_pak_t));
}

void msgarr_free(msgarr_t *arr) { free(arr->mem); }
