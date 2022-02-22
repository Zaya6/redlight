#pragma once

typedef union point {
   struct { float x, y, z; };
   float xyz[3];
} point;

typedef point vector;

typedef union matrix {
   point a,b,c;
   point abc[3];
   float ij[3][3];
   float f[9];
} matrix;

// point x float
point
pmulf (point p, float f);

void
pmulfm (point *p, float f);

//point x point

point 
pmulp (point p0, point p1);

void
pmulpm (point *p0, point p1);

//point / float
point
pdivf ( point p, float f);

void
pdivfm (point *p, float f);

// others
float
isqrt(float n);

float
pmag (const point *p);

float rsqrt( float x );
