#ifndef SERVER_HH
#define SERVER_HH
#include <memory>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>

class server
{
  private:
	std::int32_t port, max_connections, message_length;
	fd_set master; // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	struct sockaddr_in myaddr;// server address
	struct sockaddr_in remoteaddr; // client address
	std::int32_t fdmax;// maximum file descriptor number
	std::uint32_t listener; // listening socket descriptor
	std::shared_ptr<char> message_buffer;
  public:
	server(std::int32_t port, std::uint32_t max_connections, std::uint32_t message_length);
	void run();;
	std::int32_t send();
	std::int32_t recieve();
};
std::int32_t create_socket(std::int32_t domain, std::int32_t type, std::int32_t protocol);
std::int32_t set_socket_reusable(int socket, int level, int option_name, const void * option_value, socklen_t option_length);
std::int32_t bind_socket(std::int32_t socket, const struct sockaddr address, socklen_t address_length);
#endif // end server.hh
