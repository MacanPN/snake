#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
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
  
  int dist[FIELD_WIDTH][FIELD_HEIGHT]={INT_MAX};
  int queue[FIELD_WIDTH*FIELD_HEIGHT][2];
  qb = 0;
  qe = 1;
  queue[0][0] = hx;
  queue[0][1] = hy;
  int dx = {1,-1,0,0};
  int dy = {0,0,-1,1};
  while(){
    if(p->map[queue[qb][0]][queue[qb][1]] <= 1)
  }
  
  
  
  
  r.direction = UP;
  return r;
}
