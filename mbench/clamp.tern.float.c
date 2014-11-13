#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "common.h"

static float fclamp0(float x, float max) {
  return x > max ? max : (x < 0.0f ? 0.0f : x);
}

int main() {
  float *fs = malloc(SIZE * sizeof (float));
  int i;
  Uint64 c, acc;

  srand(SEED);
  acc = 0;
  for (i = 0; i < SIZE; i++) {
    fs[i] = rand()/(float)RAND_MAX;
    fs[i] *= 2.0f;
    fs[i] -= 1.0f;
    c = SDL_GetPerformanceCounter();
    fs[i] = fclamp0(fs[i], F_MAX);
    acc += SDL_GetPerformanceCounter() - c;
  }
  free(fs);
  printf("%f secs\n", acc/(double) SDL_GetPerformanceFrequency());
  return 0;
}
