#include "ServerSocket.hpp"

http::ServerSocket::ServerSocket(const char* server_addr, int port)
	: server_addr_(server_addr)
	, port_(port)
	, listen_sd_(-1) {}

http::ServerSocket::~ServerSocket() {
	std::cout << "ServerSocket default destractor call listen_sd_: " << listen_sd_ << std::endl;
	if (listen_sd_ >= 0) {
		if (close(listen_sd_) < 0) {
			std::cerr << "Failed to close socket: " << strerror(errno) << std::endl;
			exit(1);
		}
		listen_sd_ = -1;
	}
}

int http::ServerSocket::initialize() {

	listen_sd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd_ < 0) {
		std::cerr << "socket() failed: " << strerror(errno) << std::endl;
		return -1;
	}

	int rc;
	int on = 1;

	rc = setsockopt(listen_sd_, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	if (rc < 0) {
		std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
		return -1;
	}

	rc = fcntl(listen_sd_, F_GETFL, 0);
	if (rc < 0) {
		std::cerr << "fcntl() get flags failed: " << strerror(errno) << std::endl;
		return -1;
	}

	rc |= O_NONBLOCK;
	rc = fcntl(listen_sd_, F_SETFL, rc);
	if (rc < 0) {
		std::cerr << "fcntl() set flags failed: " << strerror(errno) << std::endl;
		return -1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, this->getServerAddr(), &(addr.sin_addr)) <= 0) {
		std::cerr << "inet_pton() : " << strerror(errno) << std::endl;
		return -1;
	}
	addr.sin_port = htons(this->getPort());
	rc = bind(listen_sd_, (struct sockaddr*)&addr, sizeof(addr));

	if (rc < 0) {
		std::cerr << "bind() failed: " << strerror(errno) << std::endl;
		return -1;
	}
	return 0;
}

const char* http::ServerSocket::getServerAddr() const {
	return server_addr_;
}

int http::ServerSocket::getPort() const {
	return port_;
}

int http::ServerSocket::getListenSd() const {
	return listen_sd_;
}

// #include "ServerSocket.hpp"

// http::ServerSocket::ServerSocket()
// 	: server_addr_("")
// 	, port_(-1)
// 	, listen_sd_(-1) {}

// http::ServerSocket::ServerSocket(const char* server_addr, int port)
// 	: server_addr_(server_addr)
// 	, port_(port)
// 	, listen_sd_(-1) {}

// http::ServerSocket::~ServerSocket() {
// 	if (listen_sd_ >= 0) {
// 		close(listen_sd_);
// 	}
// }

// int http::ServerSocket::initialize() {
// 	listen_sd_ = socket(AF_INET, SOCK_STREAM, 0);
// 	if (listen_sd_ < 0) {
// 		std::cerr << "socket() failed: " << strerror(errno) << std::endl;
// 		return -1;
// 	}

// 	int rc;
// 	int on = 1;

// 	rc = setsockopt(listen_sd_, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
// 	if (rc < 0) {
// 		std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
// 		return -1;
// 	}

// 	rc = fcntl(listen_sd_, F_GETFL, 0);
// 	if (rc < 0) {
// 		std::cerr << "fcntl() get flags failed: " << strerror(errno) << std::endl;
// 		return -1;
// 	}

// 	rc |= O_NONBLOCK;
// 	rc = fcntl(listen_sd_, F_SETFL, rc);
// 	if (rc < 0) {
// 		std::cerr << "fcntl() set flags failed: " << strerror(errno) << std::endl;
// 		return -1;
// 	}

// 	struct sockaddr_in addr;
// 	memset(&addr, 0, sizeof(addr));
// 	addr.sin_family = AF_INET;
// 	if (inet_pton(AF_INET, this->getServerAddr(), &(addr.sin_addr)) <= 0) {
// 		std::cerr << "inet_pton() : " << strerror(errno) << std::endl;
// 		return -1;
// 	}
// 	addr.sin_port = htons(this->getPort());
// 	rc = bind(listen_sd_, (struct sockaddr*)&addr, sizeof(addr));

// 	if (rc < 0) {
// 		std::cerr << "bind() failed: " << strerror(errno) << std::endl;
// 		return -1;
// 	}
// 	return 0;
// }

// const char* http::ServerSocket::getServerAddr() const {
// 	return this->server_addr_;
// }

// int http::ServerSocket::getPort() const {
// 	return this->port_;
// }

// int http::ServerSocket::getListenSd() const {
// 	return this->listen_sd_;
// }
