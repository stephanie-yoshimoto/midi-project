/* Stephanie Yoshimoto, parser.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "parser.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#define MTHD (4)

song_data_t *parse_file(const char *file_path) {
  return NULL;
} /* parse_file() */

void parse_header(FILE *file_ptr_in, song_data_t *song) {
  char chunk_type[MTHD + 1];
  uint32_t length = 0;
  uint16_t format = 0;
  uint16_t num_tracks = 0;
  uint16_t division = 0;
  int returned_value = fscanf(file_ptr_in, "%4s%u%hu%hu%hu\n", chunk_type,
                              &length, &format, &num_tracks, &division);
  if (returned_value != 5) {
    return;
  }

  assert(strcmp(chunk_type, "MThd") == 0);
  assert(sizeof(format + num_tracks + division) == 6);

  song->format = (uint8_t) format;
  song->num_tracks = num_tracks;

  if ((division << 15) == 0) {
    song->division.uses_tpq = true;
    song->division.ticks_per_qtr = division;
  }
  else {
    song->division.uses_tpq = false;
    song->division.ticks_per_frame = (division >> 8);
    song->division.frames_per_sec = ((uint8_t) division);
  }
} /* parse_header() */

void parse_track(FILE *file_ptr_in, song_data_t *song) {
  char chunk_type[MTHD + 1];
  uint32_t length = 0;
  uint32_t delta_time = 0;
  int returned_value = fscanf(file_ptr_in, "%4s%u%u", chunk_type, &length,
                              delta_time,);
  if (returned_value != 3) {
    return;
  }

  uint32_t data_len = 0;
  uint8_t *data = NULL;
  sys_event_t sys_event =
  meta_event_t meta_event =
  midi_event_t midi_event = 
  returned_value = fscanf(file_ptr_in, "|%u%u|", &data_len, data);
  if (returned_value != ) {
    return;
  }

  assert(strcmp(chunk_type, "MThd") == 0);
  
} /* parse_track() */

/* Define parse_event here */

/* Define parse_sys_event here */

/* Define parse_meta_event here */

/* Define parse_midi_event here */

/* Define parse_var_len here */

uint8_t event_type(event_t *event) {
  return 0;
} /* event_type() */

void free_song(song_data_t *song) {
  if (song) {
    free(song->path);
    song->path = NULL;

    /* are uint_t's malloc'd and need to be freed? */
    /* free all malloc'd elements in division, then free division itself */

    free_track_node(song->track_list);
    song->track_list = NULL;
  }
}

void free_track_node(track_node_t *track_node) {
  if (track_node) {
    if ((!track_node->next_track) || (track_node == track_node->next_track)) {
      free_event_node(track_node->track->event_list);
      track_node->track->event_list = NULL;
      free(track_node->track);
      track_node->track = NULL;
    }
  }
} /* free_track_node() */

void free_event_node(event_node_t *event_node) {
  if (event_node) {
    if ((!event_node->next_event) || (event_node == event_node->next_event)) {
      /* free & null union that has structs in it (midi is largest byte wise) */

      free(event_node->event);
      event_node->event = NULL;
    }
  }
} /* free_event_node() */

/* Define end_swap_16 here */

/* Define end_swap_32 here */
