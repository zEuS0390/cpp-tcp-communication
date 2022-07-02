#include <iostream>
#include <string>
#include "options.h"

int main (void) {
    int retval = 0;
    std::string select;
    std::cout << "TCP Socket Options\n"
              << "1 - Server\n"
              << "2 - Client\n"
              << "Select [1-2]: ";
    std::getline(std::cin, select);
    if (select == "1") {
        retval = tcp::server();
    } else if (select == "2") {
        retval = tcp::client();
    } else {
        std::cout << "Invalid input!" << std::endl;
    }
    std::cin.get();
    return retval;
}
