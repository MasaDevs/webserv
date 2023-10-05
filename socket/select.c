#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int master_socket, addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;
    char buffer[1025]; 
    fd_set readfds;
    
    // ソケットの初期化...
    
    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }
    
    while(1) {
        FD_ZERO(&readfds);

        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];
            if(sd > 0) FD_SET(sd, &readfds);
            if(sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno!=EINTR)) {
            printf("select error");
        }

        if (FD_ISSET(master_socket, &readfds)) {
            // 新しい接続の処理...

        } else {
            for (i = 0; i < max_clients; i++) {
                sd = client_socket[i];
                if (FD_ISSET(sd, &readfds)) {
                    // ソケットからのデータの読み取り処理...
                }
            }
        }
    }
    return 0;
}

