#include "presenter.h"
#include "console.h"

#include <cmath>
#include <SDL/SDL.h>

Presenter Presenter::instance_;

Presenter::Presenter(int w, int h, const std::string& filename)
    : width(0), height(0), position(0), document(), screen(NULL), transitionSpace(2)
{
  init(w, h, filename);
}

void Presenter::init(int w, int h, const std::string& file)
{
  filename = file;

  document.open(filename);
  resize(w,h);
}

void Presenter::reopen()
{
  document.open(filename);
  document.render(width);
  render();
}

void Presenter::resize(int w, int h)
{
  bool changedWidth = (w != width);
  double oldWidth = width;
  width = w;
  height = h;

  SDL_Init(SDL_INIT_VIDEO);
  // screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
  if ( !screen ) {
    console::err() << "Unable to initialize SDL screen!" << std::endl;
  }

  if ( changedWidth ) {
    if ( oldWidth !=  0 ) {
      position *= ((double)width)/oldWidth;
    }
    document.render(width);
  }
}

void Presenter::render()
{
  SDL_FillRect(screen, NULL, 0);

  const int pageHeight = document[0].getSurface()->h; // TODO: multiple heights per document!
  const int currentPage = position/pageHeight;
  if ( currentPage < document.pageCount ) {
    const int offset = position-currentPage*pageHeight;
    SDL_Surface* src = document[currentPage].getSurface();
    SDL_Rect s;
    s.x = 0; s.y = offset; s.w = src->w; s.h = std::min(src->h - offset, height);
    SDL_BlitSurface(src, &s, screen, NULL);

    int renderPos = std::min(src->h - offset, height) + transitionSpace;
    int pageOffset = 1;
    while ( renderPos < height && currentPage + pageOffset < document.pageCount ) {
      SDL_Surface* next = document[currentPage+pageOffset].getSurface();
      SDL_Rect nextRec, nextDest;
      nextRec.x = 0; nextRec.y = 0; nextRec.w = next->w; nextRec.h = std::min(height - renderPos, next->h);
      nextDest.x = 0; nextDest.y = renderPos;
      SDL_BlitSurface(next, &nextRec, screen, &nextDest);
      renderPos += next->h + transitionSpace;
      pageOffset++;
    }
  }

  SDL_Flip(screen);
}

void Presenter::run()
{
  Event& event = Event::instance();
  render();
  while ( event.poll() ) {
    event.fillFrame();

    // if ( event.diag(1000) ) {
    //   console::out() << event.getFps() << std::endl;
    // }    
  }
}
