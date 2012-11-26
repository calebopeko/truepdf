// -*- c++ -*-
#ifndef PDF_H
#define PDF_H

#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include <poppler.h>
#include <vector>
#include <string>

class Page
{
public:

  Page()
    : width(1.0), height(1.0), rendered(false), page(NULL), surface(NULL) {}

  ~Page();

  void prepare(PopplerPage* page, bool render=false);

  void render();

private:

  double width, height;

  bool rendered;

  PopplerPage* page;

  SDL_Surface* surface;

};

class Document
{
public:

  Document(const std::string& filename);
  
  ~Document();

private:

  PopplerDocument* document;

  int pageCount;

  std::vector<Page> pages;

};
#endif
