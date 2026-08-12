#include <arpa/inet.h>
#include <stdio.h>
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

int main() {
  struct socketels sockel;
  struct sockaddr_in my_addr;

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(MYPORT);

  int sockfd;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  inet_aton("10.0.0.5", &my_addr.sin_addr);

  memset(&my_addr.sin_zero, '\0', 8);

  printf("IP: %s\n", inet_ntoa(my_addr.sin_addr));
}
