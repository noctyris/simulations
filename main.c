#include "fabric.h"
#include "ui.h"

int main() {
  sdl_init("Test");

  fabric_t *fabric = create_fabric(20, 5, 10);
  int running = 1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = 0;
    }
    SDL_RenderPresent(renderer);
  }

  free_fabric(fabric);

  return 0;
}
