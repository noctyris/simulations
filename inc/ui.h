#ifndef UI_H
#define UI_H

#include "fabric.h"
#include <SDL2/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

typedef struct {
  int r, g, b;
} rgb_t;

int sdl_init(char *title);
void set_color(rgb_t color);
void fill_rect(pos_t pos, int width, int height, rgb_t color, int filled);
void draw_line(pos_t pos1, pos_t pos2, rgb_t color);

#endif
