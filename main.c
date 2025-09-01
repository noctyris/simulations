#include "actions.h"
#include "fabric.h"
#include "ui.h"

int main() {
  sdl_init("Test");

  fabric_t *fabric = create_fabric(15, 5, 30);
  int running = 1;

  int is_click = 0;
  pos_t click_pos[2];
  click_pos[1].x = -1;
  click_pos[1].y = -1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_MOUSEMOTION:
        if (is_click) {
          click_pos[1].x = event.motion.x;
          click_pos[1].y = event.motion.y;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        is_click = 1;
        click_pos[0].x = event.motion.x;
        click_pos[0].y = event.motion.y;
        break;
      case SDL_MOUSEBUTTONUP:
        is_click = 0;
        click_pos[1].x = -1;
        click_pos[1].y = -1;
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    if (click_pos[1].x != -1 && click_pos[1].y != -1)
      draw_line(click_pos[0], click_pos[1], (rgb_t){0, 255, 255});

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

          // if (is_mouse_between(mesh->pos, nbr->pos, click_pos[1]))
          // printf("(%d, %d) -> (%d, %d)\t(%d, %d)\n", mesh->pos.x,
          // mesh->pos.y, nbr->pos.x, nbr->pos.y, click_pos[1].x,
          // click_pos[1].y);

          if (is_mouse_between(mesh->pos, nbr->pos, click_pos[1])) {
            mesh->nbrs[i] = NULL;
          }
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
