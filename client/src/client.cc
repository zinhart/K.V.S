//#include "common"
#include<iostream>
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
enum class ACTIONS : std::uint8_t {DEFAULT = 0, GET, PUT, DELETE};
int main(int argc, char *argv[])
{

  if(argc <= 3)
  {
	std::cerr<<"USAGE: ./client <hostname> <portnum> <actions>\n";
	std::cerr<<"ACTIONS: <GET> key\n<PUT> <key> <value>\n<Delete> <key> <value>\n";
	std::exit(1);
  }
  std::int32_t error, i;
  //std::uint16_t port;
  //ACTIONS action = ACTIONS::GET;
  char * hostname,* port,* action,* key, * value;
  hostname = argv[1];
  port = argv[2];
  action = argv[3];
  key = argv[4];
  value = (argc <= 4) ? NULL : argv[5];


  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
  
  memset(&hints, 0, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) 
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
  freeaddrinfo(servinfo); // all done with this structure
  if(strcmp(action,"GET") == 0)
  {
	//send key
	if( (numbytes = sendto(sockfd, "GET", strlen("GET"), 0, p->ai_addr, p->ai_addrlen)) == -1)
	{
	  perror("sendto");
	  std::exit(1);
	}	  
	//if key is existing
	
	//recieve value
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) 
	{
	  perror("recv");
	  exit(1);
	}
	buf[numbytes] = '\0';
	printf("client: received '%s'\n",buf);
  }
  else if(strcmp(action,"PUT") == 0)
  {
	//send key
	
	//if key exists
	
	//set value
  }
  else if(strcmp(action,"DELETE") == 0)
  {
	//send key
	
	//if key exists
	
	//delete value
  }
  close(sockfd);
  return 0;
}

