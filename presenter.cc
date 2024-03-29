#include "presenter.h"
#include "console.h"

#include <cmath>
#include <algorithm>
#include <SDL/SDL.h>

Presenter Presenter::instance_;

void Presenter::init(int w, int h, const std::string& file, bool pre, const std::string& c)
{
  SDL_Init(SDL_INIT_VIDEO);

  if ( c == "dark" ) {
    colored = COLOR_SOLARIZED_DARK;
  } else if ( c == "light" ) {
    colored = COLOR_SOLARIZED_LIGHT;
  }

  preRender = pre;
  filename = file;

  document.open(filename);
  resize(w,h);
}

void Presenter::reopen()
{
  document.open(filename);
  document.setWidth(width);
  render();
}

void Presenter::resize(int w, int h)
{
  bool changedWidth = (w != width);
  double oldWidth = width;
  width = w;
  height = h;

  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
  if ( !screen ) {
    console::err() << "Unable to initialize SDL screen!" << std::endl;
  }

  if ( changedWidth ) {
    if ( oldWidth !=  0 ) {
      position *= ((double)width)/oldWidth;
      clamp();
    }
    document.setWidth(width);
    if ( preRender ) {
      document.render(colored);
    }
  }
}

int Presenter::renderPage(int src, int dest, int page)
{
  if ( page < 0 || page >= document.pageCount ) return document.pageHeight();
  if ( !document[page].isRendered() ) {
    document[page].render(colored);
  }

  SDL_Surface* srcSurf = document[page].getSurface();
  SDL_Rect srcRect;
  srcRect.x = 0;
  srcRect.y = src;
  srcRect.w = srcSurf->w;
  srcRect.h = std::min(srcSurf->h - src, height);
  SDL_Rect destRect;
  destRect.x = 0;
  destRect.y = dest;
  destRect.w = srcRect.w;
  destRect.h = srcRect.h;

  SDL_BlitSurface(srcSurf, &srcRect, screen, &destRect);

  usedPages.push_back(page);

  return srcRect.h;
}

void Presenter::render()
{
  usedPages.clear();
  SDL_FillRect(screen, NULL, 0);
  
  const int pageHeight = document.pageHeight(); // TODO: multiple heights per document
  int currentPage = position/pageHeight;
  int srcPos = position-currentPage*pageHeight;
  int destPos = 0;
  do {
    const int renderHeight = renderPage(srcPos, destPos, currentPage);
    srcPos = 0;
    destPos += renderHeight + transitionSpace;
    currentPage++;
  } while ( destPos < height && currentPage < document.pageCount );
    
  SDL_Flip(screen);
}

void Presenter::clearCache()
{
  std::list<int>::const_iterator it = usedPages.begin();
  for ( int i=0; i < document.pageCount; ++i ) {
    if ( i == *it ) {
      ++it;
    } else {
      if ( document[i].isRendered() ) {
	if ( console::verbose() ) console::out() << "Freeing page " << i << std::endl;
	document[i].unrender();	
      }
    }
  }
}

void Presenter::run()
{
  Event& event = Event::instance();
  render();
  while ( event.poll() ) {
    event.fillFrame();

    if ( event.diag(1000) ) {
      if ( console::verbose() ) {
	console::out() << event.getFps() << " with " << document.renderedPages() << " rendered pages." << std::endl;
	if ( !preRender ) clearCache();
      }
    }    
  }
}

void Presenter::clamp()
{ 
  const int maxPos = (document.pageCount*document.pageHeight()) - height;
  if ( position > maxPos ) position = maxPos;
  if ( position < 0 ) position = 0;
}
