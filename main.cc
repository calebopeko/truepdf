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

  // GTK Stuff
  gtk_init (&argc, &argv);

  // PDF STUFF
  GError *error = NULL;

  const char* filename = "test.pdf";
  gchar *absoluteFileName = getAbsoluteFileName(filename);
  gchar *filename_uri = g_filename_to_uri(absoluteFileName, NULL, &error);

  PopplerDocument *pdfDocument = poppler_document_new_from_file(filename_uri, NULL, &error);

  g_free(absoluteFileName);
  g_free(filename_uri);

  if ( NULL == pdfDocument ) {
    console::out() << "Error loading pdf document!" << std::endl;
  } else {
    console::out() << "Pdf document successfully loaded!" << std::endl;
  }

  PopplerPage* page = poppler_document_get_page(pdfDocument, 0);
  if ( page == NULL ) {
    console::out() << "Error loading page!" << std::endl;
  } else {
    console::out() << "Page successfully loaded!" << std::endl;
  }

  gdouble pageWidth, pageHeight;
  poppler_page_get_size(page, &pageWidth, &pageHeight);
  console::out() << "Page is " << pageWidth << "x" << pageHeight << std::endl;

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, pageWidth);
  unsigned char* data = new unsigned char[stride*((int)pageHeight)*4];
  cairo_surface_t *surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_RGB24, pageWidth, pageHeight, stride);
  cairo_t* context = cairo_create(surface);
  cairo_save(context);
  cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
  cairo_rectangle(context, 0, 0, pageWidth, pageHeight);
  cairo_fill(context);
  cairo_restore(context);
  cairo_save(context);

  // cairo_scale(context, 10.0, 10.0);
  poppler_page_render(page, context);

  cairo_destroy(context);
  cairo_surface_destroy(surface);


  // SDL stuff
  SDL_Surface* screen = SDL_SetVideoMode(pageWidth, pageHeight, 32, SDL_SWSURFACE | SDL_DOUBLEBUF );
  int bpp = screen->format->BytesPerPixel;

  for ( int iy=0; iy<pageWidth; iy++ ) {
    for ( int ix=0; ix<pageWidth; ix++ ) {
      Uint8 *p = (Uint8 *)screen->pixels + iy * screen->pitch + ix * bpp;
      for ( int b = 0; b<3; b++ ) {
	p[b] = data[(ix+iy*stride)*4+b];
      }
    }
  }

  while ( poll() ) {
    SDL_Delay(10);
    SDL_Flip(screen);
  }
  
  delete[] data;
}

