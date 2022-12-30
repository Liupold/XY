#include "bs_tools.h"
#include <nd-xy.h>
#include <time.h>

// ---------------------------------------------------
double XY_dTheta(XY_lat *lat) {
  double del_th = 0;
  double th = 0;
  double tmp;

  for (uint64_t i = 0; i < lat->N; i++) {
    th += lat->S[i];
  }
  th /= (double)lat->N;
  for (uint64_t i = 0; i < lat->N; i++) {
    tmp = lat->S[i] - th;
    del_th += tmp * tmp;
  }
  return del_th / (double)lat->N;
}
// ---------------------------------------------------

int main(void) {
  uint32_t r_arr[5] = {32, 64, 128, 256, 512};
  double M_arr[5] = {0};
  int seed_done = 0, seed_avg = 100;
  time_t ltime;
  time(&ltime);

#pragma omp parallel for
  for (uint32_t sd = 0; sd < seed_avg; sd++) {
    double J[3] = {1.0, 1.0, 1.0};
    xor256s_t seed = xor256s_init((uint64_t)ltime + sd);
    XY_lat lat;
    for (uint32_t k = 0; k < 5; k++) {
      double M = 0, T = 0.4;
      double c_m = -1, p_m;
      lat = XY_init(2, r_arr[k]);

      // XY_rand(&lat, &seed);
      // XY_evolve(&lat, 1.0/T, 20000, J, 0, &seed);

      for (uint64_t i = 0; i < lat.N; i++) {
        lat.S[i] =
            M_PI / 2 + 0.01 * (2 * rand_uni(&seed) - 1); // all at same angle
      }

      do {
        p_m = c_m;
        XY_evolve(&lat, 1.0 / T, lat.N, J, 0, &seed);
        c_m = XY_dTheta(&lat) * (double)lat.N;
      } while (fabs(c_m - p_m) / c_m > 1E-2);

      for (uint32_t _ = 0; _ < 100; _++) {
        M += XY_dTheta(&lat);
        XY_evolve(&lat, 1.0 / T, 1, J, 0, &seed);
      }
      M_arr[k] += M / 100;
      XY_free(&lat);
    }
#pragma omp atomic
    ++seed_done;
#pragma omp critical
    fprintf(stderr, "Progress: [%u/%u]\r", seed_done, seed_avg);
  }
  fprintf(stderr, "--------Done!-------\n");

  for (uint32_t k = 0; k < 5; k++) {
    printf("%f, %f\n", log(r_arr[k]), M_arr[k] / 400);
  }

  return 0;
}
