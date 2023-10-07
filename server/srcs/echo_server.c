#include "server.h"

void	echo_server(char *server_address, unsigned short port_number)
{
	int receiver_socket;
	int	connection_socket;

	receiver_socket = tcp_receiver_socket(server_address, port_number);
	if (receiver_socket < 0)
	{
		fprintf(stderr, "receiver socket error\n");
		return ;
	}

	while(1)
	{
		printf("waiting incoming connection..\n");
		if ((connection_socket = accept(receiver_socket, NULL, NULL)) < 0)
		{
			fprintf(stderr, "accept error\n");
			return ;
		}
		echo_receive_request_and_send_reply(connection_socket);
	}
	return ;
}
	
