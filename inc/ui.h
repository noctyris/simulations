#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

int sdl_init(char *title);

#endif
