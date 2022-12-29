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

int main(void) {
  double J[3] = {1.0, 1.0, 1.0};
  XY_lat lat = XY_init(2, 8);
  double M = 0, T = 0.5;
  xor256s_t r = xor256s_init(49829);

  XY_rand(&lat, &r);
  XY_evolve(&lat, 1.0/T, 10000 * lat.N, J, 0, &r);
  M = XY_M2_N2(&lat);
  fprintf(stderr, "%f, %f\n", T, M);

  take_2dsnap(&lat, "./data/snap_1.csv");
  XY_evolve(&lat, 1.0/T, 1 * lat.N, J, 0, &r);
  take_2dsnap(&lat, "./data/snap_2.csv");
  XY_evolve(&lat, 1.0/T, 1 * lat.N, J, 0, &r);
  take_2dsnap(&lat, "./data/snap_3.csv");
  XY_evolve(&lat, 1.0/T, 1 * lat.N, J, 0, &r);
  take_2dsnap(&lat, "./data/snap_4.csv");
  XY_evolve(&lat, 1.0/T, 1 * lat.N, J, 0, &r);
  take_2dsnap(&lat, "./data/snap_5.csv");

  XY_free(&lat);
  return 0;
}
