#include "TcpServer.hpp"

// http::TcpServer::TcpServer(){};

http::TcpServer::TcpServer(const char* server_addr, int port, int max_clients)
	: server_socket_(server_addr, port)
	, max_clients_(max_clients) {}

http::TcpServer::~TcpServer() {
	for (int i = 0; i <= max_sd_; ++i) {
		if (FD_ISSET(i, &master_set_))
			close(i);
	}
}

int http::TcpServer::initialize() {
	if (this->server_socket_.initialize() < 0)
		return -1;
	int rc = listen(this->server_socket_.getListenSd(), this->max_clients_);
	if (rc < 0) {
		std::cout << "listen() failed" << strerror(errno) << std::endl;
		return -1;
	}
	FD_ZERO(&this->master_set_);
	this->max_sd_ = this->server_socket_.getListenSd();
	FD_SET(this->server_socket_.getListenSd(), &this->master_set_);

	this->timeout_.tv_sec = 3 * 60;
	this->timeout_.tv_usec = 0;
	return 0;
}