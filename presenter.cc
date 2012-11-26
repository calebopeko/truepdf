#include "presenter.h"
#include "console.h"

#include <cmath>
#include <SDL/SDL.h>

Presenter::Presenter(int w, int h, const std::string& filename)
    : width(0), height(0), document(), screen(NULL), position(0), transitionSpace(2)
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
  position = 500;
  const double pageHeight = document[0].height; // TODO: multiple heights per document!
  const int currentPage = position/pageHeight;
  const int offset = position-currentPage*pageHeight;
  SDL_Surface* src = document[currentPage].getSurface();
  SDL_Rect s;
  s.x = 0; s.y = offset; s.w = src->w; s.h = std::min(src->h - offset, height);
  SDL_BlitSurface(src, &s, screen, NULL);

  int renderPos = std::min(src->h - offset, height) + transitionSpace;
  int pageOffset = 1;
  while ( renderPos < height ) { // TODO: Check for end of page list
    SDL_Surface* next = document[currentPage+pageOffset].getSurface();
    SDL_Rect nextRec, nextDest;
    nextRec.x = 0; nextRec.y = 0; nextRec.w = next->w; nextRec.h = std::min(height - renderPos, next->h);
    nextDest.x = 0; nextDest.y = renderPos;
    SDL_BlitSurface(next, &nextRec, screen, &nextDest);
    renderPos += next->h + transitionSpace;
    pageOffset++;
  }
  if ( pageHeight - offset < height ) {
    
  }

  SDL_Flip(screen);
}

void Presenter::run()
{
  render();
  while ( poll() ) {
    SDL_Delay(10);
  }
}
