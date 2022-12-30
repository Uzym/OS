#include "../include/zmq_manage.h"

int main(int argc, char const *argv[])
{
    int id = atoi(argv[1]);
    int parent_port = atoi(argv[2]);

    //std::cout << "client start with " << id << ' ' << parent_port << '\n';

    std::map <std::string, int> node_map;

    zmq::context_t context(3);
    zmq::socket_t parent_socket(context, zmq::socket_type::rep);
    parent_socket.connect(get_port_name(parent_port));

    std::vector <zmq::socket_t> childs;

    std::string result, msg_str; 

    bool flag = true;
    while (flag) {

        std::string request_string = recieve_message(parent_socket);
        std::istringstream command_stream(request_string);
        std::string command;

        result = "", msg_str = "";

        //std::cout << "log " << id << ": " << request_string << '\n';

        command_stream >> command;

        if (command == "ping") {
            
            int id_search;
            command_stream >> id_search;    

            if (id_search == id) {

                msg_str = "Ok: 1";
                send_message(parent_socket, msg_str);

                continue;
            }


            bool flag_succes = false;
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], request_string);
                result = recieve_message(childs[i]);
                
                if (result == "Ok: 1") {
                    send_message(parent_socket, result);
                    flag_succes = true;
                    break;
                }
            }
            if (!flag_succes) {
                send_message(parent_socket, "Ok: 0");
            }

            continue;
        }

        if (command == "pid") {

            msg_str = "Ok: " + std::to_string(getpid());
            send_message(parent_socket, msg_str);

            continue;
        }

        if (command == "create") {

            int child_id, parent_id;
            command_stream >> child_id >> parent_id;

            if (parent_id == id) {

                zmq::socket_t tmp(context, zmq::socket_type::req);
                tmp.bind(get_port_name(BASE_PORT + child_id + 1));

                pid_t child_pid = fork();
                if (child_pid == -1) {
                    
                    msg_str = "Error: Fork error";
                    send_message(parent_socket, msg_str);

                    continue;

                } else if (child_pid == 0) {
                    create_node(child_id, BASE_PORT + child_id + 1);
                } else {
                    send_message(tmp, "pid");
                    result = recieve_message(tmp);
                    send_message(parent_socket, result);

                    childs.push_back(std::move(tmp));

                    continue;
                }
            }

            bool flag_succes = false;
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], request_string);
                result = recieve_message(childs[i]);
                
                if (result.substr(0, 2) == "Ok") {
                    send_message(parent_socket, result);
                    flag_succes = true;
                    break;
                }
            }
            if (!flag_succes) {
                send_message(parent_socket, "Error: Not found");
            }

            continue;
        }

        if (command == "remove") {
            int remove_id;
            command_stream >> remove_id;

            bool flag_get_remove_id = false;
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], "get_id");
                result = recieve_message(childs[i]);
                
                if (result == std::to_string(remove_id) + " ") {
                    
                    send_message(childs[i], "kill");
                    sleep(1);
                    childs[i].unbind(get_port_name(BASE_PORT + remove_id + 1));
                    childs.erase(std::next(childs.begin() + i));
                    flag_get_remove_id = true;

                    break;
                }
            }
            if (flag_get_remove_id) {
                send_message(parent_socket, "Ok");
                continue;
            }

            bool flag_succes = false;
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], request_string);
                result = recieve_message(childs[i]);
                
                if (result.substr(0, 2) == "Ok") {
                    send_message(parent_socket, result);
                    flag_succes = true;
                    break;
                }
            }
            if (!flag_succes) {
                send_message(parent_socket, "Error: Not found");
            }

            continue;
        }

        if (command == "kill") {
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], "get_id");
                result = recieve_message(childs[i]);

                send_message(childs[i], "kill");
                childs[i].unbind(get_port_name(BASE_PORT + std::stoi(result) + 1));
            }

            parent_socket.disconnect(get_port_name(parent_port));
            flag = false;

        }

        if (command == "get_value") {

            int id_map;
            std::string var_name;
            command_stream >> id_map >> var_name;

            if (id_map == id) {
                
                if (node_map.count(var_name) < 1) {
                    
                    msg_str = "Ok:" + std::to_string(id) + ": '" + var_name + "' not found";
                    send_message(parent_socket, msg_str);

                    continue;
                }

                msg_str = "Ok:" + std::to_string(id) + ": " + std::to_string(node_map[var_name]);
                send_message(parent_socket, msg_str);

                continue;
            }

            bool flag_succes = false;
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], request_string);
                result = recieve_message(childs[i]);
                
                if (result.substr(0, 2) == "Ok") {
                    send_message(parent_socket, result);
                    flag_succes = true;
                    break;
                }
            }
            if (!flag_succes) {
                send_message(parent_socket, "Error: Not found");
            }

            continue;
        }

        if (command == "set_value") {

            int id_map, value;
            std::string var_name;
            command_stream >> id_map >> var_name >> value;

            if (id_map == id) {
                
                node_map[var_name] = value;

                msg_str = "Ok: " + std::to_string(id);
                send_message(parent_socket, msg_str);

                continue;
            }

            bool flag_succes = false;
            for (int i = 0; i < childs.size(); i ++) {

                send_message(childs[i], request_string);
                result = recieve_message(childs[i]);
                
                if (result.substr(0, 2) == "Ok") {
                    send_message(parent_socket, result);
                    flag_succes = true;
                    break;
                }
            }
            if (!flag_succes) {
                send_message(parent_socket, "Error: Not found");
            }

            continue;
        }

        if (command == "get_id") {
            
            msg_str = std::to_string(id) + " ";
            send_message(parent_socket, msg_str);
            
            continue;
        }

    }

    for (int i = 0; i < childs.size(); i ++) {
        childs[i].close();
    }
    parent_socket.close();

    return EXIT_SUCCESS;
}

