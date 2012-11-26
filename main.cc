#include "options.h"
#include "console.h"

#include <gtk/gtk.h>
#include <poppler.h>

void callback_main_window_quit(GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}

gchar* getAbsoluteFileName (const gchar *fileName)
{
  gchar *absoluteFileName = NULL;
  if ( g_path_is_absolute(fileName) ) {
      absoluteFileName = g_strdup (fileName);
  } else {
    gchar *currentDir = g_get_current_dir ();
    absoluteFileName = g_build_filename (currentDir, fileName, NULL);
    g_free (currentDir);
  }

  return absoluteFileName;
}

int main(int argc, char** argv)
{
  Options options(argc, argv);



  // GTK Stuff

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

  // gtk_image_set_from_pixbuf (GTK_IMAGE (mainImage), NULL);
  // GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data (page->getData (),
  // 						  GDK_COLORSPACE_RGB, page->hasAlpha(), 8,
  // 						  page->getWidth(), page->getHeight(),
  // 						  page->getRowStride(), NULL, NULL);
  // gtk_image_set_from_pixbuf (GTK_IMAGE (mainImage), pixbuf);
  // g_object_unref (pixbuf);

  // gtk_main();    


  // PDF STUFF

  GError *error = NULL;

  const char* filename = "test.pdf";
  gchar *absoluteFileName = getAbsoluteFileName(filename);
  gchar *filename_uri = g_filename_to_uri(absoluteFileName, NULL, &error);

  PopplerDocument *pdfDocument = poppler_document_new_from_file(filename_uri, NULL, &error);

  g_free(absoluteFileName);
  g_free(filename_uri);

  if ( NULL == pdfDocument ) {
    console::out() << "Error loading pdf document!" << std::endl;
  } else {
    console::out() << "Pdf document successfully loaded!" << std::endl;
  }

  PopplerPage* page = poppler_document_get_page(pdfDocument, 0);
  if ( page == NULL ) {
    console::out() << "Error loading page!" << std::endl;
  } else {
    console::out() << "Page successfully loaded!" << std::endl;
  }

  gdouble pageWidth, pageHeight;
  poppler_page_get_size(page, &pageWidth, &pageHeight);
  console::out() << "Page is " << pageWidth << "x" << pageHeight << std::endl;

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, pageWidth);
  unsigned char* data = new unsigned char[stride*((int)pageHeight)];
  cairo_surface_t *surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_RGB24, pageWidth, pageHeight, stride);
  cairo_t* context = cairo_create(surface);
  cairo_save(context);
  cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
  cairo_rectangle(context, 0, 0, pageWidth, pageHeight);
  cairo_fill(context);
  cairo_restore(context);
  cairo_save(context);

  poppler_page_render(page, context);

  cairo_destroy(context);
  cairo_surface_destroy(surface);
}

