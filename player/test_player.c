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
  int dlzka = p->snake_len;
  int hx = p->pos[dlzka-1][0];
  int hy = p->pos[dlzka-1][1];
  
  int fy = p->fy;
  int fx = p->fx;
  
  printf("\n\n hy: %d fy:%d \n\n\n",hy,fy);
  
  if(hy<fy){
    r.direction = DOWN;
  }
  if(hy>fy){
    r.direction = UP;
  }
  
  if(hx>fx){
    r.direction = LEFT;
  }
  if(hx<fx){
    r.direction = RIGHT;
  }
  
  if(r.direction == RIGHT){
    if(p->map[hx+1][hy] == EMPTY){
      return r;
    }else{
      r.direction = DOWN;
    }
  }
  
  if(r.direction == DOWN){
    if(p->map[hx][hy+1] == EMPTY){
      return r;
    }else{
      r.direction = LEFT;
    }
  }
  return r;
}


// balazovic petricek
