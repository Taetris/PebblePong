#pragma once

// TODO: rename to paddle 
typedef struct Player {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    GColor color;
} Player;

Player* create_left_player(GRect bounds, GColor color);

Player* create_right_player(GRect bounds, GColor color);

void reset_left_player(Player *player, GRect bounds); 

void reset_right_player(Player *player, GRect bounds);

void draw_player(Player *player, Layer *layer, GContext *ctx);

bool move_up(Player *player, GRect bounds, int16_t dy);

bool move_down(Player *player, GRect bounds, int16_t dy);
