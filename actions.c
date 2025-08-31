#include "fabric.h"

int is_mouse_between(pos_t a, pos_t b, pos_t mouse) {
  float dx = fabs(b.x - a.x), dy = fabs(b.y - a.y);
  float D = dy / dx;

  pos_t min = a.x > b.x ? b : a;
  pos_t max = a.x < b.x ? b : a;

  for (int x = 1; x < dx; x++) {
    int pX = min.x + x, pY = min.y + D * x;
    if (pX == mouse.x && pY == mouse.y)
      return 1;
    if (pX == mouse.x + 1 && pY == mouse.y + 1)
      return 1;
    if (pX == mouse.x && pY == mouse.y + 1)
      return 1;
    if (pX == mouse.x - 1 && pY == mouse.y + 1)
      return 1;
    if (pX == mouse.x - 1 && pY == mouse.y)
      return 1;
    if (pX == mouse.x - 1 && pY == mouse.y - 1)
      return 1;
    if (pX == mouse.x && pY == mouse.y - 1)
      return 1;
    if (pX == mouse.x + 1 && pY == mouse.y - 1)
      return 1;
    if (pX == mouse.x + 1 && pY == mouse.y)
      return 1;
  }

  return 0;
}
