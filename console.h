// -*- C++ -*-
#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <fstream>

namespace console
{
  void verbose(bool v);

  bool verbose();

  void out(const char* c);
  void err(const char* c);

  void in(bool& b);
  void in(int& i);
  void in(char& c);

  std::ostream& out();
  std::ostream& err();
}

#endif
