#include "options.h"
#include "presenter.h"
#include "console.h"

int main(int argc, char** argv)
{
  Options options(argc, argv);

  if ( (std::string) options["file"] == "" ) {
    options.printUsage();
    return -1;
  }

  console::verbose(options["verbose"]);

  gtk_init (&argc, &argv);

  Event& event = Event::instance();
  event.init(30);

  Presenter& presenter = Presenter::instance();
  presenter.init(options["width"], options["height"], options["file"]);
  presenter.run();

  return 0;
}

