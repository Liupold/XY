#include <nd-xy.h>

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

double corr(XY_lat *lat, int del_ij_sq) {
  int *pos_i = (int *)calloc(lat->d, sizeof(int));
  int *pos_i_plus_j = (int *)calloc(lat->d, sizeof(int));
  int dist = 0, dist_x;
  double del_S = 0, count_j = 0;

  for (uint32_t k = 0; k < lat->d; k++) {
    pos_i[k] = 0;
    pos_i_plus_j[k] = 0;
  }

  for (int i = 0; i < (int)lat->N; i++) {
    for (int j = 0; j < (int)lat->N; j++) {
      if (i != j) {
        dist = 0;
        for (uint32_t k = 0; k < lat->d; k++) {
          dist_x = (pos_i[k] - pos_i_plus_j[k]);
          if (2 * dist_x > (int)lat->r) {
            dist_x = (int)lat->r - dist_x;
          }
          dist += dist_x * dist_x;
        }

        if (del_ij_sq == dist) {
          del_S += pow(lat->S[i] - lat->S[j], 2.0);
          count_j += 1;
        }
        // printf("%ld, %ld: %d [%ld, %ld]\n", i, j, dist,
        //     pos_i_plus_j[1], pos_i_plus_j[0]);
      }
      // increment addr.
      for (uint64_t k = 0; k < lat->d; k++) {
        if (pos_i_plus_j[k] < (int)lat->r - 1) {
          pos_i_plus_j[k] += 1;
          break;
        } else
          pos_i_plus_j[k] = 0;
      }
    }

    // increment addr.
    for (uint64_t k = 0; k < lat->d; k++) {
      if (pos_i[k] < (int)lat->r - 1) {
        pos_i[k] += 1;
        break;
      } else
        pos_i[k] = 0;
    }
  }
  free(pos_i);
  free(pos_i_plus_j);
  if (count_j == 0)
    return 0;
  return del_S / count_j;
}

double corr_2d(XY_lat *lat, int j_sq) {
  int dist_x, dist_sq = 0;
  uint64_t z1 = 0, z2 = 0, count = 0;
  double del_S = 0;
  for (int m1 = 0; m1 < (int)lat->r; m1++) {
    for (int n1 = 0; n1 < (int)lat->r; n1++) {
      z2 = 0;
      for (int m2 = 0; m2 < (int)lat->r; m2++) {
        for (int n2 = 0; n2 < (int)lat->r; n2++) {
          dist_sq = 0;
          if (!((m1 == m2) && (n1 == n2))) {
            dist_x = abs(m2 - m1);
            if (2 * dist_x > (int)lat->r) {
              dist_x = (int)lat->r - dist_x;
            }
            dist_sq += dist_x * dist_x;
            dist_x = abs(n2 - n1);
            if (2 * dist_x > (int)lat->r) {
              dist_x = (int)lat->r - dist_x;
            }
            dist_sq += dist_x * dist_x;
            if (dist_sq == j_sq) {
              del_S += pow(lat->S[z2] - lat->S[z1], 2.0);
              count++;
            }
          }
          z2++;
        }
      }
      z1++;
    }
  }
  return del_S / (double)count;
}

int main(void) {
  // uint32_t r_arr[4] = {32, 64, 128, 256};
  int j_max = 100, seed_avg = 20;

#pragma omp parallel for
  for (int del_j2 = 1; del_j2 < j_max; del_j2++) {
    double J[3] = {1.0, 1.0, 1.0};
    XY_lat lat = XY_init(2, 32);
    double th_C = 0, T = 0.2;
    for (uint32_t sd = 0; sd < seed_avg; sd++) {
      xor256s_t seed = xor256s_init(9842 + 2874982 * sd);
      double c_m = -1, p_m;
      XY_rand(&lat, &seed);
      do {
        p_m = c_m;
        XY_evolve(&lat, 1.0 / T, 10 * lat.N, J, 0, &seed);
        c_m = XY_dTheta(&lat);
      } while (fabs(c_m - p_m) > 1E-2);
      for (uint32_t _ = 0; _ < 100; _++) {
        th_C += corr(&lat, del_j2);
        XY_evolve(&lat, 1.0 / T, 100, J, 0, &seed);
      }
      if (th_C <= 0) {
        break;
      }
    }
    if (th_C > 0)
      printf("%f, %f\n", log(del_j2), th_C / (1000 * seed_avg));
    XY_free(&lat);
  }
  return 0;
}
