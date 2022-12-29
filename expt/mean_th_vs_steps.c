#include <nd-xy.h>

// ---------------------------------------------------
double mean_Theta(XY_lat *lat) {
  double th = 0;

  for (uint64_t i = 0; i < lat->N; i++) {
    th += lat->S[i];
  }
  return th / (double)lat->N;
}
// ---------------------------------------------------

int main(void) {
  XY_lat lat;
  double T = 0.5;
  double J[3] = {1.0, 1.0};
  uint64_t MC_steps = 0;

  xor256s_t seed = xor256s_init(7938090);
  lat = XY_init(2, 8);

  XY_rand(&lat, &seed);

  for (int j=0; j < 100; j++) {
    printf("%ld, %f\n", MC_steps, mean_Theta(&lat));
    XY_evolve(&lat, 1.0/T, 1000 * 64 * 64, J, 0, &seed);
    MC_steps += 1000;
  }
  XY_free(&lat);

  return 0;
}
