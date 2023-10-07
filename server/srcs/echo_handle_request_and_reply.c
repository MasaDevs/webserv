#include "server.h"

static int	find_maxfds(fd_set *fds)
{
	for (int i = FD_SETSIZE; 0 <= i; i--)
		if (FD_ISSET(i, fds))
			return (i + 1);
	return (0);
}

void	echo_handle_request_and_reply(int socket, int maxfds, int *next_maxfds, fd_set *readfds_save)
{
	if (echo_receive_request_and_send_reply(socket) <= 0)	
	{
		fprintf(stderr, "[%d] connection was closed.", socket);
		close(socket);
		FD_CLR(socket, readfds_save);
		if (maxfds == socket + 1)
			*next_maxfds = find_maxfds(readfds_save);
	}
}


