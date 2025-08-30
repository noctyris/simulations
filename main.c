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

    for (int y = 0; y < fabric->height; y++) {
      for (int x = 0; x < fabric->width; x++) {
        mesh_t *mesh = &fabric->grid[y][x];

        fill_rect((pos_t){mesh->pos.x-1, mesh->pos.y-1}, 3, 3, (rgb_t){255, mesh->fixed ? 0 : 255, mesh->fixed ? 0 : 255}, 1);
      }
    }

    SDL_RenderPresent(renderer);
  }

  free_fabric(fabric);

  return 0;
}
