#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

uv_loop_t *loop;
#define DEFAULT_PORT 7000

void my_write_helper(uv_stream_t *);

void alloc_buf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*) malloc(suggested_size);
	buf->len = suggested_size;
}

void on_write(uv_write_t *req, int status)
{
	if (status)
	{
		fprintf(stderr, "Write error %s.\n", uv_strerror(status));
	}
	printf("Wrote.\n");
	free(req);
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
	if (nread < 0)
	{
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*) stream, NULL);
	} else if (nread > 0)
	{
		printf("Read %s", buf->base);
	}

	if (buf->base)
		free(buf->base);

	my_write_helper(stream);
}

void my_write_helper(uv_stream_t *stream)
{
	printf("Enter a string:");
	char input[1024] = {0};
	fgets(input, sizeof(input), stdin);
	int len = strlen(input);
	uv_buf_t wrbuf = uv_buf_init(input, len);
	uv_write_t *wreq = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_write(wreq, stream, &wrbuf, 1, on_write);
}

void on_connect(uv_connect_t *req, int status)
{
	if (status < 0)
	{
		fprintf(stderr, "On connect error %s.\n", uv_strerror(status));
		return;
	}
	printf("Connected.\n");
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
