#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/chat.h"

struct Client {
  char username[100];
  bool has_username;
};

void start_chat(int server_fd) {
  // array of structures used by poll
  struct pollfd fds[2];
  // Initalize every field to zero
  struct Client clients[2] = {0};

  struct sockaddr_in client_adrr;
  // socklen_t     msg_namelen     size of address
  socklen_t client_len = sizeof(client_adrr);

  // listening socket // client info // size of structure
  fds[0].fd = accept(server_fd, (struct sockaddr *)&client_adrr, &client_len);
  if (fds[0].fd == -1) {
    err(EXIT_FAILURE, "accept \n");
  }
  const char *userask = "Choose an Username ";
  send(fds[0].fd, userask, strlen(userask), 0);

  printf("Client connected \n");

  ssize_t received = recv(fds[0].fd, clients[0].username, sizeof(clients[0].username) - 1, 0);

  if (received > 0) {
      clients[0].username[received] = '\0';
      clients[0].username[strcspn(clients[0].username, "\r\n")] = '\0';

      clients[0].has_username = true;

      printf("Username set to %s\n", clients[0].username);
  }

  fds[1].fd = accept(server_fd, (struct sockaddr *)&client_adrr, &client_len);
  send(fds[1].fd, userask, strlen(userask), 0);

  if (fds[1].fd == -1) {
    err(EXIT_FAILURE, "accept");
  }

  printf("Client connected \n");

  // fds stores the sockets tha poll needs to whatch
  fds[0].events = POLLIN;
  fds[0].revents = 0;

  fds[1].events = POLLIN;
  fds[1].revents = 0;

  // Number of users && wait indefenetly
  nfds_t nfds = 2;
  int timeout = -1;

  while (1) {
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
        int destination = 1 - i;

        // the -1 means the recv reads 1023 bytes and lets the last byte free
        ssize_t received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

        if (received > 0) {
          if (clients[i].has_username == false) {
            int tocopy = received;

            if (tocopy > 99) {
              err(EXIT_FAILURE, "username");
            }

            memcpy(clients[i].username, buffer, tocopy);
            clients[i].username[tocopy] = '\0';

            clients[i].has_username = true;


            printf("Username set to %s", clients[i].username);


            char message[60];
            // creates a formated text and stores it in a char array
            int show_user = snprintf(message, sizeof(message),
                                     "%s connected", clients[i].username);
            send(fds[i].fd, message, (size_t)message, 0);

          } else {
            send(fds[destination].fd, buffer, received, 0);
          }
        } else if (received == 0) {
          printf("Client %d disconnected", i + 1);
          close(fds[0].fd);
          close(fds[1].fd);

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
