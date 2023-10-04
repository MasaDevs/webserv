#include <iostream>
#include <cerrno>
#include <cstring>
#include <fstream>

int main() {
    std::ifstream file("non_existent_file.txt");

    if (!file) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    }

    return 0;
}

