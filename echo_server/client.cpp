#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024
#define BUFFER_SIZE 80

#define TRUE 1
#define FALSE 0

int main() {
  int client_sd, bytes_sent, bytes_received;
  struct  sockaddr_in addr;
  char send_buffer[BUFFER_SIZE];
  char recv_buffer[BUFFER_SIZE];
  
  client_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_sd < 0) {
    perror("socket() failed");
    exit(1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, SERVER_ADDR, &(addr.sin_addr));
  addr.sin_port = htons(SERVER_PORT);

  if (connect(client_sd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
    perror("connect() failed");
    close(client_sd);
    exit(1);
  }

  std::cout << "Connect to server. Enter a message: " ;
  fgets(send_buffer, BUFFER_SIZE, stdin);

  bytes_sent = send(client_sd, send_buffer, strlen(send_buffer), 0);
  

  if (bytes_sent <= 0) {
    perror("send() failed");
    close(client_sd);
    exit(-1);
  }

  bytes_received = recv(client_sd, recv_buffer, BUFFER_SIZE, 0);
  if (bytes_received > 0) {
    recv_buffer[bytes_received] = '\0';
    std::cout << "Received from server: " << recv_buffer;
  } else {
    perror("recv() failed");
  }

  close(client_sd);
}
