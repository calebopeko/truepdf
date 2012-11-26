#include "options.h"
#include "console.h"

#include <gtk/gtk.h>
#include <poppler.h>
#include <SDL/SDL.h>

gchar* getAbsoluteFileName (const gchar *fileName)
{
  gchar *absoluteFileName = NULL;
  if ( g_path_is_absolute(fileName) ) {
      absoluteFileName = g_strdup (fileName);
  } else {
    gchar *currentDir = g_get_current_dir ();
    absoluteFileName = g_build_filename (currentDir, fileName, NULL);
    g_free (currentDir);
  }

  return absoluteFileName;
}

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

  const int w = options["width"];
  const int pageNumber = options["page"];
  std::string filename = options["file"];

  // GTK Stuff
  gtk_init (&argc, &argv);

  // PDF STUFF
  char *absoluteFileName = getAbsoluteFileName(filename.c_str());
  char *filename_uri = g_filename_to_uri(absoluteFileName, NULL, NULL);
  g_free(absoluteFileName);
  std::string file(filename_uri);
  g_free(filename_uri);

  PopplerDocument *pdfDocument = poppler_document_new_from_file(file.c_str(), NULL, NULL);
  PopplerPage* page = poppler_document_get_page(pdfDocument, pageNumber-1);

  double pageWidth, pageHeight;
  poppler_page_get_size(page, &pageWidth, &pageHeight);

  const double zoom = w/pageWidth;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(pageWidth*zoom, pageHeight*zoom, 32, SDL_SWSURFACE );

  unsigned char* data = (unsigned char*) screen->pixels;
  cairo_surface_t *surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_RGB24, pageWidth*zoom, pageHeight*zoom, screen->pitch);

  cairo_t* context = cairo_create(surface);
  cairo_save(context);
  cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
  cairo_rectangle(context, 0, 0, pageWidth*zoom, pageHeight*zoom);
  cairo_fill(context);
  cairo_restore(context);
  cairo_save(context);
  cairo_scale(context, zoom, zoom);
  poppler_page_render(page, context);
  cairo_destroy(context);
  cairo_surface_destroy(surface);

  SDL_Flip(screen);
  while ( poll() ) {
    SDL_Delay(10);
  }
}

