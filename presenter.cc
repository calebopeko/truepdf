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

  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE );
  if ( !screen ) {
    console::err() << "Unable to initialize SDL screen!" << std::endl;
  }

  document.open(filename);
  document.render(width);
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

void Presenter::render()
{
  const int i=0;
  SDL_Surface* src = document[i].getSurface();
  SDL_Rect s;
  s.x = 0; s.y = 0; s.w = src->w; s.h = src->h;
  SDL_BlitSurface(src, &s, screen, NULL);
  SDL_Flip(screen);
}

void Presenter::run()
{
  render();
  while ( poll() ) {
    SDL_Delay(10);
  }
}
