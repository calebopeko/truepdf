#include "options.h"
#include "console.h"
#include "pdf.h"

#include <SDL/SDL.h>

bool poll()
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

int main(int argc, char** argv)
{
  Options options(argc, argv);

  const int width = options["width"];
  const int pageNumber = options["page"];
  std::string filename = options["file"];

  if ( filename == "" ) {
    options.printUsage();
    return 0;
  }

  // GTK Stuff
  gtk_init (&argc, &argv);

  Document document(filename);

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(width, width, 32, SDL_SWSURFACE );

  // unsigned char* data = (unsigned char*) screen->pixels;
  // cairo_surface_t *surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_RGB24, pageWidth*zoom, pageHeight*zoom, screen->pitch);

  // cairo_t* context = cairo_create(surface);
  // cairo_save(context);
  // cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
  // cairo_rectangle(context, 0, 0, pageWidth*zoom, pageHeight*zoom);
  // cairo_fill(context);
  // cairo_restore(context);
  // cairo_save(context);
  // cairo_scale(context, zoom, zoom);
  // poppler_page_render(page, context);
  // cairo_destroy(context);
  // cairo_surface_destroy(surface);

  SDL_Flip(screen);
  while ( poll() ) {
    SDL_Delay(10);
  }

  return 0;
}

