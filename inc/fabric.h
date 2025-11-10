#ifndef FABRIC_H
#define FABRIC_H

#define OFFSET_X 100
#define OFFSET_Y 100
#define WIDTH 800
#define HEIGHT 600

#include <stdlib.h>

typedef struct {
  int x, y;
} pos_t;

typedef struct mesh_t {
  pos_t pos, old_pos;
  struct mesh_t *nbrs[8];
  int fixed;
} mesh_t;

typedef struct {
  mesh_t **grid;
  int width, height;
  float spacing;
} fabric_t;

fabric_t *create_fabric(int width, int height, float spacing);
void free_fabric(fabric_t *fabric);
void setup_fabric_connections(fabric_t *fabric);

#endif
