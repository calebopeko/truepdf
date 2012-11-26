// -*- c++ -*-
#ifndef PRESENTER_H
#define PRESENTER_H

#include "pdf.h"

class Presenter
{
public:

  Presenter()
    : width(0), height(0), document(), screen(NULL), position(0) {}

  Presenter(int width, int height, const std::string& filename);

  void init(int width, int height, const std::string& filename);

  void run();

private:

  void render();

  bool poll();
  
  int width, height;

  std::string filename;

  Document document;

  SDL_Surface* screen;

  double position;
};

#endif
