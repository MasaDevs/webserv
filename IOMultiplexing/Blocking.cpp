#include <iostream>
#include <unistd.h>
#include <cstring>

int main() {
    int pipefd[2];
    pipe(pipefd);

    // デフォルトはブロッキングモード

    pid_t pid = fork();
    if (pid == 0) {
        sleep(3);
        const char *message = "Hello from child!";
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    } else {
        char buffer[1024];
        ssize_t bytesRead;

        std::cout << "Waiting for data..." << std::endl;
        
        bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        if (bytesRead > 0) {
            std::cout << "Received: " << buffer << std::endl;
        } else {
            std::cerr << "Read error" << std::endl;
        }

        close(pipefd[0]);
        close(pipefd[1]);
    }

    return 0;
}
