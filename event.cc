#include "event.h"

Event Event::instance_;

void Event::init(int fpsc)
{
  // hardcoded standard keymapping
  keyMap[SDLK_UP] = KeyDef(Key_ScrollUp, false);
  keyMap[SDLK_DOWN] = KeyDef(Key_ScrollDown, false);
  keyMap[SDLK_ESCAPE] = KeyDef(Key_Quit, false);

  // array for pressed keys
  for (int i=0; i<Key_Total; i++) keys[i]=false;

  fpsCap = fpsc;
  frameAim = 1000./fpsCap;
}

bool Event::diag(Uint32 ms)
{
  static Uint32 lastTrigger=0;
  Uint32 now;
  now = SDL_GetTicks();
  if ( now - lastTrigger > ms) {
    lastTrigger = now;
    return true;
  }
  return false;
}

void Event::fillFrame()
{
  static Uint32 last_frame;

  Uint32 now = SDL_GetTicks();
  Uint32 frameDur = (now - last_frame);
  if ( frameAim > frameDur ) SDL_Delay( frameAim - frameDur );

  frameDur = (SDL_GetTicks() - last_frame);
  last_frame = now;
  fps = 1000. / frameDur;
  ifps = 1./fps;
}
  
void Event::keyPressed()
{
  // handle continuously pressed keys
}

void Event::keyUp(SDL_Event& ev)
{
  SDLKey pressed = ev.key.keysym.sym;
  if ( keyMap.find(pressed) != keyMap.end() ) {
    KeyDef key = keyMap[pressed];
    if ( key.continuous ) {
      keys[key.index] = false;
    }
  }
}

void Event::keyDown(SDL_Event& ev)
{
  SDLKey pressed = ev.key.keysym.sym;

  if ( keyMap.find(pressed) != keyMap.end() ) {
    KeyDef key = keyMap[pressed];
    if ( key.continuous ) {
      keys[key.index] = true;
    } else {
      keySingle(key.index);
    }
  }
}

void Event::keySingle(KeyIndex key)
{
  switch (key) {
  case Key_ScrollUp:
    break;
  case Key_ScrollDown:
    break;
  case Key_Quit:
    running = false;
    break;
  default:
    break;
  }
}

void Event::mouseMove(int x, int y)
{
}

bool Event::poll()
{
  SDL_Event ev;
  
  while ( SDL_PollEvent( &ev ) ) {
    switch (ev.type) {
    case SDL_KEYDOWN:
      keyDown(ev);
      break;
    case SDL_KEYUP:
      keyUp(ev);
      break;
    case SDL_MOUSEBUTTONDOWN:
      // if (ev.button.button == SDL_BUTTON_LEFT) {
      // }
      // if (ev.button.button == SDL_BUTTON_RIGHT) {
      // }
      break;
    case SDL_MOUSEBUTTONUP:
      // if (SDL_BUTTON(ev.button.button) & SDL_BUTTON_LEFT) {
      // }
      break;
    case SDL_MOUSEMOTION:
      mouseMove(ev.button.x, ev.button.y);
      break;
    case SDL_QUIT:
      return false;
    }
  }
  keyPressed();

  return running;
}
