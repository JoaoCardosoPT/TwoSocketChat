#include <arpa/inet.h>
#include <err.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "include/socket.h"

// TO DO : Understand the errors of the ip
#define DEST_IP "127.0.0.1"
#define DEST_PORT 8080

int main() {
  connection();
  int sockfd;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  // dest adress
  struct sockaddr_in dest_addr;

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(DEST_PORT);

  if (DEST_IP == 0) {
    err(EXIT_FAILURE, "Error On Port Or Ip");
  }

  dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);

  memset(&(dest_addr.sin_zero), '\0', 8);

  if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) ==
      -1) {
    perror("connect");
    return -1;
  }
}
