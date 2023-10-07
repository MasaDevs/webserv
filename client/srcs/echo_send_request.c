#include "client.h"

int	echo_send_request(int socketfd, char *message)
{
	int response;
	response = send(socketfd, message, strlen(message), 0);
	return (response);
}
