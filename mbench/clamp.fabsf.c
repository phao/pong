#include <stdlib.h>
#include <math.h>

#include "common.h"

float fclamp0(float x, float max) {
  x = (x + max - fabsf(x - max))/2.0f;
  return (x + fabsf(x))/2.0f;
}

int main() {
  float *fs;
  int i;

  fs = malloc(SIZE * sizeof (float));
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
