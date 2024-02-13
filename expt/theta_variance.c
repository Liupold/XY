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
    del_th += (tmp * tmp);
  }
  return del_th / (double)lat->N;
}
// ---------------------------------------------------

int main(void) {
  uint32_t r_arr[5] = {16, 20, 32, 40, 64};
  double M_arr[5] = {0};
  int seed_done = 0, seed_avg = 32;
  time_t ltime;
  time(&ltime);

#pragma omp parallel for
  for (uint32_t sd = 0; sd < seed_avg; sd++) {
    double J[3] = {1.0, 1.0, 1.0};

    xor256s_t seed = xor256s_init((uint64_t)ltime + sd);
    XY_lat lat;
    for (uint32_t k = 0; k < 5; k++) {
      double M = 0, T = 0.5;
      lat = XY_init(2, r_arr[k]);

      //XY_rand(&lat, &seed);
      XY_evolve(&lat, 1.0/T, 1000 * lat.N, J, 0, &seed);

      for (uint32_t _ = 0; _ < 10000; _++) {
        M += XY_dTheta(&lat);
        XY_evolve(&lat, 1.0 / T, lat.N, J, 0, &seed);
      }
#pragma omp atomic
      M_arr[k] += M / (10000);
      XY_free(&lat);
    }
#pragma omp atomic
    ++seed_done;
#pragma omp critical
    fprintf(stderr, "Progress: [%u/%u]\r", seed_done, seed_avg);
  }
  fprintf(stderr, "--------Done!-------\n");

  for (uint32_t k = 0; k < 5; k++) {
    printf("%f, %f\n", log(r_arr[k]), M_arr[k] / seed_avg);
  }

  return 0;
}
