#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include "fabric.h"

#define WIDTH 800
#define HEIGHT 600

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

typedef struct {
  int r,g,b;
} rgb_t;

int sdl_init(char *title);
void fill_rect(pos_t pos, int width, int height, rgb_t color, int filled);

#endif
