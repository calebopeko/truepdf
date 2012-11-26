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
  position = 800;
  const double pageHeight = document[0].height; // TODO: multiple heights per page!
  const int currentPage = position/pageHeight;
  const int offset = position-currentPage*pageHeight;
  SDL_Surface* src = document[currentPage].getSurface();
  SDL_Rect s, d;
  s.x = 0; s.y = offset; s.w = src->w; s.h = src->h;
  d.x = 0; d.y = 0;
  SDL_BlitSurface(src, &s, screen, &d);
  SDL_Flip(screen);
}

void Presenter::run()
{
  render();
  while ( poll() ) {
    SDL_Delay(10);
  }
}
