#ifndef FABRIC_H
#define FABRIC_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define OFFSET_X 100
#define OFFSET_Y 100
#define WIDTH 800
#define HEIGHT 600

typedef struct {
  int x;
  int y;
} pos_t;

typedef struct mesh_t {
  pos_t pos, old_pos;
  struct mesh_t *nbrs[8];
  int fixed;
} mesh_t;

typedef struct {
  mesh_t **grid;
  int width, height; // number of meshes
  float spacing;
} fabric_t;

fabric_t *create_fabric(int width, int height, float spacing);
void setup_fabric_connections(fabric_t *fabric);
void free_fabric(fabric_t *fabric);
void update_fabric(fabric_t *fabric, float dt);
void apply_constraints(fabric_t *fabric);
void apply_spring_constraint(mesh_t *a, mesh_t *b, float rest_length);

#endif
