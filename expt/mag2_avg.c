#include <nd-xy.h>

int main(void) {
  double J[3] = {1.0, 1.0, 1.0};

#pragma omp parallel for
  for (uint32_t t = 0; t < 100; t++) {
    XY_lat lat = XY_init(3, 8);
    double M = 0, T = 0.05 + (4.0 - 0.05) * 0.01 * t;
    xor256s_t r = xor256s_init(t * 908042 + 4888);

    XY_rand(&lat, &r);
    XY_evolve(&lat, 1.0 / T, 10000 * lat.N, J, 0, &r);
    for (int l = 0; l < 200; l++) {
      M += XY_M2_N2(&lat);
      XY_evolve(&lat, 1.0 / T, 100 * lat.N, J, 0, &r);
    }
    printf("%f, %f\n", T, M / 200);
    XY_free(&lat);
  }

  return 0;
}