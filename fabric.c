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

      mesh->pos.x = x * spacing + OFFSET_X;
      mesh->pos.y = y * spacing + OFFSET_Y;
      mesh->old_pos = mesh->pos;

      for (int i = 0; i < 8; i++) {
        mesh->nbrs[i] = NULL;
      }

      mesh->fixed =
          (y == 0 && (x == 0 || x == 1 || x == width - 2 || x == width - 1));
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
      if (x > 0 && y < fabric->height - 1)
        mesh->nbrs[6] = &fabric->grid[y + 1][x - 1];
      if (x < fabric->width - 1 && y < fabric->height - 1)
        mesh->nbrs[7] = &fabric->grid[y + 1][x + 1];
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

void update_fabric(fabric_t *fabric, float dt) {
  for (int y = 0; y < fabric->height; y++) {
    for (int x = 0; x < fabric->width; x++) {
      mesh_t *mesh = &fabric->grid[y][x];

      if (!mesh->fixed) {
        pos_t temp = mesh->pos;

        // Add damping (0.99 = 1% damping per frame)
        float damping = 0.7f;
        float vel_x = (mesh->pos.x - mesh->old_pos.x) * damping;
        float vel_y = (mesh->pos.y - mesh->old_pos.y) * damping;

        // Verlet integration: pos = pos + dpos + acceleration * dt^2
        mesh->pos.x = mesh->pos.x + vel_x + 3.5f * dt * dt;
        mesh->pos.y = mesh->pos.y + vel_y + 9.8f * dt * dt;

        if (mesh->pos.x < 0)
          mesh->pos.x = 0;
        if (mesh->pos.x > WIDTH)
          mesh->pos.x = WIDTH;
        if (mesh->pos.y < 0)
          mesh->pos.y = 0;
        if (mesh->pos.y > HEIGHT)
          mesh->pos.y = HEIGHT;

        mesh->old_pos = temp;
      }
    }
  }

  for (int iter = 0; iter < 2; iter++) {
    apply_constraints(fabric);
  }
}

void apply_constraints(fabric_t *fabric) {
  static int reverse = 0;

  if (!reverse) {
    for (int y = 0; y < fabric->height; y++) {
      for (int x = 0; x < fabric->width; x++) {
        mesh_t *mesh = &fabric->grid[y][x];

        // Only process RIGHT and DOWN neighbors to avoid double-processing
        if (mesh->nbrs[1] != NULL) {
          apply_spring_constraint(mesh, mesh->nbrs[1], fabric->spacing);
        }
        if (mesh->nbrs[3] != NULL) {
          apply_spring_constraint(mesh, mesh->nbrs[3], fabric->spacing);
        }
      }
    }
  } else {
    for (int y = 0; y < fabric->height; y++) {
      for (int x = fabric->width - 1; x > 0; x++) {
        mesh_t *mesh = &fabric->grid[y][x];

        // Only process RIGHT and DOWN neighbors to avoid double-processing
        if (x < fabric->width - 1 && mesh->nbrs[1] != NULL) {
          apply_spring_constraint(mesh, mesh->nbrs[1], fabric->spacing);
        }
        if (y < fabric->height - 1 && mesh->nbrs[3] != NULL) {
          apply_spring_constraint(mesh, mesh->nbrs[3], fabric->spacing);
        }
      }
    }
  }
}

void apply_spring_constraint(mesh_t *a, mesh_t *b, float rest_length) {
  float dx = b->pos.x - a->pos.x;
  float dy = b->pos.y - a->pos.y;
  float dist = sqrt(dx * dx + dy * dy);

  if (dist < 0.001f)
    return;

  float difference = (rest_length - dist) / dist;
  float corr_x = difference * dx * 0.5f; // Split the difference
  float corr_y = difference * dy * 0.5f; // in both meshes

  if (!a->fixed && !b->fixed) {
    a->pos.x -= corr_x;
    a->pos.y -= corr_y;
    b->pos.x += corr_x;
    b->pos.y += corr_y;
  } else if (!a->fixed && b->fixed) {
    a->pos.x -= corr_x * 2;
    a->pos.y -= corr_y * 2;
  } else if (a->fixed && !b->fixed) {
    b->pos.x += corr_x * 2;
    b->pos.y += corr_y * 2;
  }
}
