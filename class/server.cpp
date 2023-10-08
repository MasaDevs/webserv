// #include "TcpServer.hpp"
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
#define SERVER_PORT 8081
#define BUF_SIZE 1024

#define TRUE 1
#define FALSE 0

#include "TcpServer.hpp"
// #include "ServerSocket.hpp"

int main() {
	// サーバーアドレスとポートを指定して、TcpServerのインスタンスを作成
	http::TcpServer server(SERVER_ADDR, SERVER_PORT);

	// サーバーの初期化
	if (server.initialize() < 0) {
		std::cerr << "Failed to initialize server." << std::endl;
		return 1;
	}
	// サーバーの開始
	std::cout << "Server started. Waiting for connections..." << std::endl;
	int status = server.start();
	return 0;

	// // 任意の終了条件に基づいてサーバーの停止（例: Ctrl+Cを押す、特定のリクエストを受け取るなど）
	// server.stop();

	// return status;
}

// int main() {
//     http::ServerSocket server("127.0.0.1", 8080);

//     if (server.initialize() != 0) {
//         std::cerr << "Failed to initialize the server socket." << std::endl;
//         return -1;
//     }

//     // std::cout << "Server initialized on address: " << server.getServerAddr()
//     //           << ", port: " << server.getPort()
//     //           << ", listen_sd: " << server.getListenSd() << std::endl;

//     // ここにサーバーの処理や他のテストを追加

//     return 0;
// }