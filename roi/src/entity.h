#ifndef _entity_h
#define _entity_h

#include <uv.h>
#include <set>
#include "olist.h"

typedef struct Entity
{
	uv_stream_t *client;
	unsigned int id;
	char *repr;
	std::set<unsigned int> roi_entities;
} Entity;

typedef struct ClientEntity
{
	unsigned int id;
	double pos[COORD_NUM];
	uv_stream_t *server;
} ClientEntity;

#endif
