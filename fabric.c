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
        mesh->pos.x = mesh->pos.x + vel_x + 0.0f * dt * dt;
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
    const fabric_t original = {fabric->grid, fabric->width, fabric->height,
                               fabric->spacing};
    apply_constraints(original, fabric);
  }
}

void apply_constraints(const fabric_t original, fabric_t *modified) {
  static int reverse = 0;

  for (int y = reverse ? original.height - 1 : 0;
       reverse ? y >= 0 : y < original.height; y += (reverse ? -1 : 1)) {
    for (int x = reverse ? original.width - 1 : 0;
         reverse ? x >= 0 : x < original.width; x += (reverse ? -1 : 1)) {
      mesh_t *mesh = &modified->grid[y][x];
      const mesh_t or_mesh = original.grid[y][x];

      /*for (int i = 0; i < 4; i++) { // ¤
        if (mesh->nbrs[i] != NULL) apply_spring_constraint(mesh,
      *or_mesh.nbrs[i], original.spacing);;
      }*/
      for (int i = 0; i < 8; i++) { // Apply to all 8 neighbors
        if (mesh->nbrs[i] != NULL) {
          float length =
              (i < 4) ? original.spacing : original.spacing * sqrt(2);
          apply_spring_constraint(mesh, *or_mesh.nbrs[i], length);
        }
      }
    }
  }
  reverse = !reverse;
}

void apply_spring_constraint(mesh_t *mesh, const mesh_t nbr,
                             float rest_length) {
  float dx = nbr.pos.x - mesh->pos.x;
  float dy = nbr.pos.y - mesh->pos.y;
  float dist = sqrt(dx * dx + dy * dy);

  if (dist < 0.001f)
    return;

  float difference = (rest_length - dist) / dist;
  float corr_x = difference * dx * 0.5f; // Split the difference
  float corr_y = difference * dy * 0.5f; // in both meshes

  if (!mesh->fixed) {
    mesh->pos.x -= corr_x; // Process only in 1st node because every neighbor is
                           // processed (¤)
    mesh->pos.y -= corr_y;
  }
}
