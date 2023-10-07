
#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "ServerSocket.hpp"

#define BUF_SIZE 1024
#define TRUE 1
#define FALSE 0

namespace http {

class TcpServer {
private:
	ServerSocket server_socket_;
	int max_sd_;
	int max_clients_;
	struct timeval timeout_;
	fd_set master_set_, working_set_;

	TcpServer();

public:
	TcpServer(const char* server_addr, int port, int max_clients = 10);
	~TcpServer();

	int start();
	void stop();
	int initialize();
};

} // namespace http

#endif
