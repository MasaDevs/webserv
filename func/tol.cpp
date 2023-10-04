#include <iostream>
#include <arpa/inet.h>  // for htons, htonl, ntohs, ntohl functions

int main() {
    // サンプルデータの定義
    uint16_t host_short = 0x1234;
    uint32_t host_long = 0x12345678;

    // htons と htonl を使用してホストバイトオーダーからネットワークバイトオーダーに変換
    uint16_t net_short = htons(host_short);
    uint32_t net_long = htonl(host_long);

    std::cout << "Host order short: " << std::hex << host_short << ", Network order short: " << net_short << std::endl;
    std::cout << "Host order long: " << std::hex << host_long << ", Network order long: " << net_long << std::endl;

    // ntohs と ntohl を使用してネットワークバイトオーダーからホストバイトオーダーに変換
    uint16_t converted_short = ntohs(net_short);
    uint32_t converted_long = ntohl(net_long);

    std::cout << "Converted back from network to host short: " << std::hex << converted_short << std::endl;
    std::cout << "Converted back from network to host long: " << std::hex << converted_long << std::endl;

    return 0;
}

