#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

int main() {
    int sv[2]; // ソケットのペアを保持するための配列

    // AF_UNIXドメインのSOCK_STREAMタイプのソケットペアを作成
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        std::cerr << "Failed to create socket pair." << std::endl;
        return 1;
    }

    // 例: sv[0] に何かを書き込み、sv[1] からそれを読み取る
    const char* message = "Hello from the other side!";
    write(sv[0], message, strlen(message));

    char buffer[256];
    read(sv[1], buffer, sizeof(buffer));
    std::cout << "Received message: " << buffer << std::endl;

    // ソケットを閉じる
    close(sv[0]);
    close(sv[1]);

    return 0;
}

