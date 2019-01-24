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
  int fx = p->fx;
  int fy = p->fy;
  p->map[p->fx][p->fy] = EMPTY;
  printf("hx:%d hy:%d fx:%d fy:%d\n",hx, hy, fx, fy);
  
  int dist[FIELD_WIDTH][FIELD_HEIGHT];
  
  for(int i=0; i<FIELD_WIDTH; i++){
    for(int j=0; j<FIELD_HEIGHT; j++){
      dist[i][j] = INT_MAX;
    }
  }
  dist[hx][hy] = 0;
  
  int qx[FIELD_WIDTH*FIELD_HEIGHT];
  int qy[FIELD_WIDTH*FIELD_HEIGHT];
  int qb = 0;
  int qe = 0;
  qx[0] = hx;
  qy[0] = hy;
  
  while(qb<=qe){
    int ax = qx[qb];
    int ay = qy[qb];
    if((ax < FIELD_WIDTH-1)
       &&(p->map[ax+1][ay] <= 1) // ak napravo od aktivneho pola je volno
       &&(dist[ax+1][ay] > dist[ax][ay]+1)){ // A vieme sa tam dostat na viacej krokov ako N+1
        dist[ax+1][ay] = dist[ax][ay]+1;
        qe++;
        qx[qe] = ax+1;
        qy[qe] = ay;
    }
    if((ax > 0)
       &&(p->map[ax-1][ay] <= 1) // ak vlavo od aktivneho pola je volno
       &&(dist[ax-1][ay] > dist[ax][ay]+1)){ // A vieme sa tam dostat na viacej krokov ako N+1
        dist[ax-1][ay] = dist[ax][ay]+1;
        qe++;
        qx[qe] = ax-1;
        qy[qe] = ay;
    }
    if((ay < FIELD_HEIGHT-1)
       &&(p->map[ax][ay+1] <= 1) // ak dole od aktivneho pola je volno
       &&(dist[ax][ay+1] > dist[ax][ay]+1)){ // A vieme sa tam dostat na viacej krokov ako N+1
        dist[ax][ay+1] = dist[ax][ay]+1;
        qe++;
        qx[qe] = ax;
        qy[qe] = ay+1;
    }
    if((ay > 0)
       &&(p->map[ax][ay-1] <= 1) // ak hore od aktivneho pola je volno
       &&(dist[ax][ay-1] > dist[ax][ay]+1)){ // A vieme sa tam dostat na viacej krokov ako N+1
        dist[ax][ay-1] = dist[ax][ay]+1;
        qe++;
        qx[qe] = ax;
        qy[qe] = ay-1;
    }
    if(dist[fx][fy] < INT_MAX){
      printf("Vzdialenost: %d\n",dist[fx][fy]);
      break;
    }
    qb++; // posun zaciatok fronty (to co je teraz na zaciatku sme uz spracovali)
  }
  
  if(dist[fx][fy] == INT_MAX){ // k ovoci sa neda dostat
    r.direction = RIGHT;
    return r;
  }
  
  int came_from;
  int x=fx;
  int y=fy;
  int lx=x;
  int ly=y;
  while((x!=hx)||(y!=hy)){
    if((dist[x-1][y] == dist[x][y]-1)&&(x>0)){
      x = x-1;
      came_from = RIGHT;
    }
    if((dist[x+1][y] == dist[x][y]-1)&&(x<FIELD_WIDTH)){
      x = x+1;
      came_from = LEFT;
    }
    if((dist[x][y+1] == dist[x][y]-1)&&(y<FIELD_HEIGHT)){
      y = y+1;
      came_from = UP;
    }
    if((dist[x][y-1] == dist[x][y]-1)&&(y>0)){
      y = y-1;
      came_from = DOWN;
    }
    printf("X: %d, Y: %d\n",x,y);
    if((lx==x)&&(ly==y))
      break;
    lx = x; ly = y;
  }
/*  for(int i=0; i<FIELD_HEIGHT; i++){
    for(int j=0; j<FIELD_WIDTH; j++){
      printf("%d,",dist[j][i]);
    }
    printf("\n");
  }*/

  
  printf("sending: %d",came_from);
  r.direction = came_from;
  return r;
}
