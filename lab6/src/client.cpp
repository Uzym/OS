#include "../include/zmq_manage.h"

int main(int argc, char const *argv[])
{
    
    pid_t api_daemon_pid = fork();
    if (api_daemon_pid == -1) {
        std::cerr << "Error to fork api_daemon\n";
        return EXIT_FAILURE;
    } else if (api_daemon_pid == 0) {
        execl("./api_daemon", NULL);
    }

    std::cout 
            << "commands:\n" 
            << "\tcreate id parent\n" 
            << "\tremove id\n"
            << "\texec id var_name value\n"
            << "\texec id var_name\n"
            << "\tping id\n"
            << "\theartbeat time\n";

    zmq::context_t contex(1);
    zmq::socket_t api_socket(contex, zmq::socket_type::pair);

    api_socket.bind(get_port_name(API_PORT));

    std::string str;

    bool flag = true;
    while (flag) {
        std::getline(std::cin, str);
        send_message(api_socket, str);
        
        if (str == "exit") {
            flag = false;
        }
    }

    return EXIT_SUCCESS;
}

