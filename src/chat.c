#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/chat.h"

void start_chat(int server_fd) {
  struct pollfd fds[2];
  int client1;
  int client2;

  struct sockaddr_in client_adrr;
  // socklen_t     msg_namelen     size of address
  socklen_t client_len = sizeof(client_adrr);

  // listening socket // client info // size of structure
  client1 = accept(server_fd, (struct sockaddr *)&client_adrr, &client_len);

  if (client1 == -1) {
    err(EXIT_FAILURE, "accept \n");
  }

  printf("client 1 connected \n");

  client2 = accept(server_fd, (struct sockaddr *)&client_adrr, &client_len);

  if (client2 == -1) {
    err(EXIT_FAILURE, "accept");
  }

  printf("client 2 connected \n");

  // fds stores the sockets tha poll needs to whatch
  fds[0].fd = client1;
  fds[0].events = POLLIN;
  fds[0].revents = 0;

  fds[1].fd = client2;
  fds[1].events = POLLIN;
  fds[1].revents = 0;

  // Number of users && wait indefenetly
  nfds_t nfds = 2;
  int timeout = -1;

  while (1) {
    printf("Waiting for responses \n");

    int rc = poll(fds, nfds, timeout);

    if (rc == -1) {
      // errno is used to report and identify the specific reaseon why a system
      // or a library failed
      if (errno == EINTR) {
        continue;
      }

      perror("poll");
      break;
    }

    // type to represent quantitys of files descriptors used for pool
    for (nfds_t i = 0; i < nfds; i++) {
      char buffer[1024];
      if (fds[i].revents & POLLIN) {
        printf("Client %d as data Available\n", i + 1);
        int destination = 1 - i;

        // the -1 means the recv reads 1023 bytes and lets the last byte free
        ssize_t received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

        if (received > 0) {
          send(fds[destination].fd, buffer, received, 0);
        } else if (received == 0) {
          printf("Client %d disconnected", i + 1);
          close(client1);
          close(client2);

          return;
        } else {
          err(EXIT_FAILURE, "Data Error");
        }
      }

      if (fds[i].revents & POLL_HUP) {
        printf("Client %d as disconected \n", i + 1);
        break;
      }

      if (fds[i].revents & (POLL_ERR | POLLNVAL)) {
        printf("Error on client %d \n", i + 1);
        break;
      }
    }
  }
}
