#include "../include/zmq_manage.h"
#include "../include/tree.h"

int main(int argc, char const *argv[])
{
    
    Tree T = Tree();

    zmq::context_t contex(3);
    zmq::socket_t main_socket(contex, zmq::socket_type::req);

    main_socket.bind(get_port_name(BASE_PORT + manage_worker_id + 1));
    pid_t manage_pid = fork();
    if (manage_pid == -1) {
        std::cerr << "Error to create manage worker\n";
    } else if (manage_pid == 0) {
        create_node(manage_worker_id, BASE_PORT + manage_worker_id + 1);
    }

    std::string command;
    bool flag = true;

    std::string result = "";
    std::string msg_s = "";

    zmq::socket_t api_socket(contex, zmq::socket_type::pair);
    api_socket.connect(get_port_name(API_PORT));

    while (flag) {
        T.print_tree();
        std::cout << result << '\n';
        result = "";
        
        std::vector <std::string> args;

        std::string str;
        str = recieve_message(api_socket);
        
        std::istringstream ist(str);
        for (std::string word; ist >> word; args.push_back(word));

        if (args[0] == "ping" && args.size() == 2) {
            int id = std::stoi(args[1]);

            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            continue;
        }

        if (args[0] == "create" && args.size() == 3) {
            int id = std::stoi(args[1]), parent = std::stoi(args[2]);

            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result != "Ok: 0") {
                std::cout << "Error: Already exists\n";

                continue;
            }

            msg_s = "ping " + std::to_string(parent);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result != "Ok: 1") {
                std::cout << "Error: Not found " << parent << "\n";

                continue;
            }

            msg_s = "create " + std::to_string(id) + " " + std::to_string(parent);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result.substr(0, 2) == "Ok") {
                T.create_node(id, parent);
            }

            continue;
        }

        if (args[0] == "remove" && args.size() == 2) {
            
            int id = std::stoi(args[1]);

            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result != "Ok: 1") {
                std::cout << "Error: Not found " << id << "\n";

                continue;
            }
            
            msg_s = "remove " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result.substr(0, 2) == "Ok") {
                T.delete_node(id);
            }
            

            continue;
        }

        if (args[0] == "exec" && args.size() == 3) {
            
            int id = std::stoi(args[1]);
            std::string var_name = args[2];

            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result != "Ok: 1") {
                std::cout << "Error: Not found " << id << "\n";

                continue;
            }

            msg_s = "get_value " + std::to_string(id) + " " + var_name;
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            continue;
        }

        if (args[0] == "exec" && args.size() == 4) {
            
            int id = std::stoi(args[1]);
            std::string var_name = args[2];
            int value = std::stoi(args[3]);

            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);

            if (result != "Ok: 1") {
                std::cout << "Error: Not found " << id << "\n";

                continue;
            }

            msg_s = "set_value " + std::to_string(id) + " " + var_name + " " + std::to_string(value);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);


            continue;
        }

        if (args[0] == "heartbit") {
            int time;
            std::cin >> time;
            
            continue;
        }

        if (args[0] == "exit") {

            msg_s = "kill";
            send_message(main_socket, msg_s);
            std::cout << "Ok\n";
            flag = false;
            continue;
        }
    }


    return EXIT_SUCCESS;
}
