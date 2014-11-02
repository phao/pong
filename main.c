#include <assert.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <SDL2/SDL.h>

enum Direction {
  LEFT = -1,
  RIGHT = 1
};

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
  // dy and dx should be such that dy^2 + dx^2 is 1. Collectively, they form
  // the unit direction vector of the pong ball.
  float x, y, dx, dy;

  // Dimmensions of the frame within which the ball is.
  const struct Dimensions *within;
};

struct Racket {
  // dy should be 0, 1 or -1. It's only to indicate the direction of the
  // change in y.
  float y, dy;
};

struct Score {
  int player, enemy;
};

struct PlayState {
  struct Racket player, enemy;
  struct PongBall ball;
  Uint32 last_update;
  struct Score score;

  // Dimmensions of the frame within which the game is played.
  const struct Dimensions *frame;
};

struct Game {
  struct GameVideo video;
  struct PlayState play;
};

struct DigitRenderingContext {
  int x_offset; // Offset from the midline.
  enum Direction direction; // The direction to write.

  // Dimensions within which the digit is being written. Serves to compute the
  // midline.
  const struct Dimensions *within;
};

/** Size values are in pixels by default, unless specified otheriwse. */

enum {
  RACKET_WIDTH = 20,
  RACKET_HEIGHT = RACKET_WIDTH*3,
  RACKET_SPEED = 450 /* Pixels per second. */
};

const float RACKET_MS_SPEED = RACKET_SPEED/1000.0f;

enum {
  PONG_BALL_SIZE = 20,
  PONG_BALL_SPEED = 11*(RACKET_SPEED/10)
};

const float PONG_BALL_MS_SPEED = PONG_BALL_SPEED/1000.0f;

const float RACKET_HITBACK_MAXANGLE = 85.0f*M_PI/180.0f;

// The maximum distance of middles of racket and ball within a hit.
const float MAX_HIT_DISTANCE = RACKET_HEIGHT/2.0f + PONG_BALL_SIZE/2.0f;

enum {
  MIDLINE_POINT_WIDTH = 3,
  MIDLINE_POINT_HEIGHT = 2*MIDLINE_POINT_WIDTH,
  MIDLINE_POINT_MARGIN = 3,
  MIDLINE_PADDING = 20
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

// The distance the enemy is willing to tolerate and wait before he starts
// to go after the ball. If the ball is less than ENEMY_WAIT_TOLERANCE pixels
// above/below is center, it'll wait; otherwise it'll pursue the ball.
const float ENEMY_WAIT_TOLERANCE = RACKET_HEIGHT/5.0f;

enum {
  END_SCORE = 10
};

enum {
  DIGIT_PIECE_SIZE = 7,
  DIGIT_HEIGHT = 5, // In DIGIT_PIECE_SIZE units.
  DIGIT_WIDTH = 5, // In DIGIT_PIECE_SIZE units.
  DIGIT_INNER_MARGIN = 1, // In DIGIT_PIECE_SIZE units.
  DIGIT_OUTER_MARGIN = 2 // In DIGIT_PIECE_SIZE units.
};

const char SCORE_DIGITS[10][DIGIT_HEIGHT][DIGIT_WIDTH+1] = {
  {" ****",
   "*   *",
   "* * *",
   "*   *",
   "*****"},

  {"  *  ",
   " **  ",
   "  *  ",
   "  *  ",
   "*****"},

  {" ****",
   "    *",
   "*****",
   "*    ",
   "*****"},

  {"*****",
   "    *",
   " ****",
   "    *",
   "*****"},

  {"*   *",
   "*   *",
   "*****",
   "    *",
   "    *"},

  {"**** ",
   "*    ",
   "*****",
   "    *",
   "*****"},

  {"**** ",
   "*    ",
   "*****",
   "*   *",
   "*****"},

  {"*****",
   "*   *",
   "    *",
   "    *",
   "    *"},

  {"*****",
   "*   *",
   "*****",
   "*   *",
   "*****"},

  {"*****",
   "*   *",
   "*****",
   "    *",
   "    *"},
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
toggle_ball(struct PongBall *ball) {
  float angle;

  angle = rand()/(float)RAND_MAX * M_PI/1.8f - 5.0f*M_PI/18.0f;
  ball->dy = sinf(angle);
  ball->dx = ball->dx < 0 ? cosf(angle) : -cosf(angle);
}

void
reset_pong_ball(struct PongBall *ball) {
  ball->x = ball->within->width/2 - PONG_BALL_SIZE/2.0f;
  ball->y = ball->within->height/2 - PONG_BALL_SIZE/2.0f;
  toggle_ball(ball);
}

void
init_play(struct PlayState *p, const struct Dimensions *frame) {
  p->frame = frame;
  p->enemy.y = p->player.y = frame->height/2.0f - RACKET_HEIGHT/2.0f;
  p->ball.within = frame;
  p->ball.dx = 0;
  p->ball.dy = 0;
  reset_pong_ball(&p->ball);
  p->score = (struct Score) {
    .player = 0,
    .enemy = 0
  };
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

/**
 * Clamps x. If x < 0, then returns 0. If x > max, then returns max.
 */
float
fclamp0(float x, float max) {
  return x > max ? max : (x < 0.0f ? 0.0f : x);

  /* Note: check the folder 'mbench' for some microbenchmarks on different
   * methods for clamping. As of now, there are 4 different methods in there.
   * And this one is about 5-10% slower than the faster one (on my machine
   * according to the microbenchmarks). */
}

void
move_racket(struct Racket *r, Uint32 delta, float maxY) {
  r->y += delta * RACKET_MS_SPEED * r->dy;
  r->y = fclamp0(r->y, maxY);
}

void
move_pong_ball(struct PongBall *ball, Uint32 delta) {
  ball->x += ball->dx * delta * PONG_BALL_MS_SPEED;
  ball->y += ball->dy * delta * PONG_BALL_MS_SPEED;
  ball->x = fclamp0(ball->x, ball->within->width);
  ball->y = fclamp0(ball->y, ball->within->height - PONG_BALL_SIZE);
}

void
play_movements(struct PlayState *p, Uint32 delta) {
  move_racket(&p->player, delta, p->frame->height - RACKET_HEIGHT);
  move_racket(&p->enemy, delta, p->frame->height - RACKET_HEIGHT);
  move_pong_ball(&p->ball, delta);
}

void
play_enemy(struct Racket *enemy, const struct PongBall *ball) {
  float pong_middle_y, middle_y, diff, abs_diff;

  middle_y = enemy->y + RACKET_HEIGHT/2.0f;
  pong_middle_y = ball->y + PONG_BALL_SIZE/2.0f;
  diff = middle_y - pong_middle_y;
  abs_diff = fabsf(diff);
  if (abs_diff <= ENEMY_WAIT_TOLERANCE) {
    enemy->dy = 0;
  }
  else {
    enemy->dy = -diff / abs_diff;
  }
}

/**
 * This function does both the checking for racket/ball collision and the
 * update of the ball state if the collision indeed happened.
 *
 * @return 1 for if collision happened and 0 otherwise.
 */
int
ball_yhits_racket(struct PongBall *ball, const struct Racket *racket) {
  float by0, by1, mby;
  float ry0, ry1, mry;
  float mid_distance, angle;
  int happened;

  by0 = ball->y;
  by1 = ball->y + PONG_BALL_SIZE;
  ry0 = racket->y;
  ry1 = racket->y + RACKET_HEIGHT;

  happened = (ry0 < by0 && by0 < ry1) || (ry0 < by1 && by1 < ry1);
  if (!happened) {
    return 0;
  }

  // Generates an angle from -50deg to 50deg depending on where the ball hit
  // the racket. If in the upmost point then +50deg; if in the downmost point
  // then -50deg; 0deg if in the middle.
  //
  // The actual min/max angles are named constants.

  mby = by0 + PONG_BALL_SIZE/2.0f;
  mry = ry0 + RACKET_HEIGHT/2.0f;
  mid_distance = mry - mby;
  angle = RACKET_HITBACK_MAXANGLE * (mid_distance/MAX_HIT_DISTANCE);

  ball->dy = -sinf(angle); // Y increases as you go down, not up.
  ball->dx = ball->dx < 0 ? cosf(angle) : -cosf(angle);

  return 1;
}

void
score(struct PongBall *ball, int *benefit) {
  ++*benefit;
  reset_pong_ball(ball);
}

/**
 * Checks if there will be any collisions if a movement happens given the
 * current play state.
 */
void
run_collisions(struct PlayState *p, Uint32 delta) {
  struct PongBall *ball;
  float xp, yp; // These are x prime and y prime, the next (x,y) for ball.

  ball = &p->ball;
  xp = ball->x + ball->dx*delta*PONG_BALL_MS_SPEED;
  yp = ball->y + ball->dy*delta*PONG_BALL_MS_SPEED;

  // A ball can collide with the top/bottom walls, in which case its dy changes
  // sign.
  if (yp > p->frame->height - PONG_BALL_SIZE || yp < 0.0f) {
    ball->dy = -ball->dy;
  }

  // If a ball reaches the region before any racket...
  if (xp < RACKET_WIDTH) { // player
    if (!ball_yhits_racket(ball, &p->player)) {
      score(ball, &p->score.enemy);
    }
  }
  else if (xp > p->frame->width - RACKET_WIDTH - PONG_BALL_SIZE) { // enemy
    if (!ball_yhits_racket(ball, &p->enemy)) {
      score(ball, &p->score.player);
    }
  }
}

void
reset_game(struct PlayState *p) {
  p->last_update = SDL_GetTicks();
  init_play(p, p->frame);
}

void
play(struct PlayState *p, Uint32 now) {
  Uint32 delta;

  delta = now - p->last_update;
  run_collisions(p, delta);
  play_enemy(&p->enemy, &p->ball);
  play_movements(p, delta);
  p->last_update = now;
}

void
render_racket(SDL_Renderer *r, int x, int y) {
  SDL_Rect racket;

  racket = (SDL_Rect) {
    .x = x,
    .y = y,
    .w = RACKET_WIDTH,
    .h = RACKET_HEIGHT
  };
  SDL_RenderFillRect(r, &racket);
}

void
render_pong_ball(SDL_Renderer *r, const struct PongBall *ball) {
  SDL_Rect ball_rect;

  ball_rect = (SDL_Rect) {
    .x = ball->x,
    .y = ball->y,
    .w = PONG_BALL_SIZE,
    .h = PONG_BALL_SIZE
  };
  SDL_RenderFillRect(r, &ball_rect);
}

int
midline_npoints(const struct Dimensions *screen) {
  return 1 +
    (screen->height - MIDLINE_PADDING*2 - MIDLINE_POINT_HEIGHT) /
    (MIDLINE_POINT_MARGIN + MIDLINE_POINT_HEIGHT);
}

void
render_midline(SDL_Renderer *r, const struct Dimensions *screen) {
  int npoints;
  SDL_Rect mpoint;

  npoints = midline_npoints(screen);
  mpoint = (SDL_Rect) {
    .x = screen->width/2 - MIDLINE_POINT_WIDTH/2,
    .y = MIDLINE_PADDING,
    .w = MIDLINE_POINT_WIDTH,
    .h = MIDLINE_POINT_HEIGHT
  };
  SDL_RenderFillRect(r, &mpoint);
  while (--npoints > 0) {
    mpoint.y += MIDLINE_POINT_HEIGHT + MIDLINE_POINT_MARGIN;
    SDL_RenderFillRect(r, &mpoint);
  }
}

void
render_digit(SDL_Renderer *r,
             int digit,
             const struct DigitRenderingContext *cx)
{
  const char (*graphic)[DIGIT_HEIGHT][DIGIT_WIDTH+1];
  SDL_Rect digit_rect;
  int i, j, sign_offset, acc_offset, midline_x;

  digit_rect.w = digit_rect.h = DIGIT_PIECE_SIZE;
  graphic = &SCORE_DIGITS[digit];
  sign_offset = cx->direction == LEFT ? 0 : DIGIT_WIDTH*DIGIT_PIECE_SIZE;
  midline_x = cx->within->width/2;
  acc_offset = midline_x + cx->x_offset + sign_offset;

  for (i = 0; i < DIGIT_HEIGHT; i++) {
    for (j = 0; j < DIGIT_WIDTH; j++) {
      if ((*graphic)[i][DIGIT_WIDTH-j-1] == '*') {
        digit_rect.y = (DIGIT_OUTER_MARGIN + i)*DIGIT_PIECE_SIZE;
        digit_rect.x = acc_offset - (j+1)*DIGIT_PIECE_SIZE;
        SDL_RenderFillRect(r, &digit_rect);
      }
    }
  }
}

void
render_single_score(SDL_Renderer *r,
                    int score,
                    struct DigitRenderingContext *cx)
{
  int first_offset, second_offset;

  assert(score >= 0);
  assert(score <= 10);

  first_offset = cx->direction * DIGIT_OUTER_MARGIN*DIGIT_PIECE_SIZE;
  second_offset = cx->direction *
    DIGIT_PIECE_SIZE*(DIGIT_OUTER_MARGIN + DIGIT_INNER_MARGIN + DIGIT_WIDTH);

  if (cx->direction == RIGHT && score == 10) {
    first_offset = first_offset ^ second_offset;
    second_offset = first_offset ^ second_offset;
    first_offset = first_offset ^ second_offset;
  }

  cx->x_offset = first_offset;
  render_digit(r, score%10, cx);
  if (score == 10) {
    cx->x_offset = second_offset;
    render_digit(r, 1, cx);
  }
}

void
render_score(SDL_Renderer *r,
             const struct Score *s,
             const struct Dimensions *screen)
{
  struct DigitRenderingContext cx = {
    .x_offset = 0,
    .direction = LEFT,
    .within = screen
  };
  render_single_score(r, s->player, &cx);
  cx.direction = RIGHT;
  render_single_score(r, s->enemy, &cx);
}

void
render(SDL_Renderer *r, const struct PlayState *p) {
  SDL_SetRenderDrawColor(r, BG_R, BG_G, BG_B, BG_A);
  SDL_RenderClear(r);
  SDL_SetRenderDrawColor(r, FG_R, FG_G, FG_B, FG_A);
  render_racket(r, 0, p->player.y);
  render_racket(r, p->frame->width - RACKET_WIDTH, p->enemy.y);
  render_pong_ball(r, &p->ball);
  render_midline(r, p->frame);
  render_score(r, &p->score, p->frame);
  SDL_RenderPresent(r);
}

void
check_finish_round(struct Game *g) {
  struct Score *s;

  s = &g->play.score;
  if (s->player >= END_SCORE || s->enemy >= END_SCORE) {
    SDL_ShowSimpleMessageBox(
      SDL_MESSAGEBOX_INFORMATION,
      "End Round",
      s->player >= END_SCORE ?
        "You won! Go another round." :
        "You lost. Try again.",
      g->video.window
    );
    reset_game(&g->play);
  }
}

void
game_main(struct Game *g) {
  enum {
    FPS = 60,
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
    play(&g->play, SDL_GetTicks());
    render(g->video.renderer, &g->play);
    delta = SDL_GetTicks() - last;
    if (delta < MAX_WAIT) {
      SDL_Delay(MAX_WAIT - delta);
    }
    check_finish_round(g);
  }
}

void
run_game(struct Game* g, const char *title) {
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
  const char *TITLE = "PONG";

  struct Game game;

  game.video.dim = (struct Dimensions) {
    .width = 640,
    .height = 480
  };

  run_game(&game, TITLE);
  if (errorFn) {
    fprintf(stderr, "Error: %s\n", errorFn());
    return 1;
  }
  return 0;
}
