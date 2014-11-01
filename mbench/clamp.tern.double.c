#include <stdlib.h>

#include "common.h"

double fclamp0(double x, double max) {
  return x > max ? max : (x < 0.0 ? 0.0 : x);
}

int main() {
  double *fs = malloc(SIZE * sizeof (double));
  int i;

  srand(SEED);
  for (i = 0; i < SIZE; i++) {
    fs[i] = rand()/(double)RAND_MAX;
    fs[i] *= 2.0;
    fs[i] -= 1.0;
    fs[i] = fclamp0(fs[i], D_MAX);
  }
  return 0;
}
