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
#define MAXCLIENT 10

class ServerSocket;

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
	TcpServer(const char* server_addr, int port, int max_clients = MAXCLIENT);
	~TcpServer();

	int start() {
		int listen_sd, max_sd, new_sd;
		int desc_ready, end_server = FALSE;
		int close_conn;
		char buffer[BUF_SIZE];
		int len;
		int rc;
		while (end_server == FALSE) {
			memcpy(&this->working_set_, &this->master_set_, sizeof(this->master_set_));

			std::cout << "Waiting on select()..." << std::endl;
			rc = select(max_sd + 1, &this->working_set_, NULL, NULL, &this->timeout_);

			if (rc < 0) {
				perror("  select() failed");
				break;
			}

			if (rc == 0) {
				std::cout << "  select() timed out.  End program." << std::endl;
				break;
			}

			desc_ready = rc;
			// max_fdになるまで &&  データが読み取り可能なソケットディスクリプタが無くなるまで
			for (int i = 0; i <= max_sd && desc_ready > 0; ++i) {
				// データが読み取り可能なソケットディスクリプタかどうか
				if (FD_ISSET(i, &this->working_set_)) {
					desc_ready -= 1;
					if (i == listen_sd) {
						std::cout << "  Listening socket is readable" << std::endl;
						do {
							// 接続は確認された時新しい接続は確認された時, acceptする.
							new_sd = accept(listen_sd, NULL, NULL);
							if (new_sd < 0) {
								if (errno != EWOULDBLOCK) {
									perror("  accept() failed");
									end_server = TRUE;
								}
								break;
							}

							std::cout << "  New incoming connection -  " << new_sd << std::endl;
							// 新しい接続をmasterにセットするx
							FD_SET(new_sd, &this->master_set_);
							if (new_sd > max_sd)
								max_sd = new_sd;

						} while (new_sd != -1);
					}
					// 既存のclient接続に関する
					else {
						std::cout << "  Descriptor " << i << " is readable" << std::endl;
						close_conn = FALSE;
						while (TRUE) {
							// データの受信 return_value[size]
							rc = recv(i, buffer, sizeof(buffer), 0);
							if (rc < 0) {
								if (errno != EWOULDBLOCK) {
									perror("  recv() failed");
									close_conn = TRUE;
								}
								break;
							}

							if (rc == 0) {
								std::cout << "  Connection closed" << std::endl;
								close_conn = TRUE;
								break;
							}

							len = rc;
							std::cout << "  " << len << " bytes received\n" << std::endl;

							rc = send(i, buffer, len, 0);
							if (rc < 0) {
								perror("  send() failed");
								close_conn = TRUE;
								break;
							}
						};

						if (close_conn) {
							close(i);
							FD_CLR(i, &this->master_set_);
							if (i == max_sd) {
								while (FD_ISSET(max_sd, &this->master_set_) == FALSE)
									max_sd -= 1;
							}
						}
					}
				}
			}
		}
		return 0;
	}

	void stop() {
		// TODO: 必要に応じて、サーバーの停止処理やリソースの解放をここに実装
	}

	int initialize();
};
}

#endif