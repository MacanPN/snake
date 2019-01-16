#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "server.h"

reply get_reply(payload *p){
  // define reply variable
  reply r;
  // print map (just to see what have we got)
  print_map(p->map);
  // print the snake coordinates
  print_snake(p->snake_len, p->pos);
  
  // hx and hy are coordinates of the head of snake
  int hx = p->pos[(p->snake_len)-1][0];
  int hy = p->pos[(p->snake_len)-1][1];
  p->map[p->fx][p->fy] = EMPTY;
  printf("hx:%d hy:%d fx:%d fy:%d\n",hx, hy, p->fx, p->fy);
  
  // AK je ovocie vlavo A vlavo je prazdno
  if((p->fx < hx) && (p->map[hx-1][hy]==EMPTY)){
    r.direction = LEFT;
    return r;
  }
  // AK je ovocie vpravo A vpravo je volno
  if((p->fx > hx) && (p->map[hx+1][hy]==EMPTY)){
    r.direction = RIGHT;
    return r;
  }
  // AK je ovocie hore A hore je volno
  if((p->fy < hy) && (p->map[hx][hy-1]==EMPTY)){
    r.direction = UP;
    return r;
  }
  if((p->fy > hy) && (p->map[hx][hy+1]==EMPTY)){
    r.direction = DOWN;
    return r;
  }
  
  if(p->map[hx-1][hy]==EMPTY){
    r.direction = LEFT;
    return r;
  }
  if(p->map[hx+1][hy]==EMPTY){
    r.direction = RIGHT;
    return r;
  }
  if(p->map[hx][hy-1]==EMPTY){
    r.direction = UP;
    return r;
  }
  if(p->map[hx][hy+1]==EMPTY){
    r.direction = DOWN;
    return r;
  }
  r.direction = UP;
  return r;
}
