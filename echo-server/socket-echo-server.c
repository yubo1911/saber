#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define DEFAULT_PORT 7000

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	int sockfd, newsockfd, n, bind_res;
	socklen_t clilen;
	char buffer[256];
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

	listen(sockfd, 5);
	clilen = sizeof(client_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
	if(newsockfd < 0)
		error("Error when accept socket.\n");
	
	while(1)
	{
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if(n < 0)
			error("Error when reading socket.\n");
		printf("Here is the message: %s", buffer);
		n = write(newsockfd, buffer, strlen(buffer));
		if(n < 0)
			error("Error when writing socket.\n");
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
