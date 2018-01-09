#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#define PORT 3000
int main(int argc, char **argv)
{
  if(argc <= 1)
  {
	std::cout<<"Usage ./server -p <PORTNUM> -m <MESSAGELENGTH> -c <MAXCONNECTIONS>\n";
	std::exit(1);
  }
  else
  {
	std::uint32_t portnum, max_connections, message_length;
	std::int32_t i, j;
	int invalid_option;
	for(std::uint32_t i = 1, invalid_option = 0; i < argc; ++i)
	{
	  if(strcmp(argv[i],"-p") == 0)
	  {
 		portnum = std::atoi(argv[i + 1]);	
	  }
	  else if(strcmp(argv[i],"-c") == 0)
	  {
 		max_connections = std::atoi(argv[i + 1]);	
	  }
	  else if(strcmp(argv[i],"-m") == 0)
	  {
 		max_connections = std::atoi(argv[i + 1]);	
	  }
	  else
		invalid_option = 1;
	  if(invalid_option == 1)
		std::exit(1);

	}
	std::cout<<portnum<<" "<<max_connections<<" "<<message_length<<"\n";
   
	fd_set master; // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	struct sockaddr_in myaddr;// server address
	struct sockaddr_in remoteaddr; // client address
	std::int32_t fdmax;// maximum file descriptor number
	std::int32_t listener; // listening socket descriptor
	std::int32_t newfd; // newly accept()ed socket descriptor
	char buf[message_length];
	// buffer for client data
	std::int32_t nbytes;
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
	myaddr.sin_port = htons(PORT);
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
  
  
  /*
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
	for(i = 0; i <= fdmax; i++) 
	{
	  if (FD_ISSET(i, &read_fds)) 
	  { // we got one!!
		if (i == listener) 
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
		  // handle data from a client
		  if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) 
		  {
			// got error or connection closed by client
			if (nbytes == 0) 
			{
			  // connection closed
			  printf("selectserver: socket %d hung up\n", i);
			} 
			else 
			{
			  perror("recv");
			}
			close(i); // bye!
			FD_CLR(i, &master); // remove from master set
		  } 
		  else 
		  {
			// we got some data from a client
			for(j = 0; j <= fdmax; j++) 
			{
			  // send to everyone!
			  if (FD_ISSET(j, &master)) 
			  {
				// except the listener and ourselves
				if (j != listener && j != i) 
				{
				  if (send(j, buf, nbytes, 0) == -1) 
				  {
					perror("send");
				  }
				}
			  }
			}
		  }
		}
	  }
	}
  }*/
  return 0;
}
