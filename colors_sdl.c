#include <assert.h>
#include <SDL/SDL.h>

#define countof(X) (sizeof(X) / sizeof(X[0]))
#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

struct Sprite {
  int key;
  struct SDL_Rect sdl_rect;
  int color;
  int alive;
};

struct Sprite rects[255] = {};
int head, tail;

void new_sprite(int key, int x, int y, int w, int h, int color) {
  struct Sprite* r;

  if (head - tail >= countof(rects)) {
    // no room, drop it
    return;
  }
  r = &(rects[(head - tail) % countof(rects)]);
  assert(!r->alive);
  r->key = key;
  r->sdl_rect.x = x;
  r->sdl_rect.y = y;
  r->sdl_rect.w = w;
  r->sdl_rect.h = h;
  r->color = color;
  r->alive = TRUE;
  head += 1;
}

void set_sprite_position(int key, int x, int y) {
  int i;

  for (i = 0; i < countof(rects); i += 1) {
    if (rects[i].alive && rects[i].key == key) {
      rects[i].sdl_rect.x = x;
      rects[i].sdl_rect.y = y;
    }
  }
}

void set_sprite_color(int key, int color) {
  int i;

  for (i = 0; i < countof(rects); i += 1) {
    if (rects[i].alive && rects[i].key == key) {
      rects[i].color = color;
    }
  }
}

void free_sprite(int key) {
  int i;

  for (i = 0; i < countof(rects); i += 1) {
    if (rects[i].alive && rects[i].key == key) {
      rects[i].alive = FALSE;
    }
  }
  while (TRUE) {
    if (rects[tail % 255].alive) {
      break;
    } else {
      tail += 1;
    }
  }
}

#define ceu_out_emit_NEW_SPRITE(X) new_sprite(X->_1, X->_2, X->_3, X->_4, X->_5, X->_6)
#define ceu_out_emit_SET_SPRITE_POSITION(X) set_sprite_position(X->_1, X->_2, X->_3)
#define ceu_out_emit_SET_SPRITE_COLOR(X) set_sprite_color(X->_1, X->_2)
#define ceu_out_emit_FREE_SPRITE(X) free_sprite(X->_1)

#define ceu_out_assert(v) ceu_sys_assert(v)
void ceu_sys_assert (int v) {
  assert(v);
}

#define ceu_out_log(m,s) ceu_sys_log(m,s)
void ceu_sys_log (int mode, long s) {
  switch (mode) {
  case 0:
    printf("%s", (char*)s);
    break;
  case 1:
    printf("%lX", s);
    break;
  case 2:
    printf("%ld", s);
    break;
  }
}

#include "_ceu_app.c"

byte CEU_DATA[sizeof(CEU_Main)];
tceu_app app;

SDL_Surface* screen;

void render() {
  int i;

  for (i = 0; i < countof(rects); i += 1) {
    if (rects[i].alive) {
      SDL_FillRect(screen, &rects[i].sdl_rect, rects[i].color);
    }
  }

}

void mainloop() {
  render();

  SDL_Flip(screen);

  // Poll for events, and handle the ones we care about.
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: exit(0); break;
    case SDL_MOUSEBUTTONDOWN:
      {
        tceu__int__int payload = { event.button.x, event.button.y };
        ceu_sys_go(&app, CEU_IN_MOUSE_DOWN, &payload);
      }
      break;
    case SDL_MOUSEBUTTONUP:
      {
        tceu__int__int payload = { event.button.x, event.button.y };
        ceu_sys_go(&app, CEU_IN_MOUSE_UP, &payload);
      }
      break;
    case SDL_MOUSEMOTION: 
      {
        tceu__int__int payload = { event.motion.x, event.motion.y };
        ceu_sys_go(&app, CEU_IN_MOUSE_MOVE, &payload);
      }
      break;
    }
  }

  {
    tceu__int payload = { SDL_GetTicks() }; 
    ceu_sys_go(&app, CEU_IN_UPDATE, &payload);
  }

  SDL_Delay(0);
}

int main(int argc, char* argv[]) {
  app.data = (tceu_org*) &CEU_DATA;
  app.init = &ceu_app_init;
  app.init(&app);

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
  if (screen == NULL) {
    exit(1);
  }
  while (1) {
    mainloop();
  }
  return 0;
}
