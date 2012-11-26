#include "options.h"
#include "presenter.h"

int main(int argc, char** argv)
{
  Options options(argc, argv);

  const int width = options["width"];
  const int pageNumber = options["page"];
  std::string filename = options["file"];

  if ( filename == "" ) {
    options.printUsage();
    return 0;
  }

  // GTK Stuff
  gtk_init (&argc, &argv);

  Presenter presenter(width, 4*width, filename);
  presenter.run();

  return 0;
}

