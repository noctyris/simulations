#ifndef FABRIC_H
#define FABRIC_H

#include <stdlib.h>

typedef struct {
  int x;
  int y;
} pos_t;

typedef struct mesh_t {
  pos_t pos, old_pos;
  struct mesh_t *nbr[8];
  int fixed;
} mesh_t;

typedef struct {
  mesh_t **grid;
  int width, height; // number of meshes
  float spacing;
} fabric_t;

fabric_t *create_fabric(int width, int height, float spacing);

#endif
