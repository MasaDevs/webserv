#include "client.h"

int	main(int argc, char *argv[])
{
	char			*server;
	unsigned short	port_number;
	char			*message;
	int				error_number;

	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s host port 'message\n'", argv[0]);
		return (-1);
	}
	server = argv[1];
	port_number = (unsigned short)strtol(argv[2], 0, 10);
	message = argv[3];
	error_number = echo_client(server, port_number, message);
	return (error_number);
}
