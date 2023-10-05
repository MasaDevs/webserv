#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_ADDR "127.0.0.1"  // このアドレスはサーバが動作するマシンのアドレスに変更してください

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // ソケットの作成
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // サーバのIPアドレスを設定
    if (inet_pton(AF_INET, SERVER_ADDR, &server_address.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    // サーバに接続
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server!\n");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // メッセージをサーバに送信
        send(client_socket, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);  // バッファをクリア
        int valread = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (valread <= 0) {
            printf("Server disconnected or error occurred.\n");
            close(client_socket);
            break;
        }

        buffer[valread] = '\0';
        printf("Server reply: %s", buffer);
    }

    return 0;
}

