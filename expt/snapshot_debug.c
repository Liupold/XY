#include <nd-xy.h>

void take_2dsnap(XY_lat *lat, char *filename) {
  FILE *fpr = fopen(filename, "w");
  uint64_t n = 0;
  for (uint64_t i = 0; i < lat->r; i++) {
    for (uint64_t j = 0; j < lat->r; j++) {
      fprintf(fpr, "%ld, %ld, %lf, %lf\n", i, j, cos(lat->S[n]), sin(lat->S[n]));
      n++;
    }
  }
  fclose(fpr);
}
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
  return del_th / (double) lat->N;
}
// ---------------------------------------------------

int main(void) {
  const uint32_t r_array[] = {8, 16, 32, 64, 128};
//#pragma omp parallel for
  for (int r = 4; r >= 0; r--) {
    XY_lat lat;
    double M = 0, T = 1;
    double J[3] = {1.0, 1.0, 1.0};
    char buff[256];

    xor256s_t seed = xor256s_init(79427 + 505 * r);
    lat = XY_init(2, r_array[r]);
    XY_rand(&lat, &seed);
    XY_evolve(&lat, 1.0/T, 1000 * 8000, J, 0, &seed);
    for (int l=0; l < 10; l++) {
        M += XY_dTheta(&lat);
        XY_evolve(&lat, 1.0/T, 100 * 8000, J, 0, &seed);
    }
    snprintf(buff, 256, "./data/DEBUG_snap_%d.csv", r);
    take_2dsnap(&lat, buff);
    printf("%f, %f\n", log(lat.r), M);
    XY_free(&lat);
  }
  return 0;
}
