#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <cstring>
#include <iterator>
extern "C"
{
#include <uv.h>
}
#include "olist.h"
#include "proto.h"
#include "entity.h"

#define DEFAULT_PORT 7000
#define BACKLOG 128
#define ROI_RANGE 20
uv_loop_t *loop;
struct sockaddr_in addr;
static OList *list = NULL;
static std::map<unsigned int, OListNode *> entity_map;
static int max_entity_id = 0;

void insert_new_entity(char *data, int nread, uv_stream_t *client);
void move_entity(char *data, int nread, uv_stream_t *client);
void remove_entity(char *data, int nread, uv_stream_t *client);
void update_roi(uv_stream_t *client, unsigned int entity_id);
void echo_write(uv_write_t *req, int status);
void get_msg(uv_idle_t *handle);
void dispatch_cmd(char *data, ssize_t nread, uv_stream_t *client);
void ack_new_entity(uv_stream_t *client, unsigned int entity_id, int x, int y);
void send_roi(unsigned char cmd, uv_stream_t* client, unsigned int entity_id, std::set<unsigned int> &roi);
void remove_entity_helper(unsigned int entity_id);
char msg_data[1024] = {0};

void get_msg(uv_idle_t *handle)
{
	memset(msg_data, 0, 1024);
	uv_stream_t *client = nullptr;
	unsigned int len = 0;
	get_one_cmd_from_cache_msg(msg_data, len, &client);
	if(len > 0 && client != nullptr)
	{
		dispatch_cmd(msg_data, len, client);
	}
}

void dispatch_cmd(char *data, ssize_t nread, uv_stream_t *client)
{
	switch(data[0])
	{
		case CMD_NEW:
			insert_new_entity(data, nread, client);
			break;
		case CMD_MV:
			move_entity(data, nread, client);
			break;
		case CMD_GONE:
			remove_entity(data, nread, client);
			break;
		default:
			break;
	}
}

void insert_new_entity(char *data, int nread, uv_stream_t *client)
{
	unsigned int entity_id = max_entity_id;
	char buf[256];
	sprintf(buf, "entity with id %d", entity_id);
	int x = (*(int*)&data[1]);
	int y = (*(int*)&data[5]);
	Entity *ent = (Entity*)malloc(sizeof(Entity));
	ent->client = client;
	ent->repr = strdup(buf);
	ent->id = entity_id;
	ent->roi_entities = new std::set<unsigned int>();

	ack_new_entity(client, entity_id, x, y);
	
	OListNode *node = (OListNode*)malloc(sizeof(OListNode));
	node->pos[COORD_X] = x;
	node->pos[COORD_Y] = y;
	node->value = (void *)ent;
	node->next[COORD_X] = node->prev[COORD_X] = node->next[COORD_Y] = node->prev[COORD_Y] = NULL;
	OList_insert(list, node);
	entity_map[entity_id] = node;
	update_roi(client, entity_id);
	max_entity_id++;
}

void ack_new_entity(uv_stream_t *client, unsigned int entity_id, int x, int y)
{
	size_t int_size = sizeof(int);
	char buf[256] = {0};
	buf[0] = PROTO_START;
	buf[1 + int_size] = CMD_SC_NEW;
	memcpy(&buf[2 + int_size * 1], &entity_id, int_size);
	memcpy(&buf[2 + int_size * 2], &x, int_size);
	memcpy(&buf[2 + int_size * 3], &y, int_size);
	int len = 2 + int_size * 4;
	memcpy(&buf[1], &len, int_size);
	
	uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init(buf, len);
	uv_write(req, client, &wrbuf, 1, echo_write);
}

void move_entity(char *data, int nread, uv_stream_t *client)
{
	size_t int_size = sizeof(int);
	unsigned int entity_id = (unsigned int)data[1];
	int dx = (*(int*)&data[1 + int_size]);
	int dy = (*(int*)&data[1 + int_size * 2]);
	
	if(entity_map.count(entity_id) <= 0) return;
	OListNode *node = entity_map[entity_id];
	OList_move(list, node, dx, dy);
	update_roi(client, entity_id);
}

void remove_entity(char *data, int nread, uv_stream_t *client)
{
	unsigned int entity_id = (unsigned int)data[1];
	remove_entity_helper(entity_id);
}

void remove_entity_helper(unsigned int entity_id)
{
	if(entity_map.count(entity_id) <= 0) return;
	OListNode *node = entity_map[entity_id];
	std::set<OListNode*> roi;
	OList_roi(list, node, ROI_RANGE, ROI_RANGE, roi);
	OList_remove(list, node);
	
	std::set<unsigned int> entity_id_set{entity_id};
	for(auto it = roi.begin(); it != roi.end(); it++)
	{
		OListNode *node = *it;
		unsigned int roi_ent_id = ((Entity *)node->value)->id;
		if(entity_map.count(roi_ent_id) <= 0) continue;
		uv_stream_t *other_client = ((Entity *)entity_map[roi_ent_id]->value)->client;
		send_roi(CMD_SC_ROI_RM, other_client, roi_ent_id, entity_id_set);
	}
	entity_map.erase(entity_id);
}

void update_roi(uv_stream_t *client, unsigned int entity_id)
{
	if(entity_map.count(entity_id) <= 0) return;
	OListNode *avatar_node = entity_map[entity_id];
	std::set<OListNode*> roi;
	OList_roi(list, avatar_node, ROI_RANGE, ROI_RANGE, roi);
	auto tmp = roi.begin();
	std::set<unsigned int> roi_ent;
	for(auto it = roi.begin(); it != roi.end(); it++)
	{
		OListNode *node = *it;
		unsigned int id = ((Entity *)(node->value))->id;
		roi_ent.insert(id);
	}
	std::set<unsigned int> roi_add;
	std::set<unsigned int> roi_rm;
	std::set<unsigned int> roi_mv;
	auto old_roi = ((Entity*)avatar_node->value)->roi_entities;
	
	std::set_difference(roi_ent.begin(), roi_ent.end(), old_roi->begin(), old_roi->end(),
			std::inserter(roi_add, roi_add.begin()));
	
	std::set_difference(old_roi->begin(), old_roi->end(), roi_ent.begin(), roi_ent.end(),  
			std::inserter(roi_rm, roi_rm.begin()));
	
	std::set_difference(old_roi->begin(), old_roi->end(), roi_rm.begin(), roi_rm.end(),  
			std::inserter(roi_mv, roi_mv.begin()));

	send_roi(CMD_SC_ROI_ADD, client, entity_id, roi_add);
	if(roi_rm.size() > 0) send_roi(CMD_SC_ROI_RM, client, entity_id, roi_rm);
	
	std::set<unsigned int> entity_id_set{entity_id};
	for(auto it = roi_add.begin(); it != roi_add.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = ((Entity *)entity_map[*it]->value)->client;
		send_roi(CMD_SC_ROI_ADD, other_client, *it, entity_id_set);
	}

	for(auto it = roi_rm.begin(); it != roi_rm.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = ((Entity *)entity_map[*it]->value)->client;
		send_roi(CMD_SC_ROI_RM, other_client, *it, entity_id_set);
	}
	
	for(auto it = roi_mv.begin(); it != roi_mv.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = ((Entity *)entity_map[*it]->value)->client;
		send_roi(CMD_SC_ROI_MV, other_client, *it, entity_id_set);
	}
	
	old_roi->clear();
	std::copy(roi_ent.begin(), roi_ent.end(), std::inserter(*old_roi, old_roi->begin()));
}

void send_roi(unsigned char cmd, uv_stream_t* client, unsigned int entity_id, std::set<unsigned int> &roi)
{
	size_t int_size = sizeof(int);
	char buf[256];
	buf[0] = PROTO_START;
	buf[1 + int_size] = cmd;
	memcpy(&buf[1 + int_size + 1], &entity_id, int_size);

	int num = roi.size();
	memcpy(&buf[1 + int_size + 1 + int_size], &num, int_size);
	
	size_t offset = 1 + int_size + 1 + int_size * 2;
	int roi_cnt = 0;
	if(cmd == CMD_SC_ROI_RM)
	{
		std::cout<<"rm roi size:"<<roi.size()<<std::endl;
	}
	for(auto it = roi.begin(); it != roi.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		OListNode *node = entity_map[*it];
		unsigned int entid = ((Entity *)node->value)->id;
		int x = node->pos[COORD_X];
		int y = node->pos[COORD_Y];
		if(cmd == CMD_SC_ROI_RM)
		{
			std::cout<<"send roi..."<<(short)cmd<<entid<<std::endl;
		}
		memcpy(&buf[offset + roi_cnt * 3 * int_size], &entid, int_size);
		memcpy(&buf[offset + roi_cnt * 3 * int_size + int_size], &x, int_size);
		memcpy(&buf[offset + roi_cnt * 3 * int_size + int_size * 2], &y, int_size);
		roi_cnt++;
		if(offset + roi_cnt * 3 * int_size > 256 - 3 * int_size) break;
	}
	int len = offset + roi_cnt * 3 * int_size;
	memcpy(&buf[1], &len, int_size);

	uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init(buf, len);
	uv_write(req, client, &wrbuf, 1, echo_write);
	return;
}

void handle_client_close(uv_stream_t *client)
{
	unsigned int entity_id = -1;
	for(auto it = entity_map.begin(); it != entity_map.end(); it++)
	{
		OListNode *node = it->second;
		if(((Entity*)node->value)->client == client)
		{
			entity_id = it->first;
			std::cout<<"remove entity "<<entity_id<<std::endl;
			break;
		}
	}
	if(entity_id >= 0)
	{
		remove_entity_helper(entity_id);
	}

}

void echo_write(uv_write_t *req, int status)
{
	if (status)
	{
		std::cerr<<"Write error "<<uv_strerror(status)<<std::endl;
	}
	free(req);
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
		if(nread != UV_EOF) std::cerr<<"Read error "<<uv_err_name(nread)<<std::endl;
		std::cout<<"Bye client"<<std::endl;
		uv_close((uv_handle_t*)client, NULL);
		handle_client_close(client);
	}
	else if(nread > 0)
	{
		push_data_to_cache_msg(buf->base, nread, client);
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
	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
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

	uv_idle_t idler;
	uv_idle_init(loop, &idler);
	uv_idle_start(&idler, get_msg);

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, BACKLOG, on_new_connection);
	if(r)
	{
		std::cerr<<"Listen error "<<uv_strerror(r)<<std::endl;
	}
	std::cout<<"Running server..."<<std::endl;
	return uv_run(loop, UV_RUN_DEFAULT);
}
