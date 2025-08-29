#include "fabric.h"

fabric_t *create_fabric(int width, int height, float spacing) {
  fabric_t *fabric = malloc(sizeof(fabric_t));
  fabric->grid = malloc(sizeof(mesh_t)*height);

  return fabric;
}
