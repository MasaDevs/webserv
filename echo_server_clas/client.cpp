#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024
#define BUFFER_SIZE 80

int main(int argc, char* argv[]) {
	int client_socket, rc;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	// ソケットの作成
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0) {
		perror("socket() failed");
		exit(-1);
	}

	// サーバーアドレスの設定
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, SERVER_ADDR, &(server_addr.sin_addr)) <= 0) {
		perror("inet_pton() failed");
		close(client_socket);
		exit(-1);
	}
	server_addr.sin_port = htons(SERVER_PORT);

	// サーバーに接続
	if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("connect() failed");
		close(client_socket);
		exit(-1);
	}

	printf("Connected to server. Enter a message to send:\n");
	while (1) {
		// ユーザーからの入力を読み取る
		fgets(buffer, sizeof(buffer), stdin);

		// サーバーにメッセージを送信
		rc = send(client_socket, buffer, strlen(buffer), 0);
		if (rc < 0) {
			perror("send() failed");
			close(client_socket);
			exit(-1);
		}

		// サーバーからの返信を受信
		rc = recv(client_socket, buffer, sizeof(buffer), 0);
		if (rc < 0) {
			perror("recv() failed");
			close(client_socket);
			exit(-1);
		} else if (rc == 0) {
			printf("Server closed the connection.\n");
			break;
		}

		buffer[rc] = '\0';
		printf("Received from server: %s", buffer);
	}

	close(client_socket);
	return 0;
}
