#include <iostream>
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
#define SERVER_ADDR "127.0.0.1"

int main() {
  int client_socket;
  struct sockaddr_in server_address;
  char buffer[BUFFER_SIZE];

  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "socket failed" << std::endl;
    exit(EXIT_FAILURE);
  } 

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_ADDR, &server_address.sin_addr) <= 0 ) {
    std::cerr << "connect failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Connected to server!" << std::endl;

  while ( true ) {
    std::cout << "Enter message: " ;
    fgets(buffer, BUFFER_SIZE, stdin);

    send(client_socket, buffer, strlen(buffer), 0);

    memset(buffer, 0, BUFFER_SIZE);
    int valread = recv(client_socket, buffer, BUFFER_SIZE, 0);

    if (valread <= 0) {
      std::cerr << "Server disconnected or error occurred." << std::endl;
      close(client_socket);
      break;
    }

    buffer[valread] = '\0';
    std::cout << "Server reply: " << buffer << std::endl;
  }
  return 0;
}