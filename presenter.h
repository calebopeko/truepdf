// -*- c++ -*-
#ifndef PRESENTER_H
#define PRESENTER_H

#include "pdf.h"
#include "event.h"

class Presenter
{
public:

  Presenter()
    : width(0), height(0), document(), screen(NULL), position(0), transitionSpace(2) {}

  Presenter(int width, int height, const std::string& filename);

  static Presenter& instance() { return instance_; }

  void init(int width, int height, const std::string& filename);

  void run();

private:

  static Presenter instance_;

  void render();
  
  int width, height;

  std::string filename;

  Document document;

  SDL_Surface* screen;

  double position;

  int transitionSpace;
};

#endif
