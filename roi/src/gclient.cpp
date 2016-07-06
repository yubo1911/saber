#include <stdlib.h>
#include <uv.h>
#include <entity.h>
#include <map>

#define DEFAULT_PORT 7000

uv_loop_t *loop;
ClientEntity *avatar;
std::map<unsigned int, ClientEntity*> roi_entities;

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
		// TODO handle server to client proto and update client entities
	}

	if(buf->base) free(buf->base);
	my_write_helper(stream);
}

void my_write_helper(uv_stream_t *stream)
{
	// TODO offer some options to choice to send proto to server
	
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

	uv_connect_t *connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

	struct sockaddr_in dest;
	uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &dest);

	uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);

	return uv_run(loop, UV_RUN_DEFAULT);
}

