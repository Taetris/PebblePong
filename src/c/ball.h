#pragma once

#include "player.h"

typedef struct Ball {
  int16_t x;
  int16_t y;
  int16_t directionX;
  int16_t directionY;
  int16_t speed;
  int16_t radius;
  GColor color;
} Ball;

Ball* create_ball(GRect bounds, int16_t speed, int16_t radius, GColor color);

void reset_ball(Ball *ball, GRect bounds);

void draw_ball(Ball *ball, Layer *layer, GContext *ctx);

void move_ball(Ball *ball);

void change_ball_direction(Ball *ball, int16_t directionX, int16_t directionY);