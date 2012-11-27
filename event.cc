#include "event.h"
#include "presenter.h"
#include "console.h"

Event Event::instance_;

void Event::init(int fpsc)
{
  // hardcoded standard keymapping
  keyMap[SDLK_UP] = KeyDef(Key_ScrollUp, true);
  keyMap[SDLK_DOWN] = KeyDef(Key_ScrollDown, true);
  keyMap[SDLK_PAGEUP] = KeyDef(Key_PageUp, false);
  keyMap[SDLK_PAGEDOWN] = KeyDef(Key_PageDown, false);
  keyMap[SDLK_HOME] = KeyDef(Key_Home, false);
  keyMap[SDLK_END] = KeyDef(Key_End, false);
  keyMap[SDLK_r] = KeyDef(Key_Reopen, false);
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
  const double scrollSpeed = Presenter::instance().getDocument().pageHeight() * 0.5; // in pixel per second
  // handle continuously pressed keys
  bool renderChange = false;
  if ( keys[Key_ScrollUp] ) {
    Presenter::instance().setPosition(Presenter::instance().getPosition() - scrollSpeed*ifps);
    renderChange = true;
  } else if ( keys[Key_ScrollDown] ) {
    Presenter::instance().setPosition(Presenter::instance().getPosition() + scrollSpeed*ifps);
    renderChange = true;
  }

  if ( renderChange ) {
    Presenter::instance().render();
  }
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
  case Key_PageDown:
    Presenter::instance().setPosition(Presenter::instance().getPosition() + Presenter::instance().height);
    Presenter::instance().render();
    break;
  case Key_PageUp:
    Presenter::instance().setPosition(Presenter::instance().getPosition() - Presenter::instance().height);
    Presenter::instance().render();
    break;
  case Key_Home:
    Presenter::instance().setPosition(0);
    Presenter::instance().render();
    break;
  case Key_End:
    console::out() << "Not yet implemented!" << std::endl;
    break;
  case Key_Reopen:
    Presenter::instance().reopen();
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
  if ( mouseLeftDown ) {
    const int delta = y - mouseLastY;
    if ( delta != 0 ) {
      Presenter::instance().setPosition(Presenter::instance().getPosition() - delta);
      Presenter::instance().render();
    }
    mouseLastX = x;
    mouseLastY = y;
  }
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
      if (ev.button.button == SDL_BUTTON_LEFT) {
	mouseLeftDown = true;
	mouseLastX = ev.button.x;
	mouseLastY = ev.button.y;
      }
      // if (ev.button.button == SDL_BUTTON_RIGHT) {
      // }
      break;
    case SDL_MOUSEBUTTONUP:
      if (SDL_BUTTON(ev.button.button) & SDL_BUTTON_LEFT) {
	mouseLeftDown = false;
      }
      break;
    case SDL_MOUSEMOTION:
      mouseMove(ev.button.x, ev.button.y);
      break;
    case SDL_VIDEORESIZE:
      {
	Presenter::instance().resize(ev.resize.w, ev.resize.h);
	Presenter::instance().render();
      }
      break;
    case SDL_QUIT:
      return false;
    }
  }
  keyPressed();

  return running;
}
