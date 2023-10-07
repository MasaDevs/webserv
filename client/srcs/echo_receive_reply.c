#include "client.h"

int echo_receive_reply(int socketfd, char read_buffer[], size_t buffer_size)
{
	int response;

	response = recv(socketfd, read_buffer, buffer_size, 0);
	return (response);
}

