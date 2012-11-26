#include "presenter.h"
#include "console.h"

#include <cmath>
#include <SDL/SDL.h>

Presenter Presenter::instance_;

Presenter::Presenter(int w, int h, const std::string& filename)
    : position(0), width(0), height(0), document(), screen(NULL), transitionSpace(2)
{
  init(w, h, filename);
}

void Presenter::init(int w, int h, const std::string& file)
{
  width = w;
  height = h;
  filename = file;

  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF );
  if ( !screen ) {
    console::err() << "Unable to initialize SDL screen!" << std::endl;
  }

  document.open(filename);
  document.render(width);
}

void Presenter::render()
{
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

  SDL_Flip(screen);
}

void Presenter::run()
{
  Event& event = Event::instance();
  render();
  while ( event.poll() ) {
    event.fillFrame();
  }
}
