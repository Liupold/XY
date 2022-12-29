/* Original: Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)
 * modified: in 2022 by Rohn chatterjee (rohn.ch@gmail.com)
*/

#include <stdint.h>

static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

typedef struct {
  uint64_t s[4];
} xor256s_t;

uint64_t next(xor256s_t *state) {
  const uint64_t result = state->s[0] + state->s[3];
  const uint64_t t = state->s[1] << 17;

  state->s[2] ^= state->s[0];
  state->s[3] ^= state->s[1];
  state->s[1] ^= state->s[2];
  state->s[0] ^= state->s[3];

  state->s[2] ^= t;

  state->s[3] = rotl(state->s[3], 45);

  return result;
}


/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */

void jump(xor256s_t *state) {
  static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

  uint64_t s0 = 0;
  uint64_t s1 = 0;
  uint64_t s2 = 0;
  uint64_t s3 = 0;
  for(long unsigned int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
    for(int b = 0; b < 64; b++) {
      if (JUMP[i] & UINT64_C(1) << b) {
        s0 ^= state->s[0];
        s1 ^= state->s[1];
        s2 ^= state->s[2];
        s3 ^= state->s[3];
      }
      next(state);
    }

  state->s[0] = s0;
  state->s[1] = s1;
  state->s[2] = s2;
  state->s[3] = s3;
}


/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */

void long_jump(xor256s_t *state) {
  static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

  uint64_t s0 = 0;
  uint64_t s1 = 0;
  uint64_t s2 = 0;
  uint64_t s3 = 0;
  for(long unsigned int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
    for(int b = 0; b < 64; b++) {
      if (LONG_JUMP[i] & UINT64_C(1) << b) {
        s0 ^= state->s[0];
        s1 ^= state->s[1];
        s2 ^= state->s[2];
        s3 ^= state->s[3];
      }
      next(state);
    }

  state->s[0] = s0;
  state->s[1] = s1;
  state->s[2] = s2;
  state->s[3] = s3;
}

uint64_t smx64_next(uint64_t x) {
  uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}


xor256s_t xor256s_init(uint64_t x) {
  xor256s_t state;
  for (int i=0; i < 4; i++) {
    x = smx64_next(x);
    state.s[i] = x;
  }
  return state;
}

double rand_uni(xor256s_t *state) {
  return (double) (next(state) >> 11) * 0x1.0p-53;
}

uint64_t rand_u64(xor256s_t *state) {
  return next(state);
}
