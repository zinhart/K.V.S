#include "server.hh"
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

server::server(std::uint32_t prt, std::uint32_t max_connecs, std::uint32_t msg_len)
  :port(prt), max_connections(max_connecs), message_length(msg_len)
{
  std::cout<<"Port: "<<port<<" Max Clients"<<max_connections<<" Message Length"<<message_length<<"\n";
  message_buffer = std::shared_ptr<char>(new char[message_length], std::default_delete<char[]>());
  // buffer for client data
  std::int32_t yes = 1;
  // for setsockopt() SO_REUSEADDR, below
  unsigned int addrlen;
// clear the master and temp sets
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
// get the listener
  if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
  {
	std::cerr<<"Error socket\n";
	//perror("socket");
	std::exit(1);
  }
  // lose the pesky "address already in use" error message
  if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) 
  {
	std::cerr<<"error setsockopt\n";
	//perror("setsockopt");
	 std::exit(1);
  }
  // bind
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = INADDR_ANY;
  myaddr.sin_port = htons(port);
  memset(&(myaddr.sin_zero), '\0', 8);
  if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) 
  {
	std::cerr<<"error bind\n";
  	//perror("bind");
	exit(1);
  }
  // listen
  if (listen(listener, 10) == -1) 
  {
	std::cerr<<"error litsen\n";
	//perror("listen");
	 std::exit(1);
  }
  // add the listener to the master set
  FD_SET(listener, &master);
  // keep track of the biggest file descriptor
  fdmax = listener; // so far, it’s this one
}
void server::run()
{
	std::int32_t current_file_descriptor;
	std::int32_t j;

	std::int32_t newfd; // newly accept()ed socket descriptor
	// buffer for client data is shared_ptr
	std::int32_t nbytes;
	std::int32_t yes = 1;
	// for setsockopt() SO_REUSEADDR, below
	unsigned int addrlen;
 	// main loop
	for(;;) 
	{
	  read_fds = master; // copy it
	  if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) 
	  {
		perror("select");
		exit(1);
	  }
	  // run through the existing connections looking for data to read
	  for(current_file_descriptor = 0; current_file_descriptor <= fdmax; ++current_file_descriptor) 
	  {
		if (FD_ISSET(current_file_descriptor, &read_fds)) 
		{ // we got one!!
		  if (current_file_descriptor == listener) 
		  {
			// handle new connections
			addrlen = sizeof(remoteaddr);
			if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) 
			{
			  perror("accept");
			} 
			else 
			{
			  FD_SET(newfd, &master); // add to master set
			  if (newfd > fdmax) 
			  {
				// keep track of the maximum
				fdmax = newfd;
			  }
			  printf("selectserver: new connection from %s on socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
			}
		  } 
		  else 
		  {
			// when recv returns <= 0 data stopped being recieved or their was an error with recv
			if ((nbytes = recv(current_file_descriptor, message_buffer.get(), sizeof(message_buffer.get()), 0)) <= 0) 
			{
			  // got error or connection closed by client
			  if (nbytes == 0) 
			  {
				// connection closed
				printf("selectserver: socket %d hung up\n", current_file_descriptor);
			  } 
			  else 
			  {
				perror("recv");
			  }
			  close(current_file_descriptor); // bye!
			  FD_CLR(current_file_descriptor, &master); // remove from master set
			} 
			else 
			{
			  printf("Data from client: %s \n", message_buffer.get());
			  // we got some data from a client
			  for(j = 0; j <= fdmax; j++) 
			  {
				//if the current file descriptor (j) is in the master set
				if (FD_ISSET(j, &master)) 
				{
				  //j == litsener is the master socket j==i is the current open connection
				  if (/*j == listener &&*/ j == current_file_descriptor) 
				  {
														std::cout<<"Here in send\n";
					if (send(j, message_buffer.get(), nbytes, 0) == -1) 
					{
					  perror("send");
					}
				  }
				}
			  }
			}
		  }//end handle data from client
		}//end ne incoming connection
	  }//end loop file descriptors
	}//end for

}

int main(int argc, char **argv)
{
  if(argc <= 3)
  {
	std::cout<<"Usage ./server <PORTNUM> <MESSAGELENGTH> <MAXCONNECTIONS>\n";
	std::exit(1);
  }
  else
  {
	std::uint32_t port, max_connections, message_length;
	std::int32_t i, j;
 	port = std::atoi(argv[1]);	
 	message_length = std::atoi(argv[2]);	
 	max_connections = std::atoi(argv[3]);	
 	server s(port, max_connections, message_length );
		s.run();
  }
  
  return 0;
} 
