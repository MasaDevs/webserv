#include "server.h"

int		tcp_receiver_socket(char *server_address, unsigned short port_number)
{
	int					receiver_socket;
	struct sockaddr_in	addr;

	receiver_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (receiver_socket < 0)
	{
		fprintf(stderr, "server socket error\n");
		return (-1);
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_number);
	addr.sin_addr.s_addr = inet_addr(server_address);
	if (bind(receiver_socket, (const struct sockaddr *)(&addr), sizeof(addr)) < 0)
	{
		fprintf(stderr, "bind error.\n");
		close(receiver_socket);
		return (-1);
	}

	if (listen(receiver_socket, 3) < 0)
	{
		fprintf(stderr, "listen error\n");
		close(receiver_socket);
		return (-1);
	}
	printf("server setup complete!");
	return (receiver_socket);
}

