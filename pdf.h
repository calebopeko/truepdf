// -*- c++ -*-
#ifndef PDF_H
#define PDF_H

#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include <poppler.h>
#include <vector>
#include <string>
#include <assert.h>

class Page
{
public:

  Page()
    : width(1.0), height(1.0), targetWidth(1), rendered(false), page(NULL), surface(NULL) {}

  ~Page();

  void prepare(PopplerPage* page);

  void render(int targetWidth);

  SDL_Surface* getSurface() { return surface; }
  const SDL_Surface* getSurface() const { return surface; }

private:

  double width, height;

  int targetWidth;

  bool rendered;

  PopplerPage* page;

  SDL_Surface* surface;

};

class Document
{
public:

  Document()
    : document(NULL), pageCount(0), targetWidth(1), pages() {}

  Document(const std::string& filename);

  void open(const std::string& filename);

  void render(int targetWidth);

  Page& operator[](int i) { assert(0<=i && i<pageCount); return pages[i]; }
  const Page& operator[](int i) const { assert(0<=i && i<pageCount); return pages[i]; }
  
  ~Document();

private:

  PopplerDocument* document;

  int pageCount, targetWidth;

  std::vector<Page> pages;

};
#endif