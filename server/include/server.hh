#ifndef SERVER_HH
#define SERVER_HH
#include<sys/socket.h>
#include <iostream>
std::int32_t create_socket(std::int32_t domain, std::int32_t type, std::int32_t protocol);
std::int32_t set_socket_reusable(int socket, int level, int option_name, const void * option_value, socklen_t option_length);
std::int32_t bind_socket(std::int32_t socket, const struct sockaddr address, socklen_t address_length);
#endif // end server.hh
