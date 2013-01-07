#include <gtk/gtk.h>
#include "ui.h"
#include "videorent.h"

enum {
  VIDEO_ID = 0,
  VIDEO_TITLE,
  VIDEO_URL,
  VIDEO_RENTAL_AMT,
  N_COLUMNS
};

static void
init_video_list(GtkWidget* list) {
  GtkCellRenderer* renderers[N_COLUMNS] = {
    gtk_cell_renderer_text_new(),
    gtk_cell_renderer_text_new(),
    gtk_cell_renderer_text_new(),
    gtk_cell_renderer_text_new()
  };

  GtkTreeViewColumn* columns[N_COLUMNS] = {
    gtk_tree_view_column_new_with_attributes("ID", renderers[0], "text", VIDEO_ID, NULL),
    gtk_tree_view_column_new_with_attributes("Title", renderers[1], "text", VIDEO_TITLE, NULL),
    gtk_tree_view_column_new_with_attributes("URL", renderers[2], "text", VIDEO_URL, NULL),
    gtk_tree_view_column_new_with_attributes("Rental Rate", renderers[3], "text", VIDEO_RENTAL_AMT, NULL)
  };

  int col;
  for (col = 0; col < N_COLUMNS; col++) {
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(columns[col]), TRUE);
    gtk_tree_view_column_set_clickable(GTK_TREE_VIEW_COLUMN(columns[col]), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), columns[col]);
  }

  GtkListStore* list_videos = gtk_list_store_new(
    N_COLUMNS,
    G_TYPE_UINT,
    G_TYPE_STRING,
    G_TYPE_STRING,
    G_TYPE_FLOAT
  );

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(list_videos));
  g_object_unref(list_videos);
}

static void
add_to_video_list(
  GtkWidget* list,
  guint v_id,
  const gchar* const v_title,
  const gchar* const v_url,
  gfloat v_amt
) {
  GtkListStore* store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
    VIDEO_ID, v_id,
    VIDEO_TITLE, v_title,
    VIDEO_URL, v_url,
    VIDEO_RENTAL_AMT, v_amt,
    -1
  );
}

void
init_ui_gtk(int* p_argc, char*** p_argv) {
  GtkWidget* window;
  // This is the list view of videos
  GtkWidget* video_list;

  gtk_init(p_argc, p_argv);

  video_list = gtk_tree_view_new();
  init_video_list(video_list);

  // Example video:
  add_to_video_list(
    video_list,
    0,
    "Once Upon a Time in the West",
    "http://www.imdb.com/title/tt0064116/",
    3.0
  );

  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(video_list));

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Videorent");
  gtk_container_add(GTK_CONTAINER(window), video_list);

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);

  gtk_main();
}
