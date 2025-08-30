#include "fabric.h"

fabric_t *create_fabric(int width, int height, float spacing) {
  fabric_t *fabric = malloc(sizeof(fabric_t));
  fabric->width = width;
  fabric->height = height;
  fabric->spacing = spacing;

  fabric->grid = malloc(sizeof(mesh_t *) * height);
  for (int y = 0; y < height; y++) {
    fabric->grid[y] = malloc(sizeof(mesh_t) * width);
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      mesh_t *mesh = &fabric->grid[y][x];

      mesh->pos.x = x * spacing + 100;
      mesh->pos.y = y * spacing + 100;
      mesh->old_pos = mesh->pos;

      for (int i = 0; i < 8; i++) {
        mesh->nbr[i] = NULL;
      }

      mesh->fixed = (y == 0 && (x == 0 || x == width - 1));
    }
  }

  return fabric;
}

void free_fabric(fabric_t *fabric) {
  for (int y = 0; y < fabric->height; y++) {
    free(fabric->grid[y]);
  }
  free(fabric->grid);
  free(fabric);
}
