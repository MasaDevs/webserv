#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

int transfer( int sock ) {

	int recv_size;
	int send_size;

	char recv_buf[BUF_SIZE];
	char send_buf;

	while( true ) {

		//　特定のソケットから、データを受信するための関数//TCPで使用される。
		//  ssize_t recv(int sockfd, void *buf, size_t len, int flags);
		//  return_value 受信したバイト数
		//  送信側がコネクションを終了
		//  エラーが発生した場合、-1 
		recv_size = recv(sock, recv_buf, BUF_SIZE, 0);
		if( recv_size == -1 ) {
			std::cout << "recv error\n";
			break;
		}

		if ( recv_size == 0 ) {
			std::cout << "connection ended\n";
			break;
		}

		std::cout << recv_buf << std::endl;

		if ( strcmp(recv_buf, "finish") == 0 ) {
			send_buf = 0;
			send_size = send(sock, send_buf, 1, 0);
			if ( send_size == -1 ) {
				std::cout << "send error\n";
				break;
			}
			break;
		} else {
			send_size = send( sock, recv_buf, strlen(recv_buf) + 1, 0);
			if ( send_size == -1 ) {
				std::cout << "send error\n";
				break;
			}
		}

	}
	return 0; 
}



int main ( ) {
	int w_addr, c_sock;

	/**
	 *
	 	https://learn.microsoft.com/ja-jp/windows/win32/api/ws2def/ns-ws2def-sockaddr_in
	 	typedef struct sockaddr_in {
		#if ...
  			short          sin_family; // トランスポート アドレスのアドレス ファミリ。常にAF_INETにする。
		#else
	  		ADDRESS_FAMILY sin_family; 
		#endif
 		 	USHORT         sin_port; // トランスポート プロトコルのポート番号。
	  		IN_ADDR        sin_addr; //IPv4 トランスポート アドレスを含む IN_ADDR 構造体。
  			CHAR           sin_zero[8]; // システムで使用するために予約されています。 WSK アプリケーションでは、この配列の内容を 0 に設定する必要があります。
		} SOCKADDR_IN, *PSOCKADDR_IN;
*/
	struct sockaddr_in a_addr;

	// AF_INET == IPv4
	// SOCK_STREAM  接続を確立する、接続指向であり、信頼性を担保している。
	// TCP を使用したプロセス間通信に使用される。他には、HTTP通信、FTP、SSHなど
	w_addr = socket( AF_INET, SOCK_STREAM, 0);
	if ( w_addr == -1 ) {
		std::cout << "sockt error" << std::endl;
		return -1;
	}

	memset(&a_addr, 0, sizeof(struct sockaddr_in));
	
	a_addr.sin_family = AF_INET;
	//bigエンディアンに直している host to network short
	a_addr.sin_port = htons((unsigned short)SERVER_PORT);
	// inet_addr関数は、ipv4アドレスのドット区切りの文字列表記をバイナリ形式(32ビットの整数値)に変換するために使用される。
	a_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	// socketfd に　ソケットに割り当てるアドレスやポート番号の情報を設定する。
	if ( bind( w_addr, (const struct sockaddr *)&a_addr, sizeof(a_addr)) == -1 )  {
			std::cerr << "bind error\n";
			close(w_addr);
			return -1;
	}

	//指定されたソケットが新しい接続要求を受け入れることを待ち受けるようにする
	// backlogは待ち行列の最大長を表します。この待ち行列とは、サーバがまだ受け入れていない接続要求の数のことです。
	if ( listen(w_addr, 3) == -1 ) {
		std::cout << "listen error\n";
		return -1;
	}
	
	while ( true ) {
		std::cout << "Waiting connect ...\n";
		// 新しいクライアントからの接続要求を受け入れる
		// socketfd = w_addr: listen関数で待機モードにしたソケットのディスクリプタ
		// struct sockaddr *addr: 接続要求を行ったクライアントのアドレス情報
		// socklen_t *addrlen sizeof(addr)
		// return_value : 新しく作成されたソケットのディスクリプタ: w_addrは引き続きlistenをしていて、新しく通信のためのsocketfdを作成して返す。
		//  acceptはブロッキング関数(  select とかが関わってくる。（非ブロッキングモード）)
		c_sock = accept(w_addr, NULL, NULL);
		if ( c_sock == -1 ) {
			std::cout << "accept error\n";
			close(w_addr);
			return -1;
		}
		std::cout << "Connected\n";
		std::cout << "test" << std::endl;

		transfer(c_sock);

		close(c_sock);

	}

	close(w_addr);

	return 0;
}

