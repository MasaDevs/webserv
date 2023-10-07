#include "client.h"

int	main(int argc, char *argv[])
{
	char			*server;
	unsigned short	port_number;
	int				error_number;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s host port\n", argv[0]);
		return (-1);
	}
	server = argv[1];
	port_number = (unsigned short)strtol(argv[2], 0, 10);
	error_number = echo_client(server, port_number);
	return (error_number);
}
