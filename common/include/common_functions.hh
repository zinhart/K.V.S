#ifndef COMMON_FUNCTIONS_HH
#define COMMON_FUNCTIONS_HH
#include <sys/types.h> 
#include <sys/socket.h>
//pg 35 beej
int sendall(int s, char *buf, int *len);
#endif
