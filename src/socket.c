// deal with barkeley sockets
#include "../include/socket.h"
#include <arpa/inet.h>
#include <err.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// AF_INET adress family constand used to designed IPv4 protocols
#define MYPORT 8080

int connection(void) {
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET; // IPv4 adress
  my_addr.sin_port = htons(MYPORT);
  /*
   * field inside IPv4, holds the IP host adress, acess the
   * actual 32bit IPv4 adress via sub-field (s_addr)
   * */
  my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // sockfd is the listening socket
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    err(EXIT_FAILURE, "socket");
  }

  // each arguments specifies a term, 8 means the number of bytes to set
  memset(&my_addr.sin_zero, '\0', 8);

  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
    err(EXIT_FAILURE, "bind");
  }

  if (listen(sockfd, 10) == -1) {
    err(EXIT_FAILURE, "listen");
  }

  printf("Server listening on 127.0.0.1: %d\n", MYPORT);

  return sockfd;
}
