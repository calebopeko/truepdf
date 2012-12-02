#include "pdf.h"
#include "console.h"

static inline gchar* getAbsoluteFileName (const gchar *fileName)
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

void Page::prepare(PopplerPage* p)
{
  page = p;
  poppler_page_get_size(page, &width, &height);
}

void Page::setWidth(int tW)
{
  if ( tW != targetWidth ) {
    unrender();
  }
  targetWidth = tW;
}

bool Page::unrender()
{
  bool ret = false;
  if ( rendered && surface ) {
    SDL_FreeSurface(surface);
    ret = true;
  }
  rendered = false;

  return ret;
}

void Page::render()
{
  if ( rendered && surface ) {
    unrender();
  }

  Uint32 rmask, gmask, bmask, amask;

  // somehow derive these
  rmask = 0x00ff0000;
  gmask = 0x0000ff00;
  bmask = 0x000000ff;
  amask = 0xff000000;

  const double zoom = targetWidth/width;

  surface = SDL_CreateRGBSurface(SDL_SWSURFACE, targetWidth, zoom*height, 32, rmask, gmask, bmask, amask);
  if ( surface == NULL ) {
    console::err() << "Unable to create SDL surface" << std::endl;
    return;
  }

  unsigned char* data = (unsigned char*) surface->pixels;
  cairo_surface_t *cairo_surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_RGB24, targetWidth, zoom*height, surface->pitch);
  cairo_t* context = cairo_create(cairo_surface);
  cairo_save(context);
  cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
  cairo_rectangle(context, 0, 0, targetWidth, zoom*height);
  cairo_fill(context);
  cairo_restore(context);
  cairo_save(context);
  cairo_scale(context, zoom, zoom);
  poppler_page_render(page, context);
  cairo_destroy(context);
  cairo_surface_destroy(cairo_surface);

  if ( console::verbose() ) console::out() << "Rendered page " << surface->w << "x" << surface->h << std::endl;

  rendered = true;
}

Page::~Page()
{
  // if ( page ) {
  //   g_object_unref(page);
  // }
  unrender();
}

Document::Document(const std::string& filename)
  : pageCount(0), document(NULL), targetWidth(1), pages()
{
  open(filename);
}

void Document::open(const std::string& filename)
{
  gchar *absoluteFileName = getAbsoluteFileName(filename.c_str());
  gchar *filename_uri = g_filename_to_uri(absoluteFileName, NULL, NULL);
  g_free(absoluteFileName);
  std::string file(filename_uri);
  g_free(filename_uri);

  document = poppler_document_new_from_file(file.c_str(), NULL, NULL);
  if ( !document ) {
    console::err() << "Unable to open document: " << file << std::endl;
    return;
  }
  pageCount = poppler_document_get_n_pages(document);
  if ( pageCount == 0 ) {
    console::err() << "Document has 0 pages!" << std::endl;
    return;
  }

  pages.clear();
  pages.resize(pageCount);
  for ( int i=0; i<pageCount; ++i ) {
    PopplerPage* page = poppler_document_get_page(document, i);
    if ( page ) {
      pages[i].prepare(page);
    } else {
      console::err() << "Unable to get page " << i << std::endl;
    }
  }
}

void Document::setWidth(int tW)
{
  targetWidth = tW;
  for ( int i=0; i<pageCount; ++i ) {
    pages[i].setWidth(targetWidth);
  }
}

void Document::render()
{
  if ( console::verbose() ) console::out() << "Pre-rendering all pages..." << std::endl;
  for ( int i=0; i<pageCount; ++i ) {
    pages[i].setWidth(targetWidth);
    pages[i].render();
    if ( console::verbose() ) console::out() << i+1 << "/" << pageCount << std::endl;
  }
  if ( console::verbose() ) console::out() << "Done." << std::endl;
}

int Document::pageHeight()
{
  return targetWidth/pages[0].width*pages[0].height;
}

Document::~Document()
{
  // if ( document ) {
  //   g_object_unref(document);
  // }

  SDL_Quit();
}
