#include "fabric.h"

int is_mouse_between(pos_t a, pos_t b, pos_t mouse) {
  float dx = fabs(b.x - a.x), dy = fabs(b.y - a.y);
  float D = dy / dx;

  pos_t min = a.x > b.x ? b : a;
  pos_t max = a.x < b.x ? b : a;

  for (int x = 1; x < dx; x++) {
    for (int i = -2; i < 3; i++) {
      int pX = min.x + x, pY = min.y + D * x + i;
      if (mouse.x == pX && mouse.y == pY)
        return 1;
    }
    for (int i = -2; i < 3; i++) {
      int pX = min.x + x + i, pY = min.y + D * x;
      if (mouse.x == pX && mouse.y == pY)
        return 1;
    }
  }

  return 0;
}
