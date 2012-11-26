// -*- c++ -*-
#ifndef PRESENTER_H
#define PRESENTER_H

#include "pdf.h"
#include "event.h"

class Presenter
{
public:

  Presenter()
    : position(0), width(0), height(0), document(), screen(NULL), transitionSpace(2) {}

  Presenter(int width, int height, const std::string& filename);

  static Presenter& instance() { return instance_; }

  void init(int width, int height, const std::string& filename);

  void run();

  void render();
  
  double position;

  int width, height;

private:

  static Presenter instance_;

  std::string filename;

  Document document;

  SDL_Surface* screen;

  int transitionSpace;
};

#endif
