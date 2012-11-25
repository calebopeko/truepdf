#include "options.h"
#include "console.h"

#include <gtk/gtk.h>

void callback_main_window_quit(GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}

int main(int argc, char** argv)
{
  Options options(argc, argv);

    gtk_init (&argc, &argv);
    g_set_application_name("truepdf");

    GtkWidget* mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(callback_main_window_quit), NULL);

    GtkWidget* mainBox = gtk_vbox_new (FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow), mainBox);

    GtkWidget* mainLabel = gtk_label_new("Test-Label");
    gtk_box_pack_start(GTK_BOX(mainBox), mainLabel, FALSE, FALSE, 0);

    GtkWidget* mainImage = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(mainBox), mainImage, TRUE, FALSE, 0);

    gtk_widget_show_all(mainBox);

    gtk_widget_show(mainWindow);

    gtk_main();    
}

