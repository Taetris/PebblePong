#include <pebble.h>

#include "field.h"

void draw_field(GContext *ctx, GRect bounds, int16_t borderWidth) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_stroke_width(ctx, borderWidth);
    
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
  
  // middle line 
  graphics_context_set_stroke_width(ctx, 2);
  graphics_draw_line(ctx,
                    GPoint(bounds.size.w / 2, bounds.origin.y),
                    GPoint(bounds.size.w / 2, bounds.size.h));
}