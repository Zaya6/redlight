#import "rmath.h"
#include <math.h>

// point x float
point pmulf(point p, float f) { return (point){{p.x * f, p.y * f, p.z * f}}; }

void pmulfm(point *p, float f) {
  p->x *= f;
  p->y *= f;
  p->z *= f;
}

// point x point

point pmulp(point p0, point p1) {
  return (point){{p0.x + p1.x, p0.y + p1.y, p0.z + p1.z}};
}

void pmulpm(point *p0, point p1) {
  p0->x *= p1.x;
  p0->y *= p1.y;
  p0->z *= p1.z;
}

// point / float
point pdivf(point p, float f) { return (point){{p.x / f, p.y / f, p.z / f}}; }

void pdivfm(point *p, float f) {
  p->x /= f;
  p->y /= f;
  p->z /= f;
}

// others
float pmag(const point *p) {
  return (float)sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
}
