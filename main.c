#include "fabric.h"
#include "ui.h"

int main() {
  sdl_init("Test");

  fabric_t *fabric = create_fabric(50, 10, 1.f);
  int running = 1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = 0;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int y = 0; y < fabric->height; y++) {
      for (int x = 0; x < fabric->width; x++) {
        mesh_t *mesh = &fabric->grid[y][x];

        fill_rect((pos_t){mesh->pos.x - 2, mesh->pos.y - 2}, 5, 5,
                  (rgb_t){255, mesh->fixed ? 0 : 255, mesh->fixed ? 0 : 255},
                  1);
        for (int i = 0; i < 8; i++) {
          if (mesh->nbrs[i] == NULL)
            continue;
          mesh_t *nbr = mesh->nbrs[i];

          draw_line(
              mesh->pos, nbr->pos,
              (rgb_t){i > 3 ? 100 : 255, i > 3 ? 100 : 255, i > 3 ? 100 : 255});
        }
      }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);

    update_fabric(fabric, 1.0f);
  }

  free_fabric(fabric);

  return 0;
}
