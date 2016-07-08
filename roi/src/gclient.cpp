#include <stdlib.h>
#include <uv.h>
#include <entity.h>
#include <map>

#define DEFAULT_PORT 7000

uv_loop_t *loop;
ClientEntity *avatar;
std::map<unsigned int, ClientEntity*> roi_entities;

void get_msg(uv_handle_t *handle);

void get_msg(uv_handle_t *handle)
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
	// TODO: parse the msg with proto
	switch(data[0])
	{
		case CMD_SC_ROI_ADD:
			break;
		default:
			break;
	}
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
	// TODO should merge this block method to uv loop every n secs.
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
			std::cout<<"Please enter dx and dy:"<<std:endl;
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
	char buf[64] = {0};
}

void move_entity_helper(uv_stream_t *stream, int dx, int dy)
{}

void remove_entity_helper(uv_stream_t *stream)
{}

void print_entity_roi_helper()
{}

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

