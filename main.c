#include "include/chat.h"
#include "include/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int serverfd = connection();
  start_chat(serverfd);

  return 0;
}
