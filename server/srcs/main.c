#include "server.h"


int main(int argc, char *argv[])
{
	char			*server_address;
	unsigned short	port_number;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s server_name port_number\n", argv[0]);
		return (-1);
	}
	server_address = argv[1];
	port_number = (unsigned short)strtol(argv[2], 0, 10);
	echo_select_server(server_address, port_number);
	return (0);
}

