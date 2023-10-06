#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/select.h>
#include <cstring>
#include <random>

int main() {
    int pipefd[2];
    pipe(pipefd);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    // ノンブロッキングの設定
    int flags = fcntl(pipefd[0], F_GETFL, 0);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    pid_t pid = fork();
    if (pid == 0) {
        int randomValue = dist(gen);
        if(randomValue == 0) {
          std::cout << "sleep 3" << std::endl;
          sleep(3);
        }  else {
          std::cout << "sleep 1" << std::endl;
          sleep(1);
        }
        const char *message = "Hello from child!";
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    } else {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(pipefd[0], &readfds);

        int result = select(pipefd[0] + 1, &readfds, NULL, NULL, &timeout);
        if (result > 0 && FD_ISSET(pipefd[0], &readfds)) {
            char buffer[1024];
            ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                std::cout << "Received: " << buffer << std::endl;
            } else {
                std::cerr << "Read error" << std::endl;
            }
        } else if( result == 0 ) {
          std::cerr << "Select timeout!!" << std::endl;
          exit(1);
        }

        close(pipefd[0]);
        close(pipefd[1]);
    }

    return 0;
}
