#include <stdlib.h>
#include <iostream>
#include <uv.h>
#include <cstring>
#include <entity.h>
#include <proto.h>
#include <map>

#define DEFAULT_PORT 7000

uv_loop_t *loop = NULL;
ClientEntity *avatar = NULL;
std::map<unsigned int, ClientEntity*> roi_entities;

void on_write(uv_write_t *req, int status);
void get_msg(uv_idle_t *handle);
void dispatch_cmd(char *data, ssize_t nread, uv_stream_t *stream);
void handle_add_entity(char *data, ssize_t nread, uv_stream_t *stream);
void handle_add_roi_entity(char *data, ssize_t nread, uv_stream_t *stream);
void handle_mv_roi_entity(char *data, ssize_t nread, uv_stream_t *stream);
void handle_rm_roi_entity(char *data, ssize_t nread, uv_stream_t *stream);
void alloc_buf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
void my_write_helper(uv_stream_t *stream);
void add_entity_helper(uv_stream_t *stream, int x, int y);
void move_entity_helper(uv_stream_t *stream, int dx, int dy);
void remove_entity_helper(uv_stream_t *stream);
void print_entity_roi_helper();
void on_connect(uv_connect_t *req, int status);

void on_write(uv_write_t *req, int status)
{
	if(status)
	{
		std::cerr<<"Write error: "<<uv_strerror(status)<<std::endl;
	}
	std::cout<<"Wrote."<<std::endl;
	free(req);
}

void get_msg(uv_idle_t *handle)
{
	char *data = new char[1024];
	uv_stream_t **stream = nullptr;
	unsigned int len = 0;
	get_one_cmd_from_cache_msg(data, len, stream);
	if(len > 0 && stream != nullptr)
	{
		dispatch_cmd(data, len, *stream);
	}
}

void dispatch_cmd(char *data, ssize_t nread, uv_stream_t *stream)
{
	switch(data[0])
	{
		case CMD_SC_NEW:
			handle_add_entity(data, nread, stream);
			break;
		case CMD_SC_ROI_ADD:
			handle_add_roi_entity(data, nread, stream);
			break;
		case CMD_SC_ROI_MV:
			handle_mv_roi_entity(data, nread, stream);
			break;
		case CMD_SC_ROI_RM:
			handle_rm_roi_entity(data, nread, stream);
			break;
		default:
			break;
	}
}

void handle_add_entity(char *data, ssize_t nread, uv_stream_t *stream)
{
	size_t int_size = sizeof(int);
	unsigned int id = *((unsigned int *)&data[1]);
	int x = *((unsigned int *)&data[1 + int_size]);
	int y = *((unsigned int *)&data[1 + int_size * 2]);
	if(avatar)
	{
		std::cerr<<"Avatar exist already."<<std::endl;
		my_write_helper(stream);
		return;
	}

	avatar = (ClientEntity*)malloc(sizeof(ClientEntity));
	avatar->id = id;
	avatar->pos[COORD_X] = x;
	avatar->pos[COORD_Y] = y;
	my_write_helper(stream);
}

void handle_add_roi_entity(char *data, ssize_t nread, uv_stream_t *stream)
{
	size_t int_size = sizeof(int);
	unsigned int id = *((unsigned int *)&data[1]);
	if(!avatar) 
	{
		std::cerr<<"No avatar yet."<<std::endl;
		my_write_helper(stream);
		return;
	}
	unsigned int num = *((unsigned int *)&data[1 + int_size]);
	int offset = 1 + int_size * 2;
	for(int i = 0; i < num; i++)
	{
		unsigned int tgt_id = *((unsigned int *)&data[offset + i * (3 * int_size)]); 
		int x =  *((unsigned int *)&data[offset + i * (3 * int_size) + int_size]); 
		int y =  *((unsigned int *)&data[offset + i * (3 * int_size) + int_size * 2]); 
		ClientEntity *ent = (ClientEntity*)malloc(sizeof(ClientEntity)); 
		ent->id = tgt_id;
		ent->pos[COORD_X] = x;
		ent->pos[COORD_Y] = y;
		roi_entities[tgt_id] = ent;
	}
	my_write_helper(stream);
}

void handle_mv_roi_entity(char *data, ssize_t nread, uv_stream_t *stream)
{
	size_t int_size = sizeof(int);
	unsigned int id = *((unsigned int *)&data[1]);
	if(!avatar) 
	{
		std::cerr<<"No avatar yet."<<std::endl;
		my_write_helper(stream);
		return;
	}
	unsigned int num = *((unsigned int *)&data[1 + int_size]);
	int offset = 1 + int_size * 2;
	for(int i = 0; i < num; i++)
	{
		unsigned int tgt_id = *((unsigned int *)&data[offset + i * (3 * int_size)]); 
		int x =  *((unsigned int *)&data[offset + i * (3 * int_size) + int_size]); 
		int y =  *((unsigned int *)&data[offset + i * (3 * int_size) + int_size * 2]); 
		if(roi_entities.count(tgt_id) <= 0)
		{
			std::cerr<<"No entity yet: "<<tgt_id<<std::endl;
			continue;
		}
		ClientEntity *ent = roi_entities[tgt_id];
		ent->id = tgt_id;
		ent->pos[COORD_X] = x;
		ent->pos[COORD_Y] = y;
	}
	my_write_helper(stream);
}

void handle_rm_roi_entity(char *data, ssize_t nread, uv_stream_t *stream)
{
	size_t int_size = sizeof(int);
	unsigned int id = *((unsigned int *)&data[1]);
	if(!avatar) 
	{
		std::cerr<<"No avatar yet."<<std::endl;
		my_write_helper(stream);
		return;
	}
	unsigned int num = *((unsigned int *)&data[1 + int_size]);
	int offset = 1 + int_size * 2;
	for(int i = 0; i < num; i++)
	{
		unsigned int tgt_id = *((unsigned int *)&data[offset + i * (3 * int_size)]); 
		int x =  *((unsigned int *)&data[offset + i * (3 * int_size) + int_size]); 
		int y =  *((unsigned int *)&data[offset + i * (3 * int_size) + int_size * 2]); 
		if(roi_entities.count(tgt_id) <= 0)
		{
			std::cerr<<"No entity yet: "<<tgt_id<<std::endl;
			continue;
		}
		ClientEntity *ent = roi_entities[tgt_id];
		roi_entities.erase(ent->id);
		free(ent);
	}
	my_write_helper(stream);
}

void alloc_buf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
	if(nread < 0)
	{
		if(nread != UV_EOF)
		{
			std::cerr<<"Read error: "<<uv_err_name(nread)<<std::endl;
		}
		uv_close((uv_handle_t*)stream, NULL);
	}
	else if(nread > 0)
	{
		push_data_to_cache_msg(buf->base, nread, stream);
	}

	if(buf->base) free(buf->base);
}

void my_write_helper(uv_stream_t *stream)
{
	std::cout<<std::endl;
	std::cout<<"1.Add a new entity."<<std::endl;
	std::cout<<"2.Move entity."<<std::endl;
	std::cout<<"3.Remove entity."<<std::endl;
	std::cout<<"4.Print entity's roi."<<std::endl;
	int choice = 0;
	std::cin>>choice;
	switch(choice)
	{
		case 1:
			std::cout<<"Please enter x and y:"<<std::endl;
			int x, y;
			std::cin>>x>>y;
			add_entity_helper(stream, x, y);
			break;
		case 2:
			std::cout<<"Please enter dx and dy:"<<std::endl;
			int dx, dy;
			std::cin>>dx>>dy;
			move_entity_helper(stream, dx, dy);
			break;
		case 3:
			remove_entity_helper(stream);
			break;
		case 4:
			print_entity_roi_helper();
			break;
		default:
			std::cout<<"What do you mean?"<<std::endl;
			my_write_helper(stream);
			break;
	}
}

void add_entity_helper(uv_stream_t *stream, int x, int y)
{
	size_t int_size = sizeof(int);
	char buf[64] = {0};
	buf[0] = PROTO_START;
	buf[1 + int_size] = CMD_NEW;
	memcpy(&buf[2 + int_size], &x, int_size);
	memcpy(&buf[2 + int_size * 2], &y, int_size);
	int len = strlen(buf);
	memcpy(&buf[1], &len, int_size);
	uv_buf_t wrbuf = uv_buf_init(buf, len);
	uv_write_t *wreq = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_write(wreq, stream, &wrbuf, 1, on_write);
	return;
}

void move_entity_helper(uv_stream_t *stream, int dx, int dy)
{
	size_t int_size = sizeof(int);
	char buf[64] = {0};
	buf[0] = PROTO_START;
	buf[1 + int_size] = CMD_MV;
	memcpy(&buf[1 + int_size + 1], &(avatar->id), int_size);
	memcpy(&buf[1 + int_size + 1 + int_size], &dx, int_size);
	memcpy(&buf[1 + int_size + 1 + int_size * 2], &dy, int_size);
	int len = strlen(buf);
	memcpy(&buf[1], &len, int_size);
	uv_buf_t wrbuf = uv_buf_init(buf, len);
	uv_write_t *wreq = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_write(wreq, stream, &wrbuf, 1, on_write);
	return;
}

void remove_entity_helper(uv_stream_t *stream)
{
	size_t int_size = sizeof(int);
	char buf[64] = {0};
	buf[0] = PROTO_START;
	buf[1 + int_size] = CMD_GONE;
	memcpy(&buf[1 + int_size + 1], &(avatar->id), int_size);
	int len = strlen(buf);
	memcpy(&buf[1], &len, int_size);
	uv_buf_t wrbuf = uv_buf_init(buf, len);
	uv_write_t *wreq = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_write(wreq, stream, &wrbuf, 1, on_write);
	return;
}

void print_entity_roi_helper()
{
	std::cout<<"Entity in roi:\n";
	for(auto it = roi_entities.begin(); it != roi_entities.end(); it++)
	{
		std::cout<<it->first<<'\t'<<'('<<it->second->pos[COORD_X]<<','<<it->second->pos[COORD_Y]<<")\n";
	}
	std::cout<<std::endl;
}

void on_connect(uv_connect_t *req, int status)
{
	if(status < 0)
	{
		std::cerr<<"on connect error: "<<uv_strerror(status)<<std::endl;
		return;
	}
	std::cout<<"Connected.\n";
	uv_stream_t *stream = req->handle;
	my_write_helper(stream);
	uv_read_start(stream, alloc_buf, on_read);
}

int main()
{
	loop = uv_default_loop();
	uv_tcp_t *socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));

	uv_tcp_init(loop, socket);
	
	uv_idle_t idler;
	uv_idle_init(loop, &idler);
	uv_idle_start(&idler, get_msg);

	uv_connect_t *connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

	struct sockaddr_in dest;
	uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &dest);

	uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);

	return uv_run(loop, UV_RUN_DEFAULT);
}

