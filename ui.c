#include "ui.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

int sdl_init(char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Error while initialisation");
    return 1;
  }

  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    printf("Erreur while creating window : %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Erreur  renderer : %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  return 0;
}

void set_color(rgb_t color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
}

void fill_rect(pos_t pos, int width, int height, rgb_t color, int filled) {
  set_color(color);
  SDL_Rect rect = {pos.x, pos.y, width, height};
  if (filled) SDL_RenderFillRect(renderer, &rect);
  else SDL_RenderDrawRect(renderer, &rect);
}

void draw_line(pos_t pos1, pos_t pos2, rgb_t color) {
  set_color(color);
  SDL_RenderDrawLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
}
