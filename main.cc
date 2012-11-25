#include "options.h"
#include "console.h"

#include <gtk/gtk.h>

int main(int argc, char** argv)
{
  Options options(argc, argv);

    gtk_init (&argc, &argv);
    g_set_application_name("truepdf");
    gtk_main();    
}
