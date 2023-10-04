#include <iostream>
#include <netdb.h>

int main() {
    struct addrinfo hints, *res;
    int err;

    // ヒントの初期化
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;   // IPv4 または IPv6
    hints.ai_socktype = SOCK_STREAM;

    // www.example.com のアドレス情報を取得
    if ((err = getaddrinfo("www.example.com_nn", "http", &hints, &res)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(err) << std::endl;
        return 1;
    }

    // 使用後にメモリを解放
    freeaddrinfo(res);

    return 0;
}

