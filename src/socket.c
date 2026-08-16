#include "../include/socket.h"
#include <arpa/inet.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// AF_INET adress family constand used to designed IPv4 protocols
#define MYPORT 8080

// Holds scoked addr info
struct socketadress {
  unsigned short sa_family;
  unsigned short sa_data[14];
};

// A structure that helps reference the elements of the socket
struct socketels {
  short int sin_family;
  unsigned short int sin_port;
  struct in_addr sin_addr;
  unsigned char sin_zero[8];
};

struct in_adr {
  unsigned short s_addr;
};

int connection(void) {
  struct socketels sockel;
  struct sockaddr_in my_addr;

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(MYPORT);

  int sockfd;
  int backlog = 128;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    err(EXIT_FAILURE, "socket");
  }

  inet_aton("127.0.0.1", &my_addr.sin_addr);

  memset(&my_addr.sin_zero, '\0', 8);

  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) ==
      -1) {
    err(EXIT_FAILURE, "bin");
  }

  listen(sockfd, backlog);

  if (listen(sockfd, backlog)) {
    printf("Errors Listening");
    return -1;
  }

  printf("IP: %s\n", inet_ntoa(my_addr.sin_addr));

  return sockfd;
}
