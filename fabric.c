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
        mesh->nbrs[i] = NULL;
      }

      mesh->fixed = (y == 0 && (x == 0 || x == width - 1));
    }
  }

  setup_fabric_connections(fabric);

  return fabric;
}

void setup_fabric_connections(fabric_t *fabric) {
  for (int y = 0; y < fabric->height; y++) {
    for (int x = 0; x < fabric->width; x++) {
      mesh_t *mesh = &fabric->grid[y][x];

      if (x > 0)
        mesh->nbrs[0] = &fabric->grid[y][x - 1];
      if (x < fabric->width - 1)
        mesh->nbrs[1] = &fabric->grid[y][x + 1];
      if (y > 0)
        mesh->nbrs[2] = &fabric->grid[y - 1][x];
      if (y < fabric->height - 1)
        mesh->nbrs[3] = &fabric->grid[y + 1][x];

      if (x > 0 && y > 0)
        mesh->nbrs[4] = &fabric->grid[y - 1][x - 1];
      if (x < fabric->width - 1 && y > 0)
        mesh->nbrs[5] = &fabric->grid[y - 1][x + 1];
      if (x > 0 && y > fabric->height - 1)
        mesh->nbrs[6] = &fabric->grid[y + 1][x - 1];
      if (x < fabric->width - 1 && y < fabric->height - 1)
        mesh->nbrs[7] = &fabric->grid[y + 1][x];
    }
  }
}

void free_fabric(fabric_t *fabric) {
  for (int y = 0; y < fabric->height; y++) {
    free(fabric->grid[y]);
  }
  free(fabric->grid);
  free(fabric);
}

void update_fabric(fabric_t *fabric) {}
