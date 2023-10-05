#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
  int master_socket, addr_len, new_socket, max_sd, activity, valread, sd;
  int client_socket[MAX_CLIENTS];
  struct sockaddr_in address;
  char buffer[BUFFER_SIZE];
  fd_set readfds;

  for ( int i=0; i < MAX_CLIENTS; ++i) {
    client_socket[i] =0;
  }

  if ( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);


  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }

  if (listen(master_socket, MAX_CLIENTS) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  std::cout << "Listening on port " << PORT << "..." << std::endl;
  
  addr_len = sizeof(address);
  while( 1 ) {
    FD_ZERO(&readfds);

    FD_SET(master_socket, &readfds);
    max_sd = master_socket;
  
    for ( int i=0; i < MAX_CLIENTS; ++i ) {
      sd = client_socket[i];
      if ( sd > 0 ) FD_SET(sd, &readfds);
      if ( sd > max_sd ) max_sd = sd;
    }

    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0 )  && (errno != EINTR)) {
      std::cerr << "select error" << std::endl;
    }

    if (FD_ISSET(master_socket, &readfds)) {
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addr_len) < 0)) {
        std::cerr << "accept" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    std::cout << "New connection form " << inet_ntoa(address.sin_addr) << " : " << ntohs(address.sin_port) << std::endl;

    for (int i=0; i < MAX_CLIENTS; ++i) {
      if ( client_socket[i] == 0) {
        client_socket[i] = new_socket;
        break;
      }
    }

    for (int i=0; i < MAX_CLIENTS; ++i) {
      sd = client_socket[i];
      if(FD_ISSET(sd, &readfds)) {
        valread = read(sd, buffer, BUFFER_SIZE);
        if (valread == 0) {
          close(sd);
          client_socket[i] =0;
        } else {
          buffer[valread] = '\0';
          send(sd, buffer, strlen(buffer),0);
        }
      }
    }
  }
  return 0; 
}