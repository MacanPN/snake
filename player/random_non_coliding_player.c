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
  
  // is the direction we've selected Ok?
  int tries=0;
  do{
    // select random direction <0-3>
    r.direction = rand()%4;
    printf("TRYING:%d\n",r.direction);
    // find out how much would we move in x and y direction, if we moved in selected direction
    int dx,dy;
    if(r.direction == LEFT) { dx=-1; dy=0; }
    if(r.direction == RIGHT){ dx= 1; dy=0; }
    if(r.direction == UP   ){ dx= 0; dy=-1; }
    if(r.direction == DOWN) { dx= 0; dy= 1; }
    printf("hx: %d hy: %d dx: %d dy: %d\n",hx,hy,dx,dy);
       // if the new position of head is inside the playing field
    if((hx+dx >= 0) && (hx+dx < FIELD_WIDTH)&&
       (hy+dy >= 0) && (hy+dy < FIELD_HEIGHT)&&
      // and map there is empty 
        (p->map[hx+dx][hy+dy]==EMPTY)){
      // then the direction is Ok to move in
      printf("volim smer: %d\n",r.direction);
      return r;
    }
    tries++;
  }while(tries<20);
  return r;
}
