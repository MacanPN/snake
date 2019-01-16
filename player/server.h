#ifndef SERVER_H
#define SERVER_H

#define FIELD_WIDTH 50
#define FIELD_HEIGHT 35
#define MAX_SNAKE_LEN 300

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

#define EMPTY 0
#define FRUIT 1
#define ROCK 9

#include <inttypes.h>

typedef struct payload_t {
    uint16_t snake_len;
    uint16_t fx, fy;
    uint16_t pos[MAX_SNAKE_LEN][2];
    uint16_t map[FIELD_WIDTH][FIELD_HEIGHT];
} payload;

typedef struct reply_t {
  uint16_t direction;
} reply;

reply get_reply(payload *p);

/*
 * This function prints map in plain text format
 * */
void print_map(uint16_t map[FIELD_WIDTH][FIELD_HEIGHT]);

/*
 * This function prints the snake coordinates
 * The head of snake is the last coordinate
 * */
void print_snake(uint16_t snake_len, uint16_t pos[MAX_SNAKE_LEN][2]);


#endif
