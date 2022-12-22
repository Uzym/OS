#include "../include/zmq_manage.h"

int main(int argc, char const *argv[])
{
    
    std::cout 
            << "commands:\n" 
            << "\tcreate id parent\n" 
            << "\tremove id\n"
            << "\texec id var_name value\n"
            << "\texec id var_name\n"
            << "\tping id\n"
            << "\theartbit time\n";

    zmq::context_t contex(1);
    zmq::socket_t api_socket(contex, zmq::socket_type::pair);

    api_socket.bind(get_port_name(API_PORT));

    std::string str;

    bool flag = true;
    while (flag) {
        std::cout << "> ";
        std::getline(std::cin, str);
        send_message(api_socket, str);
    }

    return EXIT_SUCCESS;
}

