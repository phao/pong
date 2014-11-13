#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "common.h"

static double fclamp0(double x, double max) {
  x = (x + max - fabs(x - max))/2.0;
  return (x + fabs(x))/2.0;
}

int main() {
  double *fs;
  int i;
  Uint64 c, acc;

  fs = malloc(SIZE * sizeof (double));
  srand(SEED);
  acc = 0;
  for (i = 0; i < SIZE; i++) {
    fs[i] = rand()/(double)RAND_MAX;
    fs[i] *= 2.0;
    fs[i] -= 1.0;
    c = SDL_GetPerformanceCounter();
    fs[i] = fclamp0(fs[i], D_MAX);
    acc += SDL_GetPerformanceCounter() - c;
  }
  free(fs);
  printf("%f secs\n", acc/(double) SDL_GetPerformanceFrequency());
  return 0;
}
