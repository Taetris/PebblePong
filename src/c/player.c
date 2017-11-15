#include <pebble.h>

#include "player.h"
#include "config.h"

Player* create_left_player(GRect bounds, GColor color) {
  Player *player = (Player*) malloc(sizeof(Player));
  if (player == NULL) {
    printf("ERROR: Out of memory!");
    return NULL; 
  }
  
  *player = (Player) {
    .x = bounds.origin.x,
    .y = (bounds.size.h / 2) - (PLAYER_HEIGHT / 2),
    .width = PLAYER_WIDTH,
    .height = PLAYER_HEIGHT,
    .color = color
  };
  return player;
}

Player* create_right_player(GRect bounds, GColor color) {
  Player *player = (Player*) malloc(sizeof(Player));
  if (player == NULL) {
    printf("ERROR: Out of memory!");
    return NULL; 
  }
  
  *player = (Player) {
    .x = bounds.size.w - PLAYER_WIDTH,
    .y = (bounds.size.h / 2) - (PLAYER_HEIGHT / 2),
    .width = PLAYER_WIDTH,
    .height = PLAYER_HEIGHT,
    .color = color
  };
  return player;
}

void reset_left_player(Player *player, GRect bounds) {
  player->x = bounds.origin.x;
  player->y = (bounds.size.h / 2) - (PLAYER_HEIGHT / 2);
} 

void reset_right_player(Player *player, GRect bounds) {
  player->x = bounds.size.w - PLAYER_WIDTH;
  player->y = (bounds.size.h / 2) - (PLAYER_HEIGHT / 2);
}

void draw_player(Player *player, Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_stroke_color(ctx, player->color);
  graphics_context_set_fill_color(ctx, player->color);
  graphics_fill_rect(ctx, GRect(player->x, player->y, player->width, player->height), 0, GCornerNone);
  graphics_draw_rect(ctx, GRect(player->x, player->y, player->width, player->height));
}

bool move_up(Player *player, GRect bounds, int16_t dy) {
  if (player->y - dy < bounds.origin.y) {
    return false;
  } else {
    player->y = player->y - dy; 
    return true;
  }
}

bool move_down(Player *player, GRect bounds, int16_t dy) {
  if ((player->y + player->height) + dy > bounds.size.h) {
    return false;
  } else {
    player->y = player->y + dy; 
    return true;
  }
}