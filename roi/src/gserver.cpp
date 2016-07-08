#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <set>
#include <iterator>
#include <uv.h>
#include <olist.h>
#include <proto.h>
#include <entity.h>

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
void get_msg(uv_handle_t *handle);

void get_msg(uv_handle_t *handle)
{
	char *data = new char[1024];
	uv_stream_t **client = nullptr;
	unsigned int len = 0;
	get_one_cmd_from_cache_msg(data, len, client);
	if(len > 0 && client != nullptr)
	{
		dispatch_cmd(data, len, *client);
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

void move_entity(char *data, int nread, uv_stream_t *client)
{
	unsigned int entity_id = (unsigned int)data[1];
	int dx = (*(int*)&data[2]);
	int dy = (*(int*)&data[6]);
	
	if(map.count(entity_id) <= 0) return;
	OListNode *node = map[entity_id];
	OList_move(list, node, dx, dy);
	update_roi(client, entity_id);
}

void remove_entity(char *data, int nread, uv_stream_t *client)
{
	unsigned int entity_id = (unsigned int)data[1];
	if(map.count(entity_id) <= 0) return;
	OListNode *node = map[entity_id];
	std::set<OListNode*> roi;
	OList_roi(list, node, ROI_RANGE, ROI_RANGE, roi);
	OList_remove(list, node);
	map.erase(entity_id);
	
	for(auto it = roi.begin(); it != roi.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = entity_map[*it]->value->client;
		send_roi(CMD_SC_ROI_RM, other_client, *it, {entity_id})
	}
}

void update_roi(uv_stream_t *client, unsigned int entity_id)
{
	if(map.count(entity_id) <= 0) return;
	OListNode *node = map[entity_id];
	std::set<OListNode*> roi;
	OList_roi(list, node, ROI_RANGE, ROI_RANGE, roi);
	std::set<unsigned int> roi_ent;
	for(auto it = roi.begin(); it != roi.end(); it++)
	{
		roi_ent.insert(it->value->id);
	}

	std::set<unsigned int> roi_add;
	std::set<unsigned int> roi_rm;
	std::set<unsigned int> roi_mv;

	std::set_difference(roi_ent.begin(), roi.ent.end(), node->value->roi_entities.begin(), node->value->roi_entities.end(),
			std::inserter(roi_add, roi_add.begin()));
	
	std::set_difference(node->value->roi_entities.begin(), node->value->roi_entities.end(), roi_ent.begin(), roi_ent.end(),  
			std::inserter(roi_rm, roi_rm.begin()));
	
	std::set_difference(node->value->roi_entities.begin(), node->value->roi_entities.end(), roi_rm.begin(), roi_rm.end(),  
			std::inserter(roi_mv, roi_mv.begin()));

	send_roi(CMD_SC_ROI_ADD, client, entity_id, roi_add);
	send_roi(CMD_SC_ROI_RM, client, entity_id, roi_rm);
	
	for(auto it = roi_add.begin(); it != roi_add.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = entity_map[*it]->value->client;
		send_roi(CMD_SC_ROI_ADD, other_client, *it, {entity_id})
	}

	for(auto it = roi_rm.begin(); it != roi_rm.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = entity_map[*it]->value->client;
		send_roi(CMD_SC_ROI_RM, other_client, *it, {entity_id})
	}
	
	for(auto it = roi_mv.begin(); it != roi_mv.end(); it++)
	{
		if(entity_map.count(*it) <= 0) continue;
		uv_stream_t *other_client = entity_map[*it]->value->client;
		send_roi(CMD_SC_ROI_MV, other_client, *it, {entity_id})
	}
	
	node->value->roi_entities = roi;
}

void send_roi(unsigned char cmd, uv_stream_t* client, unsigned int entity_id, std::set<unsigned int> &roi)
{
	char buf[256];
	buf[0] = cmd;
	memcpy(&buf[1], &entity_id, 4);

	int num = roi.size();
	memcpy(&buf[5], &num, 4);
	
	size_t offset = 9;
	int roi_cnt = 0;
	for(auto it = roi.begin(); it != roi.end(); it++)
	{
		if(entity_map.cout(*it) <= 0) continue;
		OListNode *node = entity_map[*it];
		unsigned int entid = node->value->id;
		int x = node->pos[COORD_X];
		int y = node->pos[COORD_Y];
		memcpy(&buf[offset + roi_cnt * 12], &entid, 4);
		memcpy(&buf[offset + roi_cnt * 12 + 4], &x, 4);
		memcpy(&buf[offset + roi_cnt * 12 + 8], &y, 4);
		roi_cnt++;
		if(offset + roi_cnt * 12 > 256 - 12) break;
	}

	uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init(buf, offset + roi_cnt * 12);
	uv_write(req, client, &wrbuf, 1, echo_write);
	return;
}

void echo_write(uv_write_t *req, int status)
{
	if (status)
	{
		fprintf(stderr, "Write error %s.\n", uv_strerror(status));
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
		if(nread != EOF) std::cerr<<"Read error "<<uv_err_name(nread)<<std::endl;
		uv_close((uv_handle_t*)client, NULL);
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

	return uv_run(loop, UV_RUN_DEFAULT);
}
