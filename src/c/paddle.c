#include <pebble.h>

#include "paddle.h"

Paddle* create_left_paddle(GRect bounds, int16_t paddleWidth, int16_t paddleHeight, GColor color) {
  Paddle *paddle = (Paddle*) malloc(sizeof(Paddle));
  if (paddle == NULL) {
    printf("ERROR: Out of memory!");
    return NULL; 
  }
  
  *paddle = (Paddle) {
    .x = bounds.origin.x,
    .y = (bounds.size.h / 2) - (paddleHeight / 2),
    .width = paddleWidth,
    .height = paddleHeight,
    .color = color
  };
  return paddle;
}

Paddle* create_right_paddle(GRect bounds, int16_t paddleWidth, int16_t paddleHeight, GColor color) {
  Paddle *paddle = (Paddle*) malloc(sizeof(Paddle));
  if (paddle == NULL) {
    printf("ERROR: Out of memory!");
    return NULL; 
  }
  
  *paddle = (Paddle) {
    .x = bounds.size.w - paddleWidth,
    .y = (bounds.size.h / 2) - (paddleHeight / 2),
    .width = paddleWidth,
    .height = paddleHeight,
    .color = color
  };
  return paddle;
}

void reset_left_paddle(Paddle *paddle, GRect bounds) {
  paddle->x = bounds.origin.x;
  paddle->y = (bounds.size.h / 2) - (paddle->height / 2);
}

void reset_right_paddle(Paddle *paddle, GRect bounds) {
  paddle->x = bounds.size.w - paddle->width;
  paddle->y = (bounds.size.h / 2) - (paddle->height / 2);
}

void draw_paddle(Paddle *paddle, Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_stroke_color(ctx, paddle->color);
  graphics_context_set_fill_color(ctx, paddle->color);
  graphics_fill_rect(ctx, GRect(paddle->x, paddle->y, paddle->width, paddle->height), 0, GCornerNone);
  graphics_draw_rect(ctx, GRect(paddle->x, paddle->y, paddle->width, paddle->height));
}

bool move_up(Paddle *paddle, GRect bounds, int16_t dy) {
   if (paddle->y - dy < bounds.origin.y) {
    return false;
  } else {
    paddle->y = paddle->y - dy; 
    return true;
  }
}

bool move_down(Paddle *paddle, GRect bounds, int16_t dy) {
  if ((paddle->y + paddle->height) + dy > bounds.size.h) {
    return false;
  } else {
    paddle->y = paddle->y + dy; 
    return true;
  }
}
