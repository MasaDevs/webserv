#include "server.h"

void	echo_select_server(char *server_address, unsigned short port_number)
{
	int			receiver_socket;
	int			connection_socket;
	int			maxfds, next_maxfds;
	fd_set		readfds, readfds_save;	


	receiver_socket = tcp_receiver_socket(server_address, port_number);
	if (receiver_socket < 0)
	{
		fprintf(stderr, "receiver socket error\n");
		return ;
	}
	
	FD_ZERO(&readfds_save);
	FD_SET(receiver_socket, &readfds_save);
	maxfds = receiver_socket + 1;
	next_maxfds = maxfds;

	while(1)
	{
		readfds = readfds_save;
		//selecting
		int	num_of_change = select(maxfds, &readfds, 0, 0, 0);
		if (num_of_change <= 0)
		{
			fprintf(stderr, "select error\n");
			return ;
		}

		//waing connection
		if (FD_ISSET(receiver_socket, &readfds))
		{
			FD_CLR(receiver_socket, &readfds);
			printf("waiting incoming connection..\n");
			if ((connection_socket = accept(receiver_socket, NULL, NULL)) < 0)
			{
				fprintf(stderr, "accept error\n");
				return ;
			}
			FD_SET(connection_socket, &readfds_save);
			if (connection_socket + 1 > maxfds)
				next_maxfds = connection_socket + 1;
		}
		for (int i = 0; i < maxfds; i++)
			if (FD_ISSET(i, &readfds))
				echo_handle_request_and_reply(i, maxfds, &next_maxfds, &readfds_save);
		maxfds = next_maxfds;
	}
	return ;
}	
