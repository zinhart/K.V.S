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
#include <map>
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
	std::multimap<std::shared_ptr<char>, std::shared_ptr<char>> database;
  public:
	server(std::uint32_t port, std::uint32_t max_connections, std::uint32_t message_length);
	void run();

	//resources(keys)
	void get();//return all keys
	void put(std::multimap<std::shared_ptr<char>, std::shared_ptr<char>> & another_database);// replace all keys and values with another set
	void post(std::shared_ptr<char> & key);//create a new key
	void erase();//delete all keys
	//elements
	void get(std::shared_ptr<char> & key);//returns all values associated with key
	void put(std::shared_ptr<char> & key, std::shared_ptr<char> & value); //replace the value associated with this key or create a key with this value
	void patch(std::shared_ptr<char> & key, std::shared_ptr<char> & value);
	void erase(std::shared_ptr<char> & key, std::shared_ptr<char> & value);//delete a value associated with a key



	};
#endif // end server.hh
