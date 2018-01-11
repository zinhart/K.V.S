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
	std::uint32_t port, max_connections, message_length;
	fd_set master; // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	struct sockaddr_in myaddr;// server address
	struct sockaddr_in remoteaddr; // client address
	std::int32_t fdmax;// maximum file descriptor number
	std::int32_t listener; // listening socket descriptor
	std::shared_ptr<char> message_buffer;
  public:
	server(std::uint32_t port, std::uint32_t max_connections, std::uint32_t message_length);
	void run();
};
#endif // end server.hh
