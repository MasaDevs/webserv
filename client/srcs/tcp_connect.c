#include "client.h"

int		tcp_connect(char *server_address, unsigned short port_number)
{
	int					socketfd;
	struct sockaddr_in	addr;	

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0)
	{
		fprintf(stderr, "client socket error\n");
		return (-1);
	}
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_number);
	addr.sin_addr.s_addr = inet_addr(server_address);

	printf("Start connect server ...\n");
	if (connect(socketfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		fprintf(stderr, "Connection error\n");
		close(socketfd);
		return (-1);
	}
	printf("Connected to server successfully !\n");
	return (socketfd);
}
