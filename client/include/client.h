#ifndef	CLIENT_H
#define	CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

//echo_client.c
int echo_client(char *server, unsigned short port_number, char *message);

//echo_receive_reply.c
int echo_receive_reply(int socketfd, char read_buffer[], size_t buffer_size);

//echo_client_receive.c
int	echo_send_request(int socketfd, char *message);

//tcp connect
int		tcp_connect(char *server_address, unsigned short port_number);
#endif
