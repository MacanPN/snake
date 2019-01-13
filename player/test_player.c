#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "server.h"

int n=0;
int last_dir=RIGHT;

reply get_reply(payload *p){
  // define reply variable
  reply r;
  r.direction = RIGHT;
  return r;
}
