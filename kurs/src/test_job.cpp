//
// Created by lyutoev on 27.12.22.
//

#include "iostream"
#include "sstream"

int main(int argc, char const *argv[])
{
    std::stringstream ss(argv[1]);
    int i;
    if (ss >> i) {
        std::cout << i + 1;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
