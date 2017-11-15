#include <pebble.h>

#include "paddle.h"
#include "ball.h"
#include "field.h"

// screen
#define REFRESH_RATE 300

// field
#define FIELD_OFFSET 30 
#define BORDER_WIDTH 4

// player
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 20
#define PADDLE_MOVE_PIXELS 10 

// ball
#define BALL_RADIUS 5
#define BALL_SPEED 6

static Window *window;
static Layer *canvas_layer;
static TextLayer *score_layer;

static AppTimer *timer;

static Paddle *leftPaddle;
static Paddle *rightPaddle;
static Ball *ball;

char score[20];
static int16_t leftPaddleScore;
static int16_t rightPaddleScore;

static void restart() {
  GRect bounds = layer_get_bounds(canvas_layer);
  reset_ball(ball, bounds);
  reset_left_paddle(leftPaddle, bounds);
  reset_right_paddle(rightPaddle, bounds);
  snprintf(score, sizeof(score), "%d  %d", leftPaddleScore, rightPaddleScore);
  text_layer_set_text(score_layer, score);
}

static void new_game() {
  leftPaddleScore = 0;
  rightPaddleScore = 0;
  restart();
}

static bool is_ball_colliding_with_left_paddle(GRect bounds, int16_t dx, int16_t dy) {
  return ((ball->x + dx <= leftPaddle->x + leftPaddle->width) && 
          (ball->y + dy >= leftPaddle->y) &&
          (ball->y + dy <= leftPaddle->y + leftPaddle->height));
}

static bool is_ball_colliding_with_right_paddle(GRect bounds, int16_t dx, int16_t dy) {
  return ((ball->x + dx >= rightPaddle->x) && 
          (ball->y + dy >= rightPaddle->y) &&
          (ball->y + dy <= rightPaddle->y + rightPaddle->height));
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
  
  if (ball->x <= (bounds.size.w / 2) + leftPaddle->width) {
    if (ball->y < leftPaddle->y) {
      move_up(leftPaddle, bounds, PADDLE_MOVE_PIXELS);
    } else {
      move_down(leftPaddle, bounds, PADDLE_MOVE_PIXELS);
    }
  }
  
  int16_t dx = (ball->radius + ball->speed) * ball->directionX;
  int16_t dy = (ball->radius + ball->speed) * ball->directionY;
  
  if (is_ball_colliding_with_left_paddle(bounds, dx, dy) || 
      is_ball_colliding_with_right_paddle(bounds, dx, dy)) {
    change_ball_direction(ball, ball->directionX * -1, ball->directionY);
  } else if (is_ball_colliding_with_top_wall(bounds, dx, dy) || 
             is_ball_colliding_with_bottom_wall(bounds, dx, dy)) {
    change_ball_direction(ball, ball->directionX, ball->directionY * -1);
  } else if (is_ball_colliding_with_left_wall(bounds, dx, dy)) {
    rightPaddleScore++;
    restart();
  } else if (is_ball_colliding_with_right_wall(bounds, dx, dy)) {
    leftPaddleScore++;
    restart();
  }
  
  move_ball(ball);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  
  draw_field(ctx, bounds, BORDER_WIDTH);
  
  if (leftPaddle != NULL) {
    draw_paddle(leftPaddle, layer, ctx);
  }
  if (rightPaddle != NULL) {
    draw_paddle(rightPaddle, layer, ctx);
  }
  if (ball != NULL) {
    draw_ball(ball, layer, ctx);
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  new_game();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  GRect bounds = layer_get_bounds(canvas_layer);
  if (move_up(rightPaddle, bounds, PADDLE_MOVE_PIXELS)) {
    layer_mark_dirty(canvas_layer);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  GRect bounds = layer_get_bounds(canvas_layer);
  if (move_down(rightPaddle, bounds, PADDLE_MOVE_PIXELS)) {
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
  
  leftPaddle = create_left_paddle(bounds, PADDLE_WIDTH, PADDLE_HEIGHT, GColorBlack);
  rightPaddle = create_right_paddle(bounds, PADDLE_WIDTH, PADDLE_HEIGHT, GColorBlack);
  ball = create_ball(bounds, BALL_SPEED, BALL_RADIUS, GColorBlack);
  leftPaddleScore = 0;
  rightPaddleScore = 0;
  
  timer = app_timer_register(REFRESH_RATE, on_timer_tick, NULL);
}

static void window_unload(Window *window) {
  text_layer_destroy(score_layer);
  
  if (leftPaddle != NULL) {
    free(leftPaddle);
    leftPaddle = NULL; 
  }
  if (rightPaddle != NULL) {
    free(rightPaddle);
    rightPaddle = NULL; 
  }
  if (ball != NULL) {
    free(ball);
    ball = NULL;  
  }
  if (timer != NULL) {
    app_timer_cancel(timer);
    timer = NULL;
  }
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
  app_event_loop();
  deinit();
}