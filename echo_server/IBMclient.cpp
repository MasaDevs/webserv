#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define SERVER_ADDR "::1" // ループバックアドレス
#define BUFFER_SIZE 80

int main() {
    int client_sd;
    struct sockaddr_in6 server_addr;
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    int bytes_sent, bytes_received;

    // クライアントソケットの作成
    client_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (client_sd < 0) {
        perror("socket() failed");
        exit(-1);
    }

    // サーバーのアドレスを設定
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SERVER_ADDR, &(server_addr.sin6_addr));
    server_addr.sin6_port = htons(SERVER_PORT);

    // サーバーに接続
    if (connect(client_sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect() failed");
        close(client_sd);
        exit(-1);
    }

    printf("Connected to server. Enter a message: ");
    fgets(send_buffer, BUFFER_SIZE, stdin);

    // メッセージの送信
    bytes_sent = send(client_sd, send_buffer, strlen(send_buffer), 0);

    if (bytes_sent <= 0) {
        perror("send() failed");
        close(client_sd);
        exit(-1);
    }

    // エコーされたメッセージの受信
    bytes_received = recv(client_sd, recv_buffer, BUFFER_SIZE, 0);

    if (bytes_received > 0) {
        recv_buffer[bytes_received] = '\0';
        printf("Received from server: %s", recv_buffer);
    } else {
        perror("recv() failed");
    }

    close(client_sd);
    return 0;
}
