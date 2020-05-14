/* Stephanie Yoshimoto, ui.c, CS 24000, Spring 2020
 * Last updated April 9, 2020
 */

#include "ui.h"

#include <gtk/gtk.h>
#include <ftw.h>

tree_node_t *g_current_node = NULL;
song_data_t *g_current_song = NULL;
song_data_t *g_modified_song = NULL;

struct ui_widgets {
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *left_box;
  GtkWidget *right_box;
  GtkWidget *left_upper_box;
  GtkWidget *left_lower_box;
  GtkWidget *add_song_btn;
  GtkWidget *load_dir_btn;
  GtkWidget *song_list;
  GtkWidget *scroll_window;
  GtkWidget *search_bar;
  GtkWidget *search_entry;
  GtkWidget *info_box;
  GtkWidget *song_info;
  GtkWidget *t_scale_box;
  GtkWidget *t_scale_label;
  GtkAdjustment *t_scale_adjustment;
  GtkWidget *t_scale_spin;
  GtkWidget *drawing_1_box;
  GtkWidget *drawing_1_label;
  GtkWidget *drawing_1;
  cairo_t *cairo_1;
  cairo_surface_t *cairo_surface_1;
  GtkWidget *drawing_2_box;
  GtkWidget *drawing_2_label;
  GtkWidget *drawing_2;
  //GtkWidget *pixmap_2;
  GtkWidget *warp_time_box;
  GtkWidget *warp_time_label;
  GtkAdjustment *warp_time_adjustment;
  GtkWidget *warp_time_spin;
  GtkWidget *octave_box;
  GtkWidget *octave_label;
  GtkAdjustment *octave_adjustment;
  GtkWidget *octave_spin;
  GtkWidget *instrument_box;
  GtkWidget *instrument_label;
  GtkWidget *combo_box_text;
  GtkWidget *notes_box;
  GtkWidget *notes_label;
  GtkWidget *combo_box_2_text;
  GtkWidget *right_lower_box;
  GtkWidget *remove_song_btn;
  GtkWidget *save_song_btn;
} g_widgets;

void create_row(tree_node_t *node, void *data) {
  char *song_name = node->song_name;
  GtkWidget *label = gtk_label_new(song_name);
  gtk_widget_set_size_request(label, 200, 15);
  gtk_label_set_xalign(GTK_LABEL (label), 0.0);
  gtk_container_add(GTK_CONTAINER (g_widgets.song_list), label);
  //gtk_list_box_insert(GTK_LIST_BOX (g_widgets.song_list), label, -1);
  gtk_widget_show(label);
} /* create_row() */

void update_song_list() {
  GtkListBoxRow *row = NULL;
  while ((row =
         gtk_list_box_get_row_at_index(GTK_LIST_BOX (g_widgets.song_list),
         0)) != NULL) {
    gtk_widget_destroy(GTK_WIDGET (row));
  }
  traverse_in_order(g_song_library, g_widgets.song_list,
    (traversal_func_t) create_row);
} /* update_song_list() */

void update_drawing_area() {

} /* update_drawing_area() */

void update_info() {
  const char *format =
    "File name: %s\nFull path: %s\nNote range [%d, %d]\nOriginal length: %d\n";
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.song_info), true);
  int lowest_pitch = 0;
  int highest_pitch = 0;
  int delta_time = 0;
  range_of_song(g_current_song, &lowest_pitch, &highest_pitch, &delta_time);
  char *markup = g_markup_printf_escaped(format, g_current_node->song_name,
    g_current_node->song->path, lowest_pitch, highest_pitch, delta_time);
  gtk_label_set_markup(GTK_LABEL (g_widgets.song_info), markup);
  gtk_label_set_justify(GTK_LABEL (g_widgets.song_info), GTK_JUSTIFY_LEFT);
  g_free(markup);

  update_drawing_area();
  gtk_widget_set_sensitive(g_widgets.t_scale_spin, true);
  gtk_widget_set_sensitive(g_widgets.warp_time_spin, true);
  gtk_widget_set_sensitive(g_widgets.octave_spin, true);
  gtk_widget_set_sensitive(g_widgets.combo_box_text, true);
  gtk_widget_set_sensitive(g_widgets.combo_box_2_text, true);
  gtk_widget_set_sensitive(g_widgets.save_song_btn, true);
  gtk_widget_set_sensitive(g_widgets.remove_song_btn, true);
} /* update_info() */

void update_song() {
  /*gchar *temp_song_name = g_current_node->song_name;
  tree_node_t **node_to_modify = find_parent_pointer(&g_song_library,
    temp_song_name);
  if ((!node_to_modify) || (!*node_to_modify)) {
    return;
  }

  // modify current node or current song?
  update_drawing_area();

  g_free(temp_song_name);*/
} /* update_song() */

void range_of_song(song_data_t *song, int *lowest_pitch, int *highest_pitch,
                   int *song_length) {

} /* range_of_song() */

void quit(GtkWidget *window, GdkEvent *event, gpointer gptr) {
  gtk_main_quit();
} /* quit() */

void activate(GtkApplication *app, gpointer ptr) {
  /* set window */

  g_widgets.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (g_widgets.window), "MIDI Library");
  gtk_container_set_border_width(GTK_CONTAINER (g_widgets.window), 10);
  gtk_widget_set_size_request(g_widgets.window, 900, 800);
  gtk_window_set_resizable(GTK_WINDOW (g_widgets.window), false);
  g_signal_connect(G_OBJECT (g_widgets.window), "destroy",
    G_CALLBACK (quit), NULL);

  /* boxes */

  g_widgets.box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.window), g_widgets.box);

  g_widgets.left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.box), g_widgets.left_box);
  g_widgets.right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.box), g_widgets.right_box);

  g_widgets.left_upper_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.left_box),
    g_widgets.left_upper_box);
  g_widgets.left_lower_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.left_box),
    g_widgets.left_lower_box);

  /* add song button */

  g_widgets.add_song_btn = gtk_button_new_with_label("Add Song from File");
  gtk_widget_set_size_request(g_widgets.add_song_btn, 100, 30);
  g_signal_connect(G_OBJECT (g_widgets.add_song_btn), "clicked",
    G_CALLBACK (add_song_cb), NULL);
  gtk_box_pack_start(GTK_BOX (g_widgets.left_upper_box),
    g_widgets.add_song_btn, false, false, 0);

  /* load from directory button */

  g_widgets.load_dir_btn = gtk_button_new_with_label("Load from Directory");
  g_signal_connect(G_OBJECT (g_widgets.load_dir_btn), "clicked",
    G_CALLBACK (load_songs_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.left_upper_box),
    g_widgets.load_dir_btn, false, false, 0);

  /* song list */

  g_widgets.song_list = gtk_list_box_new();
  gtk_widget_set_size_request(g_widgets.song_list, 200, 700);
  gtk_list_box_set_selection_mode(GTK_LIST_BOX (g_widgets.song_list),
    GTK_SELECTION_SINGLE);
  gtk_box_pack_start(GTK_BOX (g_widgets.left_lower_box), g_widgets.song_list,
    false, false, 0);
  g_widgets.scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (g_widgets.scroll_window),
    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  g_signal_connect(g_widgets.song_list, "row-activated",
    G_CALLBACK (song_selected_cb),
    gtk_list_box_get_selected_row(GTK_LIST_BOX (g_widgets.song_list)));
  gtk_container_add(GTK_CONTAINER (g_widgets.song_list),
    g_widgets.scroll_window);

  /* search bar */

  g_widgets.search_bar = gtk_search_bar_new();
  gtk_widget_set_size_request(g_widgets.search_bar, 200, 30);
  gtk_search_bar_set_search_mode(GTK_SEARCH_BAR (g_widgets.search_bar), true);
  g_widgets.search_entry = gtk_search_entry_new();
  gtk_widget_set_size_request(g_widgets.search_entry, 200, 30);
  g_signal_connect(g_widgets.search_entry, "search-changed",
    G_CALLBACK (search_bar_cb), g_widgets.search_bar);
  gtk_box_pack_end(GTK_BOX (g_widgets.left_lower_box), g_widgets.search_entry,
    false, false, 0);

  /* label */

  g_widgets.info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box), g_widgets.info_box);
  gtk_widget_set_size_request(g_widgets.info_box, 600, 80);
  g_widgets.song_info = gtk_label_new("Select a file from list to start...");
  gtk_label_set_xalign(GTK_LABEL (g_widgets.song_info), 0);
  gtk_label_set_yalign(GTK_LABEL (g_widgets.song_info), 0.5);
  gtk_label_set_justify(GTK_LABEL (g_widgets.song_info), GTK_JUSTIFY_LEFT);
  gtk_box_pack_start(GTK_BOX (g_widgets.info_box), g_widgets.song_info, false,
    false, 0);

  /* T-scale */

  g_widgets.t_scale_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box), g_widgets.t_scale_box);
  g_widgets.t_scale_label = gtk_label_new("T-scale:");
  gtk_widget_set_size_request(g_widgets.t_scale_label, 450, 25);
  gtk_label_set_justify(GTK_LABEL (g_widgets.t_scale_label),
    GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.t_scale_label), true);
  gtk_box_pack_start(GTK_BOX (g_widgets.t_scale_box), g_widgets.t_scale_label,
    false, false, 0);
  g_widgets.t_scale_adjustment = gtk_adjustment_new(10, 1, 10000, 1, 0, 0);
  g_widgets.t_scale_spin =
    gtk_spin_button_new(g_widgets.t_scale_adjustment, 0.0, 0);
  gtk_widget_set_size_request(g_widgets.t_scale_spin, 200, 25);
  gtk_widget_set_sensitive(g_widgets.t_scale_spin, false);
  g_signal_connect(G_OBJECT (g_widgets.t_scale_spin), "value-changed",
    G_CALLBACK (time_scale_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.t_scale_box), g_widgets.t_scale_spin,
    false, false, 0);

  /* drawing areas */

  g_widgets.drawing_1_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box),
    g_widgets.drawing_1_box);
  g_widgets.drawing_1 = gtk_drawing_area_new();
  gtk_widget_set_size_request(g_widgets.drawing_1, 600, 200);
  g_signal_connect(G_OBJECT (g_widgets.drawing_1), "draw",
    G_CALLBACK (draw_cb), NULL);
  g_widgets.drawing_1_label = gtk_label_new("Original song:");
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.drawing_1_label), true);
  gtk_box_pack_start(GTK_BOX (g_widgets.drawing_1_box),
    g_widgets.drawing_1_label, false, false, 0);
  gtk_box_pack_end(GTK_BOX (g_widgets.drawing_1_box), g_widgets.drawing_1,
    false, false, 0);
  /*g_widgets.cairo_surface_1 = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
    390, 60);
  g_widgets.cairo_1 = cairo_create(g_widgets.cairo_surface_1);
  cairo_rectangle(g_widgets.cairo_1, 5, 15, 5, 5);
  cairo_fill(g_widgets.cairo_1);
  g_widgets.cairo_1 = gdk_cairo_create();
  cairo_set_source_rgb(g_widgets.cairo_1, 1, 1, 1);
  cairo_rectangle(g_widgets.cairo_1, 0, 0, 600, 200);
  cairo_fill(g_widgets.cairo_1);
  gtk_container_add(GTK_CONTAINER (g_widgets.drawing_1), g_widgets.cairo_1);
  gtk_box_pack_start(GTK_BOX (g_widgets.right_box), g_widgets.drawing_1, false,
    false, 0);*/
  g_widgets.drawing_2_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box),
    g_widgets.drawing_2_box);
  g_widgets.drawing_2 = gtk_drawing_area_new();
  gtk_widget_set_size_request(g_widgets.drawing_2, 600, 200);
  g_signal_connect(G_OBJECT (g_widgets.drawing_2), "draw",
    G_CALLBACK (draw_cb), NULL);
  g_widgets.drawing_2_label = gtk_label_new("After effect:");
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.drawing_2_label), true);
  gtk_box_pack_start(GTK_BOX (g_widgets.drawing_2_box),
    g_widgets.drawing_2_label, false, false, 0);
  gtk_box_pack_end(GTK_BOX (g_widgets.drawing_2_box),
    g_widgets.drawing_2, false, false, 0);

  /* warp time */

  g_widgets.warp_time_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box),
    g_widgets.warp_time_box);
  g_widgets.warp_time_label = gtk_label_new("Warp time:");
  gtk_box_pack_start(GTK_BOX (g_widgets.warp_time_box),
    g_widgets.warp_time_label, false, false, 0);
  gtk_widget_set_size_request(g_widgets.warp_time_label, 450, 25);
  gtk_label_set_justify(GTK_LABEL (g_widgets.warp_time_label),
    GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.warp_time_label), true);
  g_widgets.warp_time_adjustment = gtk_adjustment_new(1.0, 0.1, 10.0, 0.1, 0,
    0);
  g_widgets.warp_time_spin =
    gtk_spin_button_new(g_widgets.warp_time_adjustment, 0.0, 1);
  gtk_widget_set_size_request(g_widgets.warp_time_spin, 200, 25);
  gtk_widget_set_sensitive(g_widgets.warp_time_spin, false);
  g_signal_connect(G_OBJECT (g_widgets.warp_time_spin), "changed",
    G_CALLBACK (warp_time_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.warp_time_box), g_widgets.warp_time_spin,
    false, false, 0);

  /* song octave */

  g_widgets.octave_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box), g_widgets.octave_box);
  g_widgets.octave_label = gtk_label_new("Change octave:");
  gtk_label_set_justify(GTK_LABEL (g_widgets.octave_label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (g_widgets.octave_box), g_widgets.octave_label,
    false, false, 0);
  gtk_widget_set_size_request(g_widgets.octave_label, 450, 25);
  gtk_label_set_justify(GTK_LABEL (g_widgets.octave_label),
    GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.octave_label), true);
  g_widgets.octave_adjustment = gtk_adjustment_new(0, -5, 5, 1, 0, 0);
  g_widgets.octave_spin =
    gtk_spin_button_new(g_widgets.octave_adjustment, 0.0, 0);
  gtk_widget_set_size_request(g_widgets.octave_spin, 200, 25);
  gtk_widget_set_sensitive(g_widgets.octave_spin, false);
  g_signal_connect(G_OBJECT (g_widgets.octave_spin), "changed",
    G_CALLBACK (song_octave_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.octave_box), g_widgets.octave_spin,
    false, false, 0);

  /* instruments */

  g_widgets.instrument_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box),
    g_widgets.instrument_box);
  g_widgets.instrument_label = gtk_label_new("Remap instruments:");
  gtk_label_set_justify(GTK_LABEL (g_widgets.instrument_label),
    GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (g_widgets.instrument_box),
    g_widgets.instrument_label, false, false, 0);
  gtk_widget_set_size_request(g_widgets.instrument_label, 450, 25);
  gtk_label_set_justify(GTK_LABEL (g_widgets.instrument_label),
    GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.instrument_label), true);
  g_widgets.combo_box_text = gtk_combo_box_text_new();
  gtk_widget_set_size_request(g_widgets.combo_box_text, 200, 25);
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (g_widgets.combo_box_text),
    NULL, "Brass band");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (g_widgets.combo_box_text),
    NULL, "Helicopter");
  gtk_widget_set_sensitive(g_widgets.combo_box_text, false);
  g_signal_connect(G_OBJECT (g_widgets.combo_box_text), "changed",
    G_CALLBACK (instrument_map_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.instrument_box), g_widgets.combo_box_text,
    false, false, 0);

  /* notes */

  g_widgets.notes_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box), g_widgets.notes_box);
  g_widgets.notes_label = gtk_label_new("Remap notes:");
  gtk_label_set_justify(GTK_LABEL (g_widgets.notes_label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX (g_widgets.notes_box), g_widgets.notes_label,
    false, false, 0);
  gtk_widget_set_size_request(g_widgets.notes_label, 450, 25);
  gtk_label_set_justify(GTK_LABEL (g_widgets.notes_label),
    GTK_JUSTIFY_CENTER);
  gtk_label_set_line_wrap(GTK_LABEL (g_widgets.notes_label), true);
  g_widgets.combo_box_2_text = gtk_combo_box_text_new();
  gtk_widget_set_size_request(g_widgets.combo_box_2_text, 200, 25);
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT (g_widgets.combo_box_2_text),
    NULL, "Lower");
  gtk_widget_set_sensitive(g_widgets.combo_box_2_text, false);
  g_signal_connect(G_OBJECT (g_widgets.combo_box_2_text), "changed",
    G_CALLBACK (note_map_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.notes_box), g_widgets.combo_box_2_text,
    false, false, 0);

  /* save/remove buttons */

  g_widgets.right_lower_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add(GTK_CONTAINER (g_widgets.right_box),
    g_widgets.right_lower_box);
  g_widgets.save_song_btn = gtk_button_new_with_label("Save Song");
  gtk_widget_set_size_request(g_widgets.save_song_btn, 325, 30);
  gtk_widget_set_sensitive(g_widgets.save_song_btn, false);
  g_signal_connect(G_OBJECT (g_widgets.save_song_btn), "clicked",
    G_CALLBACK (save_song_cb), NULL);
  gtk_box_pack_start(GTK_BOX (g_widgets.right_lower_box),
    g_widgets.save_song_btn, false, false, 0);
  g_widgets.remove_song_btn = gtk_button_new_with_label("Remove Song");
  gtk_widget_set_size_request(g_widgets.remove_song_btn, 325, 30);
  gtk_widget_set_sensitive(g_widgets.remove_song_btn, false);
  g_signal_connect(G_OBJECT (g_widgets.remove_song_btn), "clicked",
    G_CALLBACK (remove_song_cb), NULL);
  gtk_box_pack_end(GTK_BOX (g_widgets.right_lower_box),
    g_widgets.remove_song_btn, false, false, 0);

  /* add widgets to window */

  gtk_widget_show_all(g_widgets.window);
  gtk_main();
} /* activate() */

void add_song_cb(GtkButton *button, gpointer ptr) {
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_transient_for(GTK_WINDOW (window), NULL);
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
    GTK_WINDOW (window), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel",
    GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_OK, NULL);
  gtk_widget_show_all(dialog);

  gint status = gtk_dialog_run(GTK_DIALOG (dialog));
  if (status == GTK_RESPONSE_OK) {
    gchar *file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
    make_library(file_name);
    update_song_list();
    gtk_widget_destroy(dialog);
    g_free(file_name);
  }
  else if (status == GTK_RESPONSE_CANCEL) {
    gtk_widget_destroy(dialog);
  }
} /* add_song_cb() */

void load_songs_cb(GtkButton *button, gpointer ptr) {
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_transient_for(GTK_WINDOW (window), NULL);
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Choose Folder",
    GTK_WINDOW (window), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "_Cancel",
    GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_OK, NULL);
  gtk_widget_show_all(dialog);

  gint status = gtk_dialog_run(GTK_DIALOG (dialog));
  if (status == GTK_RESPONSE_OK) {
    gchar *dir_name =
      gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
    make_library(dir_name);
    update_song_list();
    gtk_widget_destroy(dialog);
    g_free(dir_name);
  }
  else if (status == GTK_RESPONSE_CANCEL) {
    gtk_widget_destroy(dialog);
  }
} /* load_songs_cb() */

void song_selected_cb(GtkListBox *list_box, GtkListBoxRow *row) {
  g_print("hellooo\n");
  //const gchar *song_name = gtk_label_get_text(GTK_LABEL (row));
  // ooh put label in row container, insert row in list box, get child of
  // row, get text
  /*GtkWidget *label = NULL;*/
  gchar *song_name = "prelude2.mid";
  /*gtk_container_child_get(GTK_CONTAINER (row), label, "label",
    song_name, NULL);*/

  tree_node_t **found_node = find_parent_pointer(&g_song_library, song_name);
  g_assert((found_node) && (*found_node));
  g_current_node = *found_node;
  update_info();
} /* song_selected_cb() */

void filter_search(GtkWidget *search_entry, gpointer search_bar) {
  const gchar *match_this = gtk_entry_get_text(GTK_ENTRY (search_entry));
  g_print("%s\n", match_this);
  /*gint length_of_search = strlen(match_this);*/
  // if row does not contain for first n, invalidate
  /*Gtk
  if (strncmp(match_this, */
} /* filter_search() */

void find_matches(gpointer data, gpointer search_entry) {
  const gchar *match_this = gtk_entry_get_text(GTK_ENTRY (search_entry));
  GList *children = (GList *)(data);
  gpointer ptr = g_list_nth_data(children, 0);
  gchar *song_name = (gchar *)(ptr);
  g_print("song name %s\n", song_name);
  g_print("%s\n", match_this);
  g_free(song_name);
} /* find_matches() */

void search_bar_cb(GtkSearchBar *search_entry, gpointer search_bar) {
  GList *children =
    gtk_container_get_children(GTK_CONTAINER (g_widgets.song_list));
  g_print("children\n");
  g_list_foreach(children, find_matches, search_entry);
  /*GtkListBoxRow *row = NULL;
  while ((row =
         gtk_list_box_get_row_at_index(GTK_LIST_BOX (g_widgets.song_list), 0))
         != NULL) {
    filter_search(GTK_WIDGET (search_entry), search_bar);
  }*/
} /* search_bar_cb() */

void time_scale_cb(GtkSpinButton *button, gpointer ptr) {

} /* time_scale_cb() */

gboolean draw_cb(GtkDrawingArea *drawing_area, cairo_t *cairo, gpointer ptr) {
  GtkWidget *horizontal_scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (horizontal_scroll),
    GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
  GtkStyleContext *context =
    gtk_widget_get_style_context(GTK_WIDGET (drawing_area));
  guint width = gtk_widget_get_allocated_width(GTK_WIDGET (drawing_area));
  guint height = gtk_widget_get_allocated_height(GTK_WIDGET (drawing_area));
  gtk_render_background(context, cairo, 0, 0, width, height);
  cairo_rectangle(cairo, 0, 0, 680, 200);
  GdkRGBA color = {1.0, 1.0, 1.0, 1.0};
  /*gtk_style_context_get_color(context, gtk_style_context_get_state(context),
    &color);*/
  gdk_cairo_set_source_rgba(cairo, &color);
  cairo_fill(cairo);
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
