
#ifndef LAB6_ZMQ_MANAGE_H
#define LAB6_ZMQ_MANAGE_H

#include <string>
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <set>
#include <map>

const int API_PORT = 5040;
const int BASE_PORT = 5050;
const int WAIT_TIME = 1000;
const int manage_worker_id = -1;

std::string get_port_name(const int port);
bool send_message(zmq::socket_t &socket, const std::string &message_string);
std::string recieve_message(zmq::socket_t &socket);
void create_node(int id, int port);

#endif

