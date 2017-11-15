#pragma once

typedef struct Paddle {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    GColor color;
} Paddle;

Paddle* create_left_paddle(GRect bounds, int16_t paddleWidth, int16_t paddleHeight, GColor color);

Paddle* create_right_paddle(GRect bounds, int16_t paddleWidth, int16_t paddleHeight, GColor color);

void reset_left_paddle(Paddle *paddle, GRect bounds); 

void reset_right_paddle(Paddle *paddle, GRect bounds);

void draw_paddle(Paddle *paddle, Layer *layer, GContext *ctx);

bool move_up(Paddle *paddle, GRect bounds, int16_t dy);

bool move_down(Paddle *paddle, GRect bounds, int16_t dy);
