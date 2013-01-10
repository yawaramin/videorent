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
    gtk_tree_view_column_new_with_attributes("ID", renderers[VIDEO_ID], "text", VIDEO_ID, NULL),
    gtk_tree_view_column_new_with_attributes("Title", renderers[VIDEO_TITLE], "text", VIDEO_TITLE, NULL),
    gtk_tree_view_column_new_with_attributes("URL", renderers[VIDEO_URL], "text", VIDEO_URL, NULL),
    gtk_tree_view_column_new_with_attributes("Rental Rate", renderers[VIDEO_RENTAL_AMT], "text", VIDEO_RENTAL_AMT, NULL)
  };

  int col;
  for (col = 0; col < N_COLUMNS; col++) {
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(columns[col]), TRUE);
    gtk_tree_view_column_set_clickable(GTK_TREE_VIEW_COLUMN(columns[col]), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), columns[col]);
  }
  gtk_tree_view_column_set_min_width(GTK_TREE_VIEW_COLUMN(columns[VIDEO_TITLE]), 400);
  gtk_tree_view_column_set_min_width(GTK_TREE_VIEW_COLUMN(columns[VIDEO_URL]), 300);

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
  GtkWidget* vbox;
  GtkWidget* menubar;
  GtkWidget* menudrop_video;
  GtkWidget* menu_video;
  GtkWidget* menu_video_add;
  GtkWidget* menu_video_sep;
  GtkWidget* menu_video_quit;
  // This is the list view of videos
  GtkWidget* video_list;
  GtkWidget* statusbar;

  gtk_init(p_argc, p_argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Videorent");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  menubar = gtk_menu_bar_new();

  menudrop_video = gtk_menu_new();
  menu_video = gtk_menu_item_new_with_mnemonic("_Video");
  menu_video_add = gtk_menu_item_new_with_mnemonic("_Add...");
  menu_video_sep = gtk_separator_menu_item_new();
  menu_video_quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_video), menudrop_video);
  gtk_menu_shell_append(GTK_MENU_SHELL(menudrop_video), menu_video_add);
  gtk_menu_shell_append(GTK_MENU_SHELL(menudrop_video), menu_video_sep);
  gtk_menu_shell_append(GTK_MENU_SHELL(menudrop_video), menu_video_quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_video);

  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 3);

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
  gtk_box_pack_start(GTK_BOX(vbox), video_list, FALSE, TRUE, 1);

  statusbar = gtk_statusbar_new();
  gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(menu_video_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();
}
