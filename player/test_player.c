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
  
  r.direction = RIGHT;
  return r;
}


// balazovic petricek
