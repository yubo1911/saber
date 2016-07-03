#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <uv.h>
#include <olist.h>
#include <proto.h>

#define DEFAULT_PORT 7000
#define BACKLOG 128
uv_loop_t *loop;
struct sockaddr_in addr;
static OList *list = NULL;
static std::map<unsigned int, OListNode *> entity_map;

using std::cerr, std::endl;

void insert_new_entity(char *data, int nread);
void move_entity(char *data, int nread);
void remove_entity(char *data, int nread);

void dispatch_cmd(char *data, ssize_t nread)
{
	switch(data[0])
	{
		case CMD_NEW:
			insert_new_entity(data, nread);
			break;
		case CMD_MV:
			move_entity(data, nread);
			break;
		case CMD_GONE:
			remove_entity(data, nread);
			break;
		default:
			break;
	}
}

void insert_new_entity(char *data, int nread)
{
	unsigned int entity_id = (unsigned int)data[1];
	char buf[256];
	sprintf(buf, "entity with id %d", entity_id);
	int x = (*(int*)&data[2]);
	int y = (*(int*)&data[6]);
	
	OListNode *node = (OListNode*)malloc(sizeof(OListNode));
	node->pos[COORD_X] = x;
	node->pos[COORD_Y] = y;
	node->value = strdup(buf);
	node->next[COORD_X] = node->prev[COORD_X] = node->next[COORD_Y] = node->prev[COORD_Y] = NULL;
	OList_insert(list, node);
	entity_map[entity_id] = node;
}

void move_entity(char *data, int nread)
{
	unsigned int entity_id = (unsigned int)data[1];
	int dx = (*(int*)&data[2]);
	int dy = (*(int*)&data[6]);
	
	if(map.count(entity_id) <= 0) return;
	OListNode *node = map[entity_id];
	OList_move(list, node, dx, dy);
}

void remove_entity(char *data, int nread)
{
	unsigned int entity_id = (unsigned int)data[1];
	if(map.count(entity_id) <= 0) return;
	OListNode *node = map[entity_id];
	OList_remove(list, node);
	map.erase(entity_id);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void read_cmd(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	if(nread < 0)
	{
		if(nread != EOF) std::cerr<<"Read error "<<uv_err_name(nread)<<std::endl;
		uv_close((uv_handle_t*)client, NULL);
	}
	else if(nread > 0)
	{
		dispatch_cmd(buf->base, nread);
	}

	if(buf->base) free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status)
{
	if(status < 0)
	{
		std::cerr<<"On new connection error "<<uv_strerror(status)<<std::endl;
		return;
	}
	std::cout<<"new connection is coming...\n";
	uv_tcp_t &client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if(uv_accept(server, (uv_stream_t*)client) == 0)
	{
		uv_read_start((uv_stream_t*)client, alloc_buffer, read_cmd);
	}
	else
	{
		uv_close((uv_handle_t*)client, NULL);
	}
}

int main()
{
	loop = uv_default_loop();
	list = OList_create();

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, BACKLOG, on_new_connection);
	if(r)
	{
		std::cerr<<"Listen error "<<uv_strerror(r)<<std::endl;
	}

	return uv_run(loop, UV_RUN_DEFAULT);
}
