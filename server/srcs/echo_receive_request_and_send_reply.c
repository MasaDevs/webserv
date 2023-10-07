#include "server.h"
#define BUFFER_SIZE 1024

ssize_t	echo_receive_request_and_send_reply(int connection_socket)
{
	char	read_buffer[BUFFER_SIZE];
	ssize_t	response;

	memset(read_buffer, '\0', BUFFER_SIZE);
	response = recv(connection_socket, read_buffer, BUFFER_SIZE, 0);
	if (response < 0)
	{
		fprintf(stderr, "recv error\n");
		return (-1);
	}
	response = send(connection_socket, read_buffer, BUFFER_SIZE, 0);
	if (response < 0)
	{
		fprintf(stderr, "send error\n");
		return (-1);
	}
	return (response);
}

