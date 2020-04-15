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

void quit(GtkWidget *window, GdkEvent *event, gpointer gptr) {
  gtk_main_quit();
} /* quit() */

void activate(GtkApplication *app, gpointer ptr) {
  /* set window */

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "MIDI Library");
  gtk_container_set_border_width(GTK_CONTAINER (window), 10);
  gtk_widget_set_size_request(window, 900, 800);
  gtk_window_set_resizable(GTK_WINDOW (window), false);
  g_signal_connect(G_OBJECT (window), "destroy",
    G_CALLBACK (quit), NULL);

  /* boxes */

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (window), box);

  GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (box), left_box);
  GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (box), right_box);

  GtkWidget *left_upper_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (left_box), left_upper_box);
  GtkWidget *left_lower_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (left_box), left_lower_box);

  /* add song button */

  GtkWidget *add_song_btn = gtk_button_new_with_label("Add Song from File");
  gtk_widget_set_size_request(add_song_btn, 100, 30);
  g_signal_connect(G_OBJECT (add_song_btn), "clicked",
    G_CALLBACK (add_song_cb), NULL);
  gtk_box_pack_start(GTK_BOX (left_upper_box), add_song_btn, false, false, 0);

  /* load from directory button */

  GtkWidget *load_dir_btn = gtk_button_new_with_label("Load from Directory");
  g_signal_connect(G_OBJECT (load_dir_btn), "clicked",
    G_CALLBACK (load_songs_cb), NULL);
  gtk_box_pack_end(GTK_BOX (left_upper_box), load_dir_btn, false, false, 0);

  /* song list */

  GtkWidget *song_list = gtk_list_box_new();
  gtk_widget_set_size_request(song_list, 200, 700);
  /*gtk_list_box_set_selection_mode(GTK_LIST_BOX (song_list),
    GTK_SELECTION_SINGLE);*/
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
  gtk_box_pack_start(GTK_BOX (left_lower_box), song_list, false, false, 0);

  GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scroll_window),
    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER (song_list), scroll_window);

  /* search bar */

  GtkWidget *search_bar = gtk_search_bar_new();
  gtk_widget_set_size_request(search_bar, 200, 30);
  gtk_search_bar_set_search_mode(GTK_SEARCH_BAR (search_bar), true);
  GtkWidget *search_entry = gtk_search_entry_new();
  gtk_widget_set_size_request(search_entry, 200, 30);
  g_signal_connect(search_bar, "key-press-event", G_CALLBACK (search_bar_cb),
    search_entry);
  gtk_box_pack_end(GTK_BOX (left_lower_box), search_entry, false, false, 0);

  /* label */

  GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_widget_set_size_request(info_box, 600, 80);
  gtk_container_add(GTK_CONTAINER (right_box), info_box);
  GtkWidget *song_info = gtk_label_new("Select a file from list to start...");
  gtk_label_set_xalign(GTK_LABEL (song_info), 0);
  gtk_label_set_yalign(GTK_LABEL (song_info), 0);
  g_signal_connect(song_list, "changed", G_CALLBACK (song_selected_cb),
    song_info);
  gtk_label_set_justify(GTK_LABEL (song_info), GTK_JUSTIFY_LEFT);
  gtk_box_pack_start(GTK_BOX (right_box), song_info, false, false, 0);

  /* T-scale */

  GtkWidget *t_scale_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (right_box), t_scale_box);
  gtk_box_pack_start(GTK_BOX (right_box), t_scale_box, false, false, 0);
  GtkWidget *t_scale_label = gtk_label_new("T-scale:");
  gtk_label_set_justify(GTK_LABEL (t_scale_label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (t_scale_box), t_scale_label, false, false, 0);
  GtkAdjustment *t_scale_adjustment = gtk_adjustment_new(1, 1, 10000, 1, 0, 0);
  GtkWidget *t_scale_spin = gtk_spin_button_new(t_scale_adjustment, 0.0, 1);
  gtk_widget_set_size_request(t_scale_spin, 200, 25);
  g_signal_connect(G_OBJECT (t_scale_spin), "changed",
    G_CALLBACK (time_scale_cb), NULL);
  gtk_box_pack_end(GTK_BOX (t_scale_box), t_scale_spin, false, false, 0);

  /* drawing areas */

  GtkWidget *drawing_1 = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_1, 600, 200);
  g_signal_connect(G_OBJECT (drawing_1), "draw", G_CALLBACK (draw_cb), NULL);
  gtk_box_pack_start(GTK_BOX (right_box), drawing_1, false, false, 0);
  GtkWidget *drawing_2 = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_2, 600, 200);
  /*g_signal_connect(G_OBJECT (drawing_2), "draw", G_CALLBACK (draw_cb), NULL);*/
  gtk_box_pack_start(GTK_BOX (right_box), drawing_2, false, false, 0);

  /* warp time */

  GtkWidget *warp_time_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (right_box), warp_time_box);
  gtk_box_pack_start(GTK_BOX (right_box), warp_time_box, false, false, 0);
  GtkWidget *warp_time_label = gtk_label_new("Warp time:");
  /*gtk_label_set_justify(GTK_LABEL (warp_time_label), GTK_JUSTIFY_CENTER);*/
  /*gtk_widget_set_halign(warp_time_label, GTK_ALIGN_CENTER);*/
  gtk_box_pack_start(GTK_BOX (warp_time_box), warp_time_label, false, false, 0);
  GtkAdjustment *warp_time_adjustment =
    gtk_adjustment_new(0.1, 0.1, 10.0, 0.1, 0, 0);
  GtkWidget *warp_time_spin = gtk_spin_button_new(warp_time_adjustment, 0.0, 1);
  gtk_widget_set_size_request(warp_time_spin, 200, 25);
  g_signal_connect(G_OBJECT (warp_time_spin), "changed",
    G_CALLBACK (warp_time_cb), NULL);
  gtk_box_pack_end(GTK_BOX (warp_time_box), warp_time_spin, false, false, 0);

  /* song octave */

  GtkWidget *octave_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (right_box), octave_box);
  gtk_box_pack_start(GTK_BOX (right_box), octave_box, false, false, 0);
  GtkWidget *octave_label = gtk_label_new("Change octave:");
  gtk_label_set_justify(GTK_LABEL (octave_label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (octave_box), octave_label, false, false, 0);
  GtkAdjustment *octave_adjustment = gtk_adjustment_new(-5, -5, 5, 1, 0, 0);
  GtkWidget *octave_spin = gtk_spin_button_new(octave_adjustment, 0.0, 1);
  gtk_widget_set_size_request(octave_spin, 200, 25);
  g_signal_connect(G_OBJECT (octave_spin), "changed",
    G_CALLBACK (song_octave_cb), NULL);
  gtk_box_pack_end(GTK_BOX (octave_box), octave_spin, false, false, 0);

  /* instruments */

  GtkWidget *instrument_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (right_box), instrument_box);
  gtk_box_pack_start(GTK_BOX (right_box), instrument_box, false, false, 0);
  GtkWidget *instrument_label = gtk_label_new("Remap instruments:");
  gtk_label_set_justify(GTK_LABEL (instrument_label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (instrument_box), instrument_label, false, false, 0);
  GtkWidget *combo_box_text = gtk_combo_box_text_new();
  gtk_widget_set_size_request(combo_box_text, 200, 25);
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (combo_box_text), NULL, NULL);
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (combo_box_text), NULL,
    "Brass band");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (combo_box_text), NULL,
    "Helicopter");
  g_signal_connect(G_OBJECT (combo_box_text), "changed",
    G_CALLBACK (instrument_map_cb), NULL);
  gtk_box_pack_end(GTK_BOX (instrument_box), combo_box_text, false, false, 0);

  /* notes */

  GtkWidget *notes_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (right_box), notes_box);
  gtk_box_pack_start(GTK_BOX (right_box), notes_box, false, false, 0);
  GtkWidget *notes_label = gtk_label_new("Remap notes:");
  gtk_label_set_justify(GTK_LABEL (notes_label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (notes_box), notes_label, false, false, 0);
  GtkWidget *combo_box_2_text = gtk_combo_box_text_new();
  gtk_widget_set_size_request(combo_box_2_text, 200, 25);
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (combo_box_2_text), NULL, NULL);
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (combo_box_2_text), NULL,
    "Lower");
  g_signal_connect(G_OBJECT (combo_box_2_text), "changed",
    G_CALLBACK (note_map_cb), NULL);
  gtk_box_pack_end(GTK_BOX (notes_box), combo_box_2_text, false, false, 0);

  /* save/remove buttons */

  GtkWidget *right_lower_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (right_box), right_lower_box);
  gtk_box_pack_end(GTK_BOX (right_box), right_lower_box, false, false, 0);
  GtkWidget *remove_song_btn = gtk_button_new_with_label("Remove Song");
  gtk_widget_set_size_request(remove_song_btn, 200, 30);
  g_signal_connect(G_OBJECT (remove_song_btn), "clicked",
    G_CALLBACK (remove_song_cb), NULL);
  gtk_box_pack_end(GTK_BOX (right_lower_box), remove_song_btn, false, false, 0);
  GtkWidget *save_song_btn = gtk_button_new_with_label("Save Song");
  gtk_widget_set_size_request(save_song_btn, 200, 30);
  g_signal_connect(G_OBJECT (save_song_btn), "clicked",
    G_CALLBACK (save_song_cb), NULL);
  gtk_box_pack_end(GTK_BOX (right_lower_box), save_song_btn, false, false, 0);

  /* add widgets to window */

  gtk_widget_show_all(window);
  gtk_main();
} /* activate() */

void add_song_cb(GtkButton *button, gpointer ptr) {
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
    g_free(file_name);
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

  gint status = gtk_dialog_run(GTK_DIALOG (dialog));
  if (status == GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) {
    g_print("wrong function\n");
    gchar *dir_name =
      gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER (dialog));
    g_print("%s\n", dir_name);
    /*make_library(dir_name);
    update_song_list();*/
    gtk_widget_destroy(dialog);
    g_free(dir_name);
  }
  else if (status == GTK_RESPONSE_CANCEL) {
    gtk_widget_destroy(dialog);
  }
} /* load_songs_cb() */

void song_selected_cb(GtkListBox *list_box, GtkListBoxRow *row) {
  g_print("hellooo\n");
} /* song_selected_cb() */

void search_bar_cb(GtkSearchBar *search_bar, gpointer ptr) {

} /* search_bar_cb() */

void time_scale_cb(GtkSpinButton *button, gpointer ptr) {

} /* time_scale_cb() */

gboolean draw_cb(GtkDrawingArea *drawing_area, cairo_t *cairo, gpointer ptr) {
  return false;
} /* draw_cb() */

void warp_time_cb(GtkSpinButton *button, gpointer ptr) {
  g_print("value changed!\n");
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
