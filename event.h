#ifndef EVENT_H
#define EVENT_H

#include "SDL/SDL.h"
#include <map>

class Event
{
 private:

  enum KeyIndex {
    Key_None=0,
    Key_ScrollUp,
    Key_ScrollDown,
    Key_PageUp,
    Key_PageDown,
    Key_Home,
    Key_End,
    Key_Reopen,
    
    Key_Quit,
    Key_Total // <- last
  };

  struct KeyDef {

    KeyDef() : index(Key_None), continuous(false) {}
    
    KeyDef(KeyIndex idx, bool c) : index(idx), continuous(c) {}

    KeyIndex index;
    
    bool continuous;
  };

 public:

  Event() : fpsCap(25), frameAim(1000./fpsCap), fps(1), running(true) {}

  static Event& instance() { return instance_; }

  void init(int fps);
  
  bool poll();

  void fillFrame();
  
  bool diag(Uint32 ms);

  float getFps() { return fps; }

  float invFps() { return ifps; }

 private:

  void keyPressed();

  void keyUp(SDL_Event& ev);
  
  void keyDown(SDL_Event& ev);

  void keySingle(KeyIndex key);

  void mouseMove(int x, int y);

 private:

  static Event instance_;

  std::map<SDLKey, KeyDef> keyMap;

  int fpsCap;

  float frameAim;

  float fps;

  float ifps;

  bool running;

  bool keys[Key_Total];
};

#endif
