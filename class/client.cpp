#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "ClientSocket.hpp"

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8081
#define BUF_SIZE 1024
#define BUFFER_SIZE 80

int main(int argc, char* argv[]) {
	int rc;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	http::ClientSocket client_socket(SERVER_ADDR, SERVER_PORT);
	if (client_socket.initialize() < 0 )
		return -1;

	printf("Connected to server. Enter a message to send:\n");
	while (1) {
		fgets(buffer, sizeof(buffer), stdin);

		rc = send(client_socket.getClientSocket(), buffer, strlen(buffer), 0);
		if (rc < 0) {
			perror("send() failed");
			exit(-1);
		}

		rc = recv(client_socket.getClientSocket(), buffer, sizeof(buffer), 0);
		if (rc < 0) {
			perror("recv() failed");
			exit(-1);
		} else if (rc == 0) {
			printf("Server closed the connection.\n");
			break;
		}

		buffer[rc] = '\0';
		printf("Received from server: %s", buffer);
	}

	return 0;
}
