#include <pebble.h>

#include "player.h"
#include "ball.h"
#include "field.h"
#include "config.h"

static Window *window;
static Layer *canvas_layer;
static TextLayer *score_layer;

static AppTimer *timer;

static Player *player;
static Player *ai;
static Ball *ball;

char score_text[128];
static int16_t playerScore;
static int16_t aiScore;

static void restart(GRect bounds) {
  reset_ball(ball, bounds);
  reset_left_player(ai, bounds);
  reset_right_player(player, bounds);
  snprintf(score_text, sizeof(score_text), "%d  %d", aiScore, playerScore);
  text_layer_set_text(score_layer, score_text);
}

static void new_game() {
  GRect bounds = layer_get_bounds(canvas_layer);
  playerScore = 0;
  aiScore = 0;
  restart(bounds);
}

static bool is_ball_colliding_with_left_player(GRect bounds, int16_t dx, int16_t dy) {
  return ((ball->x + dx <= ai->x + ai->width) && 
          (ball->y + dy >= ai->y) &&
          (ball->y + dy <= ai->y + ai->height));
}

static bool is_ball_colliding_with_right_player(GRect bounds, int16_t dx, int16_t dy) {
  return ((ball->x + dx >= player->x) && 
          (ball->y + dy >= player->y) &&
          (ball->y + dy <= player->y + player->height));
}

static bool is_ball_colliding_with_left_wall(GRect bounds, int16_t dx, int16_t dy) {
  return ball->x + dx <= bounds.origin.x;
}

static bool is_ball_colliding_with_right_wall(GRect bounds, int16_t dx, int16_t dy) {
  return ball->x + dx >= bounds.size.w;
}

static bool is_ball_colliding_with_top_wall(GRect bounds, int16_t dx, int16_t dy) {
  return ball->y + dy <= bounds.origin.y;
}

static bool is_ball_colliding_with_bottom_wall(GRect bounds, int16_t dx, int16_t dy) {
  return ball->y + dy >= bounds.size.h;
}

static void on_timer_tick(void *data) {
  GRect bounds = layer_get_bounds(canvas_layer);
  
  timer = app_timer_register(REFRESH_RATE, on_timer_tick, NULL);
  layer_mark_dirty(canvas_layer);
  
  if (ball->x <= (bounds.size.w / 2) + ai->width) {
    if (ball->y < ai->y) {
      move_up(ai, bounds, PLAYER_MOVE_PIXELS);
    } else {
      move_down(ai, bounds, PLAYER_MOVE_PIXELS);
    }
  }
  
  int16_t dx = (ball->radius + ball->speed) * ball->directionX;
  int16_t dy = (ball->radius + ball->speed) * ball->directionY;
  
  if (is_ball_colliding_with_left_player(bounds, dx, dy) || 
      is_ball_colliding_with_right_player(bounds, dx, dy)) {
    change_ball_direction(ball, ball->directionX * -1, ball->directionY);
  } else if (is_ball_colliding_with_top_wall(bounds, dx, dy) || 
             is_ball_colliding_with_bottom_wall(bounds, dx, dy)) {
    change_ball_direction(ball, ball->directionX, ball->directionY * -1);
  } else if (is_ball_colliding_with_left_wall(bounds, dx, dy)) {
    playerScore++;
    restart(bounds);
  } else if (is_ball_colliding_with_right_wall(bounds, dx, dy)) {
    aiScore++;
    restart(bounds);
  }
  
  move_ball(ball);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  
  draw_field(ctx, bounds);
  
  // TODO: Null checks
  draw_player(player, layer, ctx);
  draw_player(ai, layer, ctx);
  draw_ball(ball, layer, ctx);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  new_game();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  GRect bounds = layer_get_bounds(canvas_layer);
  if (move_up(player, bounds, PLAYER_MOVE_PIXELS)) {
    layer_mark_dirty(canvas_layer);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  GRect bounds = layer_get_bounds(canvas_layer);
  if (move_down(player, bounds, PLAYER_MOVE_PIXELS)) {
    layer_mark_dirty(canvas_layer);
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  canvas_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y + FIELD_OFFSET, bounds.size.w, bounds.size.h - (FIELD_OFFSET * 2)));
  layer_set_update_proc(canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, canvas_layer);
  
  score_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.origin.y + FIELD_OFFSET));
  text_layer_set_text(score_layer, "1  0");
  text_layer_set_text_alignment(score_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(score_layer));
  
  player = create_right_player(bounds, GColorBlack);
  ai = create_left_player(bounds, GColorBlack);
  ball = create_ball(bounds, BALL_SPEED, BALL_RADIUS, GColorBlack);
  playerScore = 0;
  aiScore = 0;
  
  timer = app_timer_register(REFRESH_RATE, on_timer_tick, NULL);
}

static void window_unload(Window *window) {
  // kill timer and release resources
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  light_enable(true);
}

static void deinit(void) {
  window_destroy(window);
  light_enable(false);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}