#include <stdlib.h>

#include "common.h"

float fclamp0(float x, float max) {
  return x > max ? max : (x < 0.0f ? 0.0f : x);
}

int main() {
  float *fs = malloc(SIZE * sizeof (float));
  int i;

  srand(SEED);
  for (i = 0; i < SIZE; i++) {
    fs[i] = rand()/(float)RAND_MAX;
    fs[i] *= 2.0f;
    fs[i] -= 1.0f;
    fs[i] = fclamp0(fs[i], F_MAX);
  }
  free(fs);
  return 0;
}
