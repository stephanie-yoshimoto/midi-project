/* Stephanie Yoshimoto, parser.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "parser.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#define IDENTIFIER_LEN (4)

song_data_t *parse_file(const char *file_path) {
  assert(file_path);
  FILE *file_ptr_in = NULL;
  file_ptr_in = fopen(file_path, "r");
  assert(file_ptr_in);

  song_data_t *song = NULL;
  song = malloc(sizeof(song_data_t));
  assert(song);
  char *path = NULL;
  path = malloc(strlen(file_path) + 1);
  assert(path);
  song->path = path;
  strcpy(path, file_path);
  parse_header(file_ptr_in, song);
  track_node_t *track_list = NULL;
  parse_header(file_ptr_in, song);

  track_list = malloc(sizeof(track_node_t));
  assert(track_list);
  song->track_list = track_list;
  song->track_list->next_track = NULL;
  track_t *track = NULL;
  track = malloc(sizeof(track_t));
  assert(track);
  song->track_list->track = track;
  parse_track(file_ptr_in, song);

  assert(feof(file_ptr_in) == 0);
  return song;
} /* parse_file() */

void parse_header(FILE *file_ptr_in, song_data_t *song) {
  char chunk_type[IDENTIFIER_LEN + 1];
  int returned_value = fread(chunk_type, sizeof(chunk_type), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }
  assert(strcmp(chunk_type, "MThd") == 0);

  uint32_t length = 0;
  returned_value = fread(&length, sizeof(uint32_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }

  uint16_t format = 0;
  returned_value = fread(&format, sizeof(uint16_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }

  uint16_t num_tracks = 0;
  assert((format == 0) || (format == 1) || (format == 2));
  returned_value = fread(&num_tracks, sizeof(uint16_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }

  uint16_t division = 0;
  returned_value = fread(&division, sizeof(uint16_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }

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
  char chunk_type[IDENTIFIER_LEN + 1];
  int returned_value = fread(chunk_type, sizeof(chunk_type), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }
  assert(strcmp(chunk_type, "MTrk") == 0);

  /* traverse until current end of track list to append new tracks */

  track_node_t *list = song->track_list;
  track_node_t *temp_list = list;
  while (list) {
    if (!list->next_track) {
      break;
    }
    else {
      list = list->next_track;
    }
  }

  uint32_t length = 0;
  returned_value = fread(&length, sizeof(uint32_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return;
  }
  song->track_list->track->length = length;

  int num_events = (length / sizeof(event_t));
  event_node_t *event_list = list->track->event_list;
  event_node_t *temp = event_list;
  while (event_list) {
    if (!event_list->next_event) {
      break;
    }
    else {
      event_list = event_list->next_event;
    }
  }

  for (int i = 0; i < num_events; i++) {
    event_list->next_event->event = parse_event(file_ptr_in);
    event_list = event_list->next_event;
  }

  song->track_list = temp_list;
  song->track_list->track->event_list = temp;
} /* parse_track() */

event_t *parse_event(FILE *file_ptr_in) {
  event_t *event = NULL;
  event = malloc(sizeof(event_t));
  assert(event);

  uint32_t delta_time = 0;
  int returned_value = fread(&delta_time, sizeof(uint32_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return event;
  }
  event->delta_time = delta_time;

  uint8_t type = 0;
  returned_value = fread(&type, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return event;
  }
  event->type = type;

  uint8_t data_len = 0;
  returned_value = fread(&data_len, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return event;
  }

  /* need to do something with data_len (it means amount of events in track) */

  switch (type) {
    case (META_EVENT): {
      event->meta_event = parse_meta_event(file_ptr_in);
      break;
    }
    case ((SYS_EVENT_1) || (SYS_EVENT_2)): {
      event->sys_event = parse_sys_event(file_ptr_in, data_len);
      break;
    }
    default: {
      event->midi_event = parse_midi_event(file_ptr_in, data_len);
      break;
    }
  }
  return event;
} /* parse_event() */

sys_event_t parse_sys_event(FILE *file_ptr_in, uint8_t data_len) {
  sys_event_t *sys_event = malloc(sizeof(sys_event_t));
  assert(sys_event);
  sys_event->data_len = data_len;

  uint8_t *data = NULL;
  data = malloc(sizeof(uint8_t) * data_len);
  assert(data);
  sys_event->data = data;

  return *sys_event;
} /* parse_sys_event() */

meta_event_t parse_meta_event(FILE *file_ptr_in) {
  meta_event_t *meta_event = malloc(sizeof(meta_event_t));
  assert(meta_event);

  char *name = 0;
  int returned_value = fread(name, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return *meta_event;
  }
  meta_event->name = name;

  uint32_t data_len = 0;
  returned_value = fread(&data_len, sizeof(uint32_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return *meta_event;
  }
  meta_event->data_len = data_len;
  assert(META_TABLE[(int) *name].name);
  assert(META_TABLE[(int) *name].data_len == data_len);

  uint8_t *data = NULL;
  data = malloc(sizeof(uint8_t) * data_len);
  assert(data);
  meta_event->data = data;
  returned_value = fread(data, sizeof(data), 1, file_ptr_in);
  if (returned_value != 1) {
    return *meta_event;
  }

  return *meta_event;
} /* parse_meta_event() */

midi_event_t parse_midi_event(FILE *file_ptr_in, uint8_t data_len) {
  midi_event_t *midi_event = malloc(sizeof(midi_event_t));
  assert(midi_event);

  char *name = 0;
  int returned_value = fread(name, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return *midi_event;
  }
  midi_event->name = name;
  midi_event->data_len = data_len;

  uint8_t *data = NULL;
  data = malloc(sizeof(uint8_t) * data_len);
  assert(data);
  midi_event->data = data;
  returned_value = fread(data, sizeof(data), 1, file_ptr_in);
  if (returned_value != 1) {
    return *midi_event;
  }

  return *midi_event;
} /* parse_midi_event() */

uint32_t parse_var_len(FILE *file_ptr_in) {
  uint8_t mask = 128;
  uint32_t parsed = 0;
  uint8_t temp = 0;
  while (mask == 128) {
    int returned_value = fread(&temp, sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return parsed;
    }

    mask = temp & 128;
    if (mask == 128) {
      temp <<= 1;
      parsed <<= 7;
      parsed |= temp;
    }
  }

  temp <<= 1;
  parsed <<= 7;
  parsed |= temp;

  parsed >>= 1;
  return parsed;
} /* parse_var_len() */

uint16_t end_swap_16(uint8_t endian[2]) {
  return 0;
} /* end_swap_16() */

uint32_t end_swap_32(uint8_t endian[4]) {
  return 0;
} /*end_swap_32() */

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
