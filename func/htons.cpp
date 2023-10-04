#include <iostream>
#include <arpa/inet.h>  // for htons function

int main() {
    uint16_t host_order_value = 12345;
    uint16_t network_order_value = htons(host_order_value);

    std::cout << "Host order value: " << host_order_value << std::endl;
    std::cout << "Network order value: " << network_order_value << std::endl;

    return 0;
}

