#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace http {

class ServerSocket {
private:
	const char* server_addr_;
	const int port_;
	int listen_sd_;

	// デフォルトコンストラクタは禁止
	ServerSocket();

public:
	ServerSocket(const char* server_addr, int port);
	~ServerSocket();

	int initialize();
	const char* getServerAddr() const;
	int getPort() const;
	int getListenSd() const;
};

} // namespace http

#endif

// #ifndef SERVER_SOCKET_HPP
// #define SERVER_SOCKET_HPP

// #include <arpa/inet.h>
// #include <cerrno>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <fcntl.h>
// #include <iostream>
// #include <netinet/in.h>
// #include <sys/ioctl.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <unistd.h>

// namespace http {
// class ServerSocket {
// private:
// 	const char* server_addr_;
// 	const int port_;
// 	int listen_sd_;
// 	ServerSocket();

// public:
// 	ServerSocket(const char* server_addr, int port);
// 	~ServerSocket();

// 	int initialize();
// 	const char* getServerAddr() const;
// 	int getPort() const;
// 	int getListenSd() const;
// };
// }

// #endif