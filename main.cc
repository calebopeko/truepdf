#include "options.h"
#include "presenter.h"

int main(int argc, char** argv)
{
  Options options(argc, argv);

  if ( (std::string) options["file"] == "" ) {
    options.printUsage();
    return 0;
  }

  gtk_init (&argc, &argv);

  Event& event = Event::instance();
  event.init(50);

  Presenter& presenter = Presenter::instance();
  presenter.init(options["width"], options["height"], options["file"]);
  presenter.run();

  return 0;
}

