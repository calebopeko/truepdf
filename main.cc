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

  const double zoom = options["zoom"];
  const int pageNumber = options["page"];
  std::string filename = options["file"];

  // GTK Stuff
  gtk_init (&argc, &argv);

  // PDF STUFF
  GError *error = NULL;

  gchar *absoluteFileName = getAbsoluteFileName(filename.c_str());
  gchar *filename_uri = g_filename_to_uri(absoluteFileName, NULL, &error);

  PopplerDocument *pdfDocument = poppler_document_new_from_file(filename_uri, NULL, &error);

  g_free(absoluteFileName);
  g_free(filename_uri);

  if ( NULL == pdfDocument ) {
    console::out() << "Error loading pdf document!" << std::endl;
  } else {
    console::out() << "Pdf document successfully loaded!" << std::endl;
  }

  PopplerPage* page = poppler_document_get_page(pdfDocument, pageNumber-1);
  if ( page == NULL ) {
    console::out() << "Error loading page!" << std::endl;
  } else {
    console::out() << "Page successfully loaded!" << std::endl;
  }

  gdouble pageWidth, pageHeight;
  poppler_page_get_size(page, &pageWidth, &pageHeight);
  console::out() << "Page is " << pageWidth << "x" << pageHeight << std::endl;


  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen = SDL_SetVideoMode(pageWidth*zoom, pageHeight*zoom, 32, SDL_SWSURFACE );

  int stride = screen->pitch;
  console::out() << "Stride set to " << stride << std::endl;
  unsigned char* data = (unsigned char*) screen->pixels;
  console::out() << "Buffer for page allocated to " << stride*((int)(pageHeight*zoom)) << " bytes." << std::endl;
  cairo_surface_t *surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_RGB24, pageWidth*zoom, pageHeight*zoom, stride);
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

