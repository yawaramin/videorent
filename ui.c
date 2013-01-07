#include <gtk/gtk.h>
#include "ui.h"
#include "videorent.h"

void
init_ui_gtk(int* p_argc, char*** p_argv) {
  GtkWidget* window;
  
  gtk_init(p_argc, p_argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Videorent");

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);

  gtk_main();
}
