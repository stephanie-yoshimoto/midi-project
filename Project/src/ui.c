/* Stephanie Yoshimoto, ui.c, CS 24000, Spring 2020
 * Last updated April 9, 2020
 */

#include "ui.h"

#include <gtk/gtk.h>
#include <ftw.h>

tree_node_t *g_current_node = NULL;
song_data_t *g_current_song = NULL;
song_data_t *g_modified_song = NULL;

void update_song_list() {
  /* post order traversal of printing to text label? */
} /* update_song_list() */

void update_drawing_area() {

} /* update_drawing_area() */

void update_info() {

} /* update_info() */

void update_song() {

} /* update_song() */

void range_of_song(song_data_t *song, int *lowest_pitch, int *highest_pitch,
                   int *song_length) {

} /* range_of_song() */

void activate(GtkApplication *app, gpointer ptr) {
  /* set window */

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "MIDI Library");
  gtk_container_set_border_width(GTK_CONTAINER (window), 10);
  gtk_widget_set_size_request(window, 900, 800);
  gtk_window_set_resizable(GTK_WINDOW (window), false);
  g_signal_connect(G_OBJECT (window), "window",
    G_CALLBACK (load_songs_cb), NULL);

  /* grid */

  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER (window), grid);

  /* add song button */

  GtkWidget *add_song_btn = gtk_button_new_with_label("Add Song from File");
  gtk_widget_set_size_request(add_song_btn, 100, 30);
  g_signal_connect(G_OBJECT (add_song_btn), "clicked",
    G_CALLBACK (add_song_cb), NULL);
  gtk_container_add(GTK_CONTAINER (window), add_song_btn);
  gtk_grid_attach(GTK_GRID (grid), add_song_btn, 0, 0, 1, 1);

  /* load from directory button */

  GtkWidget *load_dir_btn =
    gtk_button_new_with_label("Load from Directory");
  gtk_widget_set_size_request(load_dir_btn, 100, 30);
  g_signal_connect(G_OBJECT (load_dir_btn), "clicked",
    G_CALLBACK (load_songs_cb), NULL);
  gtk_container_add(GTK_CONTAINER (window), load_dir_btn);
  gtk_grid_attach(GTK_GRID (grid), load_dir_btn, 1, 0, 1, 1);

  /* song list */

  GtkWidget *song_list = gtk_list_box_new();
  gtk_widget_set_size_request(song_list, 200, 700);
  gtk_list_box_set_selection_mode(GTK_LIST_BOX (song_list),
    GTK_SELECTION_SINGLE);
  gtk_container_add(GTK_CONTAINER (window), song_list);
  gtk_grid_attach(GTK_GRID (grid), song_list, 0, 1, 2, 1);
  /*for (int i = 0; i < 20; i++) {
    GtkWidget *row = gtk_list_box_row_new();
    gtk_widget_set_size_request(row, 200, 20);
    GtkWidget *label = gtk_label_new("song name");
    gtk_widget_set_size_request(label, 200, 15);
    gtk_label_set_xalign(GTK_LABEL (label), 0.0);
    gtk_container_add(GTK_CONTAINER (row), label);
    gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW (row), true);
    gtk_list_box_prepend(GTK_LIST_BOX (song_list), row);
    g_signal_emit_by_name(G_OBJECT (row), "selection",
      G_CALLBACK (song_selected_cb), NULL);
  }*/

  GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scroll_window),
    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER (song_list), scroll_window);

  /* search bar */

  GtkWidget *search_bar = gtk_search_bar_new();
  gtk_widget_set_size_request(search_bar, 200, 30);
  gtk_search_bar_set_search_mode(GTK_SEARCH_BAR (search_bar), true);
  g_signal_connect(search_bar, "highlighted",
    G_CALLBACK (search_bar_cb), NULL);
  gtk_container_add(GTK_CONTAINER (window), search_bar);
  gtk_grid_attach(GTK_GRID (grid), search_bar, 0, 2, 3, 1);

  /* label */

  GtkWidget *song_info = gtk_label_new("Select a file from list to start...");
  g_signal_connect(song_list, "highlighted",
    G_CALLBACK (song_selected_cb), song_info);
  gtk_container_add(GTK_CONTAINER (window), song_info);
  gtk_grid_attach(GTK_GRID (grid), song_info, 2, 0, 1, 1);

  /* add widgets to window */

  gtk_widget_show_all(window);
  gtk_main();
} /* activate() */

void add_song_cb(GtkButton *button, gpointer ptr) {
  /*GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
    GTK_WINDOW (window), GTK_RESPONSE_CANCEL, "Cancel",
    GTK_FILE_CHOOSER_ACTION_OPEN, "Open", NULL);
  gtk_container_add(GTK_CONTAINER (window), dialog);
  gtk_widget_show_all(window);*/
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_transient_for(GTK_WINDOW (window), NULL);
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
    GTK_WINDOW (window), GTK_RESPONSE_CANCEL, "Cancel",
    GTK_FILE_CHOOSER_ACTION_OPEN, "Open", NULL);

  gint status = gtk_dialog_run(GTK_DIALOG (dialog));
  if (status == GTK_FILE_CHOOSER_ACTION_OPEN) {
    gchar *file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
    g_print("%s\n", file_name);
    /*make_library(file_name);
    update_song_list();*/
    gtk_widget_destroy(dialog);
  }
  else if (status == GTK_RESPONSE_CANCEL) {
    gtk_widget_destroy(dialog);
  }
} /* add_song_cb() */

void load_songs_cb(GtkButton *button, gpointer ptr) {
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  /*GtkWidget *dialog = gtk_file_chooser_dialog_new("Choose Folder",
    GTK_WINDOW (window), GTK_RESPONSE_CANCEL, "Cancel",
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Open", NULL);
  gtk_container_add(GTK_CONTAINER (window), dialog);
  gtk_widget_show_all(window);*/
  gtk_window_set_transient_for(GTK_WINDOW (window), NULL);
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Choose Folder",
    GTK_WINDOW (window),
    GTK_RESPONSE_CANCEL, "Cancel",
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Open", NULL);
  gtk_widget_show(dialog);

  gint status = gtk_dialog_run(GTK_DIALOG (dialog));
  if (status == GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) {
    gchar *dir_name =
      gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER (dialog));
    g_print("%s\n", dir_name);
    /*make_library(dir_name);
    update_song_list();*/
    gtk_widget_destroy(dialog);
  }
  else if (status == GTK_RESPONSE_CANCEL) {
    gtk_widget_destroy(dialog);
  }
} /* load_songs_cb() */

void song_selected_cb(GtkListBox *list_box, GtkListBoxRow *row) {
  g_print("hellooo\n");
} /* song_selected_cb() */

void search_bar_cb(GtkSearchBar *search_bar, gpointer ptr) {
  g_print("hellooo\n");
} /* search_bar_cb() */

void time_scale_cb(GtkSpinButton *button, gpointer ptr) {

} /* time_scale_cb() */

gboolean draw_cb(GtkDrawingArea *drawing_area, cairo_t *cairo, gpointer ptr) {
  return false;
} /* draw_cb() */

void warp_time_cb(GtkSpinButton *button, gpointer ptr) {

} /* warp_time_cb() */

void song_octave_cb(GtkSpinButton *button, gpointer ptr) {

} /* song_octave_cb() */

void instrument_map_cb(GtkComboBoxText *text, gpointer ptr) {

} /* instrument_map_cb() */

void note_map_cb(GtkComboBoxText *text, gpointer ptr) {

} /* note_map_cb() */

void save_song_cb(GtkButton *button, gpointer ptr) {

} /* save_song_cb() */

void remove_song_cb(GtkButton *button, gpointer ptr) {

} /* remove_song_cb() */

/*
 * Function called prior to main that sets up the instrument to color mapping
 */

void build_color_palette()
{
  static GdkRGBA palette[16];	

  memset(COLOR_PALETTE, 0, sizeof(COLOR_PALETTE));
  char* color_specs[] = {
    // Piano, red
    "#ff0000",
    // Chromatic percussion, brown
    "#8b4513",
    // Organ, purple
    "#800080",
    // Guitar, green
    "#00ff00",
    // Bass, blue
    "#0000ff",
    // Strings, cyan
    "#00ffff",
    // Ensemble, teal
    "#008080",
    // Brass, orange
    "#ffa500",
    // Reed, magenta
    "#ff00ff",
    // Pipe, yellow
    "ffff00",
    // Synth lead, indigo
    "#4b0082",
    // Synth pad, dark slate grar
    "#2f4f4f",
    // Synth effects, silver
    "#c0c0c0",
    // Ehtnic, olive
    "#808000",
    // Percussive, silver
    "#c0c0c0",
    // Sound effects, gray
    "#808080",
  };

  for (int i = 0; i < 16; ++i) {
    gdk_rgba_parse(&palette[i], color_specs[i]);
    for (int j = 0; j < 8; ++j) {
      COLOR_PALETTE[i * 8 + j] = &palette[i];
    }
  }
} /* build_color_palette() */
