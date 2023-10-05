#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

int transfer(int sock) {
    char send_buf[BUF_SIZE], recv_buf;
    int send_size, recv_size;

    while (1) {

        /* サーバーに送る文字列を取得 */
        printf("Input Message...\n");
        scanf("%s", send_buf);

        /* 文字列を送信 */
		// ssize_t send(int sockfd, const void *buf, size_t len, int flags);
		// return_value 正の数値: 実際に送信されたバイト数。 -1: エラー
        send_size = send(sock, send_buf, strlen(send_buf) + 1, 0);
        if (send_size == -1) {
            printf("send error\n");
            break;
        }

        /* サーバーからの応答を受信 */
        recv_size = recv(sock, &recv_buf, 1, 0);
        if (recv_size == -1) {
            printf("recv error\n");
            break;
        }
        if (recv_size == 0) {
            /* 受信サイズが0の場合は相手が接続閉じていると判断 */
            printf("connection ended\n");
            break;
        }

        /* 応答が0の場合はデータ送信終了 */
        if (recv_buf == 0) {
            printf("Finish connection\n");
            break;
        }

		std::cout << recv_buf << std::endl;
    }
    return 0;
}

int main()
{
	int sock;
	struct sockaddr_in addr;

    /* ソケットを作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("socket error\n");
        return -1;
    }

    /* 構造体を全て0にセット */
    memset(&addr, 0, sizeof(struct sockaddr_in));

    /* サーバーのIPアドレスとポートの情報を設定 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    /* サーバーに接続要求送信 */
    printf("Start connect...\n");
	// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	// 接続を試みるソケットのファイルディスクリプタ
	// return_value 0 成功　-1 エラー
    if (connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1) {
        printf("connect error\n");
        close(sock);
        return -1;
    }
    printf("Finish connect!\n");

    /* 接続済のソケットでデータのやり取り */
    transfer(sock);

    /* ソケット通信をクローズ */
    close(sock);

    return 0;
}
