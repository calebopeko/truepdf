// -*- c++ -*-
#ifndef PRESENTER_H
#define PRESENTER_H

#include "pdf.h"
#include "event.h"

#include <list>

class Presenter
{
public:

  Presenter()
    : width(0), height(0), position(0), document(), screen(NULL), transitionSpace(2), preRender(false) {}

  Presenter(int width, int height, const std::string& filename);

  static Presenter& instance() { return instance_; }

  void init(int width, int height, const std::string& filename, bool pre = false);

  void run();

  void render();

  void reopen();

  void resize(int w, int h);
  
  double getPosition() { return position; }
  void setPosition(double p) { position = p; clamp(); }

  Document& getDocument() { return document; }

  void clamp();

  int width, height;

private:

  int renderPage(int src, int dest, int page);

  void clearCache();

  static Presenter instance_;

  double position;

  std::string filename;

  Document document;

  SDL_Surface* screen;

  int transitionSpace;

  bool preRender;

  std::list<int> usedPages;
};

#endif
