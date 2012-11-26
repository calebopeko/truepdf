#include "presenter.h"
#include "console.h"

#include <SDL/SDL.h>

Presenter::Presenter(int w, int h, const std::string& filename)
{
  init(w, h, filename);
}

void Presenter::init(int w, int h, const std::string& file)
{
  width = w;
  height = h;
  filename = file;
  document.open(filename);

  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE );
  if ( !screen ) {
    console::err() << "Unable to initialize SDL screen!" << std::endl;
  }
}

bool Presenter::poll()
{
  SDL_Event ev;
  while ( SDL_PollEvent( &ev ) ) {
    switch (ev.type) {
    case SDL_KEYDOWN:
      if ( ev.key.keysym.sym == SDLK_ESCAPE ) {
	return false;
      }
      break;
    case SDL_QUIT:
      return false;
    }
  }
  return true;
}

void Presenter::run()
{
  SDL_Flip(screen);
  while ( poll() ) {
    SDL_Delay(10);
  }
}
