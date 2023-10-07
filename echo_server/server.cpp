#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[]) {
	int i, len, rc, on = 1;
	int listen_sd, max_sd, new_sd;
	int desc_ready, end_server = FALSE;
	int close_conn;
	char buffer[BUF_SIZE];
	struct sockaddr_in addr;
	struct timeval timeout;
	fd_set master_set, working_set;

	listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0) {
		perror("socket() failed");
		exit(-1);
	}

	rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	if (rc < 0) {
		perror("setsockopt() failed");
		close(listen_sd);
		exit(-1);
	}

	rc = fcntl(listen_sd, F_GETFL, 0);
	if (rc < 0) {
		perror("fcntl() get flags failed");
		close(listen_sd);
		exit(-1);
	}

	rc |= O_NONBLOCK;
	rc = fcntl(listen_sd, F_SETFL, rc);
	if (rc < 0) {
		perror("fcntl() set flags failed");
		close(listen_sd);
		exit(-1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, SERVER_ADDR, &(addr.sin_addr)) <= 0) {
		perror("inet_pton() failed");
		close(listen_sd);
		exit(-1);
	}
	addr.sin_port = htons(SERVER_PORT);
	rc = bind(listen_sd, (struct sockaddr*)&addr, sizeof(addr));

	if (rc < 0) {
		perror("bind() failed");
		close(listen_sd);
		exit(-1);
	}

	rc = listen(listen_sd, 32);
	if (rc < 0) {
		perror("listen() failed");
		close(listen_sd);
		exit(-1);
	}

	FD_ZERO(&master_set);
	max_sd = listen_sd;
	FD_SET(listen_sd, &master_set);

	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;

	while (end_server == FALSE) {
		memcpy(&working_set, &master_set, sizeof(master_set));

		std::cout << "Waiting on select()..." << std::endl;
		rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);

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
		for (i = 0; i <= max_sd && desc_ready > 0; ++i) {
      //データが読み取り可能なソケットディスクリプタかどうか
			if (FD_ISSET(i, &working_set)) {
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
						FD_SET(new_sd, &master_set);
						if (new_sd > max_sd)
							max_sd = new_sd;

					} while (new_sd != -1);
				} 
        // 既存のclient接続に関する
        else {
					std::cout << "  Descriptor " << i << " is readable" << std::endl;
					close_conn = FALSE;
					while (TRUE) {
            //データの受信 return_value[size]
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
						FD_CLR(i, &master_set);
						if (i == max_sd) {
							while (FD_ISSET(max_sd, &master_set) == FALSE)
								max_sd -= 1;
						}
					}
				}
			}
		}
	}

	for (i = 0; i <= max_sd; ++i) {
		if (FD_ISSET(i, &master_set))
			close(i);
	}
}
