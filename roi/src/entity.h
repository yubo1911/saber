#ifndef _entity_h
#define _entity_h

#include <uv.h>

typedef struct Entity
{
	uv_stream_t *client;
	char *repr;
} Entity;

#endif
