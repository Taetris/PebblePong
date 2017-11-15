#include <pebble.h>

#include "ball.h"
#include "config.h"

static const int16_t directions[] = {-1, 1};

Ball* create_ball(GRect bounds, int16_t speed, int16_t radius, GColor color) {
  Ball *ball = (Ball*) malloc(sizeof(Ball));
  if (ball == NULL) {
    printf("ERROR: Out of memory!");
    return NULL;
  }

  *ball = (Ball) {
    .x = (bounds.origin.x + bounds.size.w) / 2,
    .y = (bounds.origin.y + bounds.size.h) / 2,
    .directionX = directions[rand() % ARRAY_LENGTH(directions)],
    .directionY = directions[rand() % ARRAY_LENGTH(directions)],
    .speed = speed,
    .radius = radius,
    .color = color
  };
  return ball;
}

void reset_ball(Ball *ball, GRect bounds) { 
  ball->x = (bounds.origin.x + bounds.size.w) / 2;
  ball->y = (bounds.origin.y + bounds.size.h) / 2;
  ball->directionX = directions[rand() % ARRAY_LENGTH(directions)];;
  ball->directionY = directions[rand() % ARRAY_LENGTH(directions)];;
}

void draw_ball(Ball *ball, Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_stroke_color(ctx, ball->color);
  graphics_context_set_fill_color(ctx, ball->color);
  graphics_fill_circle(ctx, GPoint(ball->x, ball->y), ball->radius);
}

void move_ball(Ball *ball) {
   ball->x += ball->speed * ball->directionX;
   ball->y += ball->speed * ball->directionY;
}

void change_ball_direction(Ball *ball, int16_t directionX, int16_t directionY) {
  ball->directionX = directionX;
  ball->directionY = directionY;
}





