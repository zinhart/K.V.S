#include "common_functions.hh"
int sendall(int s, char *buf, int *len)
{
  int total = 0;
  // how many bytes we’ve sent
  int bytesleft = *len; // how many we have left to send
  int n;
  while(total < *len) 
  {
	n = send(s, buf+total, bytesleft, 0);
	if (n == -1) { break; }
	total += n;
	bytesleft -= n;
  }
  *len = total; // return number actually sent here
  return n == -1 ?- 1 : 0; // return -1 on failure, 0 on success
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
	return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
