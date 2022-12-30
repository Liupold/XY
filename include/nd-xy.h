#ifndef NDXY_H
#define NDXY_H

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#define M_PI 3.14159265358979323846

#include "xos256plus.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  double *S;  // spin configuration (theta)
  uint32_t r; // folding length (side length)
  uint64_t d; // dimention
  uint64_t N; // total number of spin (r^d)
} XY_lat;

// -------------------------------------------------------------
double XY_M2_N2(XY_lat *lat);
// -------------------------------------------------------------
// -------------------------------------------------------------
void XY_free(XY_lat *lat);
XY_lat XY_init(uint32_t d, uint32_t r);
// -------------------------------------------------------------
void XY_rand(XY_lat *lat, xor256s_t *r);
double XY_dH(const XY_lat *lat, double *J, uint64_t i, double s2);
void XY_evolve(XY_lat *lat, double beta, uint64_t steps, double *J, double h,
               xor256s_t *r);
// -------------------------------------------------------------
double XY_is_align(const XY_lat *lat, double *J, uint64_t i);

#endif
