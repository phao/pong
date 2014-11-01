#include <stdio.h>

#include <SDL2/SDL.h>

typedef const char* (*ErrorFn)(void);

struct Dimensions {
  int width, height;
};

struct GameVideo {
  SDL_Window *window;
  SDL_Renderer *renderer;
  struct Dimensions dim;
};

struct PongBall {
  float x, y, dx, dy;
};

struct Racket {
  float y, dy;
};

struct PlayState {
  struct Racket player, enemy;
  struct PongBall pong;
  Uint32 last_update;
};

struct Game {
  struct GameVideo video;
  struct PlayState play;
};

enum {
  RACKET_WIDTH = 20,
  RACKET_HEIGHT = RACKET_WIDTH*2,
  PONG_BALL_SIZE = 20,
  MIDLINE_POINT_WIDTH = 3,
  MIDLINE_POINT_HEIGHT = 2*MIDLINE_POINT_WIDTH,
  MIDLINE_POINT_MARGIN = 3,
  MIDLINE_PADDING = 20,
  RACKET_SPEED = 250 /* Pixels per second. */
};

enum {
  FG_R = 255,
  FG_G = 255,
  FG_B = 255,
  FG_A = 255
};

enum {
  BG_R = 0,
  BG_G = 0,
  BG_B = 0,
  BG_A = 255
};

ErrorFn errorFn = 0;

void
init_video(struct GameVideo* gv, const char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    gv->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, gv->dim.width, gv->dim.height,
      SDL_WINDOW_SHOWN);
    if (gv->window) {
      gv->renderer = SDL_CreateRenderer(gv->window, -1,
        SDL_RENDERER_ACCELERATED);
      if (gv->renderer) {
        return;
      }
      SDL_DestroyWindow(gv->window);
    }
    SDL_Quit();
  }
  errorFn = SDL_GetError;
}

void
init_play(struct PlayState *p, const struct Dimensions *screen) {
  p->enemy.y = p->player.y = screen->height/2 - RACKET_HEIGHT/2;
  p->pong.dx = -1;
  p->pong.dy = 0;
  p->pong.x = screen->width/2 - PONG_BALL_SIZE/2;
  p->pong.y = screen->height/2 - PONG_BALL_SIZE/2;
}

void
quit_game(struct Game *g) {
  SDL_DestroyRenderer(g->video.renderer);
  SDL_DestroyWindow(g->video.window);
  SDL_Quit();
}

void
handle_event(struct PlayState *p, SDL_Event *e) {
  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_UP:
        p->player.dy = -1;
        return;
      case SDLK_DOWN:
        p->player.dy = 1;
        return;
    }
  }
  p->player.dy = 0;
}

void
move_racket(struct Racket *r, Uint32 delta, float minY, float maxY) {
  r->y += delta * (float)RACKET_SPEED/1000.0f * r->dy;
  if (r->y < minY) {
    r->y = minY;
  }
  else if (r->y > maxY) {
    r->y = maxY;
  }
}

void
play(struct PlayState *p, const struct Dimensions *screen, Uint32 now) {
  Uint32 delta;

  delta = now - p->last_update;
  move_racket(&p->player, delta, 0, screen->height-RACKET_HEIGHT);
  move_racket(&p->enemy, delta, 0, screen->height-RACKET_HEIGHT);
  p->last_update = now;
}

void
render_racket(SDL_Renderer *r, int x, int y) {
  SDL_Rect racket;

  racket.x = x;
  racket.y = y;
  racket.w = RACKET_WIDTH;
  racket.h = RACKET_HEIGHT;
  SDL_RenderFillRect(r, &racket);
}

void
render_pong_ball(SDL_Renderer *r, const struct PongBall *pong) {
  SDL_Rect ball;

  ball.x = pong->x;
  ball.y = pong->y;
  ball.w = PONG_BALL_SIZE;
  ball.h = PONG_BALL_SIZE;
  SDL_RenderFillRect(r, &ball);
}

int
midline_npoints(const struct Dimensions *screen) {
  return 1 +
    (screen->height - MIDLINE_PADDING*2 - MIDLINE_POINT_HEIGHT) /
    (MIDLINE_POINT_MARGIN + MIDLINE_POINT_HEIGHT);
}

void
render_midline(SDL_Renderer *r, const struct Dimensions *screen) {
  int npoints, i;
  SDL_Rect mpoint;

  npoints = midline_npoints(screen);
  mpoint.x = screen->width/2 - MIDLINE_POINT_WIDTH/2;
  mpoint.y = MIDLINE_PADDING;
  mpoint.w = MIDLINE_POINT_WIDTH;
  mpoint.h = MIDLINE_POINT_HEIGHT;
  SDL_RenderFillRect(r, &mpoint);
  npoints--;
  for (i = 0; i < npoints; i++) {
    mpoint.y += MIDLINE_POINT_HEIGHT + MIDLINE_POINT_MARGIN;
    SDL_RenderFillRect(r, &mpoint);
  }
}

void
render(SDL_Renderer *r,
       const struct PlayState *p,
       const struct Dimensions *screen)
{
  SDL_SetRenderDrawColor(r, BG_R, BG_G, BG_B, BG_A);
  SDL_RenderClear(r);
  SDL_SetRenderDrawColor(r, FG_R, FG_G, FG_B, FG_A);
  render_racket(r, 0, p->player.y);
  render_racket(r, screen->width - RACKET_WIDTH, p->enemy.y);
  render_pong_ball(r, &p->pong);
  render_midline(r, screen);
  SDL_RenderPresent(r);
}

void
game_main(struct Game *g) {
  enum {
    FPS = 30,
    SEC = 1000,
    MAX_WAIT = SEC/FPS
  };

  SDL_Event e;
  Uint32 last, delta;

  g->play.last_update = SDL_GetTicks();
  for (;;) {
    last = SDL_GetTicks();
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        return;
      }
      handle_event(&g->play, &e);
    }
    play(&g->play, &g->video.dim, SDL_GetTicks());
    render(g->video.renderer, &g->play, &g->video.dim);
    delta = SDL_GetTicks() - last;
    if (delta < MAX_WAIT) {
      SDL_Delay(MAX_WAIT - delta);
    }
  }
}

void
start_game(struct Game* g, const char *title) {
  init_video(&g->video, title);
  if (errorFn) {
    return;
  }
  init_play(&g->play, &g->video.dim);
  game_main(g);
  quit_game(g);
}

int
main(int argc, char *argv[]) {
  const char *TITLE = "Pong";

  struct Game game;

  game.video.dim.width = 640;
  game.video.dim.height = 480;

  start_game(&game, TITLE);
  if (errorFn) {
    fprintf(stderr, "Error: %s\n", errorFn());
    return 1;
  }
  return 0;
}
