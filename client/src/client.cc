//#include "common"
#include <iostream>
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "4000" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) 
  {
	return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char *argv[])
{

  if(argc <= 4)
  {
	std::cerr<<"USAGE: ./client <hostname> <portnum> <cmds>\n";
	std::cerr<<"ACTIONS: <GET> key\n<PUT> <key> <value>\n<Delete> <key> <value>\n";
	std::exit(1);
  }
  std::int32_t error, i;
  std::string packet_buffer;
  std::uint32_t packet_length;
  
  char * host_name, * port, * cmd, * key, * value;
  host_name = argv[1];
  port = argv[2];
  cmd = argv[3];
  key = argv[4];
  value = (argc <= 4) ? NULL : argv[5];
  std::int32_t sockfd, numbytes;  		
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
  memset(&hints, 0, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(host_name, port, &hints, &servinfo)) != 0) 
  {
	//std::cerr<<"getaddrinfo: %s\n", gai_strerror(rv)<<"\n";
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	return 1;
  }
  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) 
  {
	if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
	{
	  perror("client: socket");
	  continue;
	}
	if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
	{
	  perror("client: connect");
	  close(sockfd);
	  continue;
	}
	break;
  }
  if (p == NULL) 
  {
	fprintf(stderr, "client: failed to connect\n");
	return 2;
  }
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  printf("client: connecting to %s\n", s);
  std::uint32_t size = 1 + strlen(cmd) + strlen(key) + (value == NULL) ? 0 : strlen(value);
  char packet[size];
  strcpy(packet,cmd);
  strcpy(packet+ strlen(cmd), key);
  (value == NULL) ? 0  : strcpy(packet+ strlen(key), value) ; 
  freeaddrinfo(servinfo); // all done with this structur
  printf("packet: %s %s %s length %d\n", packet, cmd, key,strlen(packet));
  //send command, key, and possibly value
  if( (numbytes = sendto(sockfd, packet, strlen(packet), 0, p->ai_addr, p->ai_addrlen)) == -1)
  {
	std::cerr<<"sendto\n";
	std::exit(1);
  }	
  //recieve value strlen(cmd) determins the number of bytes to wait for and must be coordinated with what the server sends
  if ((numbytes = recv(sockfd, packet, strlen(packet), 0)) == -1) 
  {
	std::cerr<<"recv\n";
	std::exit(1);
  }
  packet[numbytes] = '\0';
  printf("client: received '%s'\n", packet);
  close(sockfd);
  return 0;
}

