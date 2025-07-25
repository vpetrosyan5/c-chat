#ifndef _SHARED_H_
#define _SHARED_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

typedef struct {
	uint8_t sender[10];
	uint8_t msg[512];
	time_t timestamp;
} msg_pak_t;

#endif // _SHARED_H_
