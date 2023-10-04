#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>  // for close function

int main() {
    fd_set readfds;
    struct timeval timeout;

    // ファイルディスクリプタのセットを初期化
    FD_ZERO(&readfds);

    // 標準入力（ファイルディスクリプタ0）をセットに追加
    FD_SET(0, &readfds);

    // タイムアウトを10秒に設定
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int ret = select(1, &readfds, NULL, NULL, &timeout);

    if (ret == -1) {
        perror("select");
    } else if (ret == 0) {
        std::cout << "Timeout occurred! No data after 10 seconds.\n";
    } else {
        std::cout << "Data is available now.\n";
    }

    return 0;
}

