#include <stdlib.h>
#include <math.h>

#include "common.h"

double fclamp0(double x, double max) {
  x = (x + max - fabs(x - max))/2.0;
  return (x + fabs(x))/2.0;
}

int main() {
  double *fs;
  int i;

  fs = malloc(SIZE * sizeof (double));
  srand(SEED);
  for (i = 0; i < SIZE; i++) {
    fs[i] = rand()/(double)RAND_MAX;
    fs[i] *= 2.0;
    fs[i] -= 1.0;
    fs[i] = fclamp0(fs[i], D_MAX);
  }
  free(fs);
  return 0;
}
