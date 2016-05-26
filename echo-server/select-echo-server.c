#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 7000
#define BACKLOG 5

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

	fd_set fdsr;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int i = 0, conn_amount = 0, ret = 0;
	int maxsock = sockfd;
	int newfd;
	clilen = sizeof(client_addr);
	while(1)
	{
		FD_ZERO(&fdsr);
		FD_SET(sockfd, &fdsr);
		for (i=0; i<BACKLOG; ++i)
		{
			if (fd_A[i] != 0)
			{
				FD_SET(fd_A[i], &fdsr);
			}
		}

		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);
		if (ret < 0)
		{
			error("Error in select.\n");
		}
		else if (ret == 0)
		{
			//printf("Just timeout.\n");
			continue;
		}

		for (i=0; i<conn_amount; ++i)
		{
			if (FD_ISSET(fd_A[i], &fdsr))
			{
				echo_message(fd_A[i]);
			}
		}

		if (FD_ISSET(sockfd, &fdsr))
		{
			newfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
			if (newfd < 0)
				error("Error when accept socket.\n");
			if (conn_amount < BACKLOG)
			{
				fd_A[conn_amount] = newfd;
				conn_amount++;
				printf("New connection client[%d] %s:%d\n", conn_amount,
						inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				if (newfd > maxsock)
				{
					maxsock = newfd;
				}
			}
			else
			{
				printf("Now can't accept more connections.\n");
				send(newfd, "bye", 4, 0);
				close(newfd);
				continue;
			}
		}
	}
	
	close(sockfd);
	return 0;
}
