#include "options.h"
#include "console.h"

int main(int argc, char** argv)
{
  Options options(argc, argv);
  console::out() << "Hello, world!" << std::endl;
}
