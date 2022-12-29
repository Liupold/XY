#include <stdint.h>

uint64_t ipow(uint32_t r, uint32_t d) {
  uint64_t x = 1;
  for (uint32_t m = 0; m < d; m++) {
    x *= r;
  }
  return x;
}
