// -*- c++ -*-
#ifndef PRESENTER_H
#define PRESENTER_H

#include "pdf.h"

class Presenter
{
public:

  Presenter()
    : width(0), height(0), document(), screen(NULL) {}

  Presenter(int width, int height, const std::string& filename);

  void init(int width, int height, const std::string& filename);

  void run();

private:

  bool poll();
  
  int width, height;

  std::string filename;

  Document document;

  SDL_Surface* screen;
};

#endif
