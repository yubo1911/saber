#include <iostream>
#include <deque>
#include <map>
#include <uv.h>
#include <proto.h>

std::map<uv_stream_t*, std::deque<char>* > cache_msg;
void get_cmd_from_cache_msg(char *data, unsigned int &len, uv_stream_t *client);

void push_data_to_cache_msg(char *data, int nread, uv_stream_t *client)
{
	if(cache_msg.count(client) <= 0)
	{
		cache_msg[client] = new std::deque<char>();
	}
	
	for(int i = 0; i < nread; i++)
	{
		cache_msg[client]->push_back(data[i]);
	}
}

void get_one_cmd_from_cache_msg(char *data, unsigned int &len, uv_stream_t **client)
{
	for(auto it = cache_msg.begin(); it != cache_msg.end(); it++)
	{
		get_cmd_from_cache_msg(data, len, it->first);
		if(len > 0)
		{
			*client = it->first;
			return;
		}
	}
}

void get_cmd_from_cache_msg(char *data, unsigned int &len, uv_stream_t *client)
{
	if(cache_msg.count(client) <= 0)
	{
		len = 0;
		return;
	}
	if(cache_msg[client]->size() < 5)
	{
		len = 0;
		return;
	}
	if(cache_msg[client]->at(0) != (char)0xff)
	{
		std::cerr<<"Wrong msg format"<<std::endl;
		exit(-1);
	}
	char clen[4] = {0};
	for(int i = 1; i < 5; i++)
	{
		clen[i-1] = cache_msg[client]->at(i);
	}
	unsigned int ulen = *((unsigned int *)clen);
	if(cache_msg[client]->size() < ulen)
	{
		len = 0;
		return;
	}
	len = ulen - 5;
	for(int i = 0; i < len; i++)
	{
		if(i < 5)
		{
			cache_msg[client]->pop_front();
			continue;
		}
		data[i - 5] = cache_msg[client]->front();
		cache_msg[client]->pop_front();
	}
	return;
}
