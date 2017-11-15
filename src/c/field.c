#include <pebble.h>

#include "field.h"
#include "config.h"

void draw_field(GContext *ctx, GRect bounds) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_stroke_width(ctx, BORDER_WIDTH);
    
  // top left -> down left
  graphics_draw_line(ctx, 
                     GPoint(bounds.origin.x, bounds.origin.y),
                     GPoint(bounds.origin.x, bounds.size.h));
  // top left -> top right
  graphics_draw_line(ctx, 
                     GPoint(bounds.origin.x, bounds.origin.y),
                     GPoint(bounds.size.w, bounds.origin.y));
  // bottom left -> bottom right
  graphics_draw_line(ctx, 
                     GPoint(bounds.origin.x, bounds.size.h),
                     GPoint(bounds.size.w, bounds.size.h));
  // top right -> bottom right
  graphics_draw_line(ctx, 
                     GPoint(bounds.size.w, bounds.origin.y),
                     GPoint(bounds.size.w, bounds.size.h));
  
  graphics_context_set_stroke_width(ctx, 2);
  // middle line 
  graphics_draw_line(ctx,
                    GPoint(bounds.size.w / 2, bounds.origin.y),
                    GPoint(bounds.size.w / 2, bounds.size.h));
}