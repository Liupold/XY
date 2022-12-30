#define _POSIX_C_SOURCE 200809L
#define M_PI 3.14159265358979323846

#include "bs_tools.h"
#include <nd-xy.h>

// -------------------------------------------------------------
void XY_free(XY_lat *lat) {
  free(lat->S);
  lat->S = NULL;
  lat->N = 0;
  lat->r = 0;
  lat->d = 0;
}
XY_lat XY_init(uint32_t d, uint32_t r) {
  uint64_t N = ipow(r, d);
  double *S = (double *)calloc(N, sizeof(double));
  XY_lat lat = {S, r, d, N};
  return lat;
}
// -------------------------------------------------------------

void XY_rand(XY_lat *lat, xor256s_t *r) {
  for (uint64_t i = 0; i < lat->N; i++) {
    lat->S[i] = rand_uni(r) * 2.0 * M_PI;
  }
}

// -------------------------------------------------------------
double XY_dH(const XY_lat *lat, double *J, uint64_t i, double del_s) {
  // INPUT:
  // lat:  lattice {S, r, d, N}
  // J: array contain interaction. (d elements)
  // i: index i
  // s2: new value @ i
  uint64_t pow_rk, idf, a, b;
  double dH = 0;
  double com = lat->S[i] + del_s / 2;

  // printf("%d: ", i);
  // a, b : are the nearest neighbour in kth dimention.
  pow_rk = 1;
  for (uint32_t k = 0; k < lat->d; k++) {
    a = i + pow_rk;
    b = i - pow_rk;
    idf = i / pow_rk;
    pow_rk *= lat->r;

    if (idf % pow_rk == 0) {
      b += pow_rk;
    } else if (idf % pow_rk == lat->r - 1) {
      a -= pow_rk;
    }
    // printf("(%d, %d) ", a, b);
    dH += J[k] * sin(com - lat->S[a]);
    dH += J[k] * sin(com - lat->S[b]);
  }
  // printf("\n");
  // -(cos(φ + δφ - φj) - cos(φ - φj)) = 2 sin(δφ/2) sin(φ + δφ/2 - φj).
  return 2 * dH * sin(del_s / 2);
}

void XY_evolve(XY_lat *lat, double beta, uint64_t steps, double *J, double h,
               xor256s_t *r) {
  double del_H, del_s;
  uint64_t i;

  for (uint64_t iter = 1; iter <= steps; iter++) {
    // i = (i + rand_u64(r) % lat->r) % (lat->N); // choose i
    i = rand_u64(r) % (lat->N); // choose i
    del_s = (-1 + 2 * rand_uni(r)) * M_PI;

    del_H = XY_dH(lat, J, i, del_s) + h * cos(lat->S[i]);
    if (del_H > 0) { // Del E > 0
      if (rand_uni(r) > exp(-beta * del_H)) {
        continue;
      }
    }
    lat->S[i] += del_s;

    if (lat->S[i] > 2 * M_PI)
      lat->S[i] -= 2 * M_PI;
    if (lat->S[i] < 0)
      lat->S[i] += 2 * M_PI;
    // printf("del_H: %f\n", del_H);
    //  printf("%5.5f\r", (double)iter / (double)steps);
  }
}
// -------------------------------------------------------------
double XY_M2_N2(XY_lat *lat) {
  double M1 = 0, M2 = 0;
  for (uint64_t i = 0; i < lat->N; i++) {
    M1 += cos(lat->S[i]);
  }
  for (uint64_t i = 0; i < lat->N; i++) {
    M2 += sin(lat->S[i]);
  }
  return ((pow(M1, 2) + pow(M2, 2)) / (double)(lat->N * lat->N));
}

// -------------------------------------------------------------
double XY_is_align(const XY_lat *lat, double *J, uint64_t i) {
  // INPUT:
  // lat:  lattice {S, r, d, N}
  // J: array contain interaction. (d elements)
  // i: index i
  uint64_t pow_rk, idf, a, b;
  double dH = 0;

  // printf("%d: ", i);
  // a, b : are the nearest neighbour in kth dimention.
  pow_rk = 1;
  for (uint32_t k = 0; k < lat->d; k++) {
    a = i + pow_rk;
    b = i - pow_rk;
    idf = i / pow_rk;
    pow_rk *= lat->r;

    if (idf % pow_rk == 0) {
      b += pow_rk;
    } else if (idf % pow_rk == lat->r - 1) {
      a -= pow_rk;
    }
    // printf("(%d, %d) ", a, b);
    dH += J[k] * cos(lat->S[i] - lat->S[a]);
    dH += J[k] * cos(lat->S[i] - lat->S[b]);
  }
  // -J * cos(φ - φj)
  return dH;
}
