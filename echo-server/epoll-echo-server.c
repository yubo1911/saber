#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 7000
#define BACKLOG 5
#define MAX_EVENTS 10

int fd_A[BACKLOG];

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

void echo_message(int newsockfd)
{
	char buffer[256];
	int n;
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if(n < 0)
		perror("Error when reading socket.\n");
	if(n == 0)
		return;
	printf("Here is the message: %s", buffer);
	n = write(newsockfd, buffer, strlen(buffer));
	if(n < 0)
		perror("Error when writing socket.\n");
}

int main()
{
	int sockfd, bind_res;
	socklen_t clilen;
	struct sockaddr_in server_addr, client_addr;
	clilen = sizeof(client_addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("Error when create socket.\n");

	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(DEFAULT_PORT);
	bind_res = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(bind_res < 0)
		error("Error when binding socket.\n");

	listen(sockfd, BACKLOG);

	struct epoll_event ev, events[MAX_EVENTS];
	int i, nfds, newsock, epollfd;
	
	epollfd = epoll_create(MAX_EVENTS);
	if (epollfd < 0)
	{
		error("Error when create epoll fd.\n");
	}
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
	{
		error("Error when add listen sock to epoll.\n");
	}

	while(1)
	{
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			error("Error when epoll wait.\n");
		}
		for (i=0; i<nfds; ++i)
		{
			if (events[i].data.fd == sockfd)
			{
				newsock = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
				if (newsock == -1)
				{
					error("Error when accept new sock.\n");
				}

				//setnonblocking(newsock);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = newsock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newsock, &ev) == -1)
				{
					error("Error when add newsock to epoll.\n");
				}
			}
			else
			{
				echo_message(events[i].data.fd);
			}
		}
	}
	
	close(sockfd);
	return 0;
}
