#include <assert.h>
#include <emscripten.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

extern void new_sprite(int key, int x, int y, int w, int h, int color);
extern void set_sprite_position(int key, int x, int y);
extern void set_sprite_color(int key, int color);
extern void free_sprite(int key);

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

void mouse_down(int x, int y) {
  tceu__int__int payload = { x, y };
  ceu_sys_go( &app, CEU_IN_MOUSE_DOWN, &payload );
}

void mouse_up(int x, int y) {
  tceu__int__int payload = { x, y };
  ceu_sys_go( &app, CEU_IN_MOUSE_UP, &payload );
}

void mouse_move(int x, int y) {
  tceu__int__int payload = { x, y };
  ceu_sys_go( &app, CEU_IN_MOUSE_MOVE, &payload );
}

void update(int ms) {
  tceu__int payload = { ms };
  ceu_sys_go( &app, CEU_IN_UPDATE, &payload );
}

void begin() {
  memset(CEU_DATA, 0, sizeof(CEU_Main));
  app.data = (tceu_org*) &CEU_DATA;
  app.init = &ceu_app_init;
  app.init(&app);
}
