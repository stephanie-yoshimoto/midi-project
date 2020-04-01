/* Stephanie Yoshimoto, parser.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "parser.h"

#include <assert.h>
#include <malloc.h>
#include <malloc_debug.h>
#include <string.h>

#define IDENTIFIER_LEN (4)

uint8_t g_prev_status = 0x80;

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
  track_list = malloc(sizeof(track_node_t));
  assert(track_list);
  song->track_list = track_list;
  song->track_list->next_track = NULL;
  song->track_list->track = NULL;
  while (feof(file_ptr_in) == 0) {
    parse_track(file_ptr_in, song);
  }

  assert(feof(file_ptr_in) == 0);
  return song;
} /* parse_file() */

void parse_header(FILE *file_ptr_in, song_data_t *song) {
  char chunk_type[IDENTIFIER_LEN + 1];
  int returned_value = fread(chunk_type, sizeof(chunk_type) - 1, 1,
                             file_ptr_in);
  if (returned_value != 1) {
    return;
  }
  assert(strcmp(chunk_type, "MThd") == 0);

  uint8_t length_arr[4];
  for (int i = 3; i >= 0; i--) {
    returned_value = fread(&length_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return;
    }
  }

  uint8_t format_arr[2];
  for (int i = 1; i >= 0; i--) {
    returned_value = fread(&format_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return;
    }
  }
  uint16_t format = end_swap_16(format_arr);
  uint8_t cast_format = (uint8_t) format;
  assert((cast_format == 0) || (cast_format == 1) || (cast_format == 2));

  uint8_t tracks_arr[2];
  for (int i = 1; i >= 0; i--) {
    returned_value = fread(&tracks_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return;
    }
  }
  uint16_t num_tracks = end_swap_16(tracks_arr);

  uint8_t division_arr[2];
  for (int i = 1; i >= 0; i--) {
    returned_value = fread(&division_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return;
    }
  }
  uint16_t division = end_swap_16(division_arr);

  assert((sizeof(format) + sizeof(num_tracks) + sizeof(division)) == 6);
  song->format = cast_format;
  song->num_tracks = num_tracks;
  if ((division & 0x8000) == 0) {
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
  int returned_value = fread(chunk_type, sizeof(chunk_type) - 1, 1,
                             file_ptr_in);
  if (returned_value != 1) {
    return;
  }
  assert(strcmp(chunk_type, "MTrk") == 0);

  uint8_t length_arr[4];
  for (int i = 3; i >= 0; i--) {
    returned_value = fread(&length_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return;
    }
  }
  uint32_t length = end_swap_32(length_arr);

  track_t *track = NULL;
  track = malloc(sizeof(track_t));
  assert(track);
  track->length = length;
  track->event_list = NULL;
  track->event_list = malloc(sizeof(event_node_t));
  assert(track->event_list);
  track->event_list->next_event = NULL;
  song->track_list->track = track;

  int num_events = (length / sizeof(event_t));
  event_node_t *event_list = track->event_list;
  for (int i = 0; i < num_events; i++) {
    event_list->event = parse_event(file_ptr_in);
    if (!event_list->event) {
      break;
    }

    event_list->next_event = malloc(sizeof(event_node_t));
    assert(event_list->next_event);
    event_list = event_list->next_event;
  }
} /* parse_track() */

event_t *parse_event(FILE *file_ptr_in) {
  event_t *event = NULL;
  event = malloc(sizeof(event_t));
  assert(event);

  if (feof(file_ptr_in)) {
    event = NULL;
    return event;
  }

  uint32_t delta_time = parse_var_len(file_ptr_in);
  event->delta_time = delta_time;

  uint8_t type = 0;
  int returned_value = fread(&type, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return event;
  }
  event->type = type;
  type = event_type(event);

  switch (type) {
    case META_EVENT_T: {
      event->meta_event = parse_meta_event(file_ptr_in);
      break;
    }
    case SYS_EVENT_T: {
      event->sys_event = parse_sys_event(file_ptr_in, type);
      break;
    }
    case MIDI_EVENT_T: {
      event->midi_event = parse_midi_event(file_ptr_in, type);
      break;
    }
  }

  return event;
} /* parse_event() */

sys_event_t parse_sys_event(FILE *file_ptr_in, uint8_t type) {
  sys_event_t sys_event = {0, NULL};

  if (feof(file_ptr_in)) {
    return sys_event;
  }

  uint32_t data_len = parse_var_len(file_ptr_in);
  sys_event.data_len = data_len;

  uint8_t data[data_len];
  for (int i = 0; i < data_len; i++) {
    int returned_value = fread(&data[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return sys_event;
    }
  }
  sys_event.data = data;

  g_prev_status = 0x80;
  return sys_event;
} /* parse_sys_event() */

meta_event_t parse_meta_event(FILE *file_ptr_in) {
  meta_event_t meta_event = {NULL, 0, NULL};

  if (feof(file_ptr_in)) {
    return meta_event;
  }

  uint8_t type_2 = 0;
  int returned_value = fread(&type_2, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return meta_event;
  }

  uint32_t data_len = parse_var_len(file_ptr_in);
  if (META_TABLE[type_2].data_len != 0) {
    assert(data_len == META_TABLE[type_2].data_len);
  }
  meta_event.data_len = data_len;

  /* if event is not valid, name should be NULL in table */

  assert(META_TABLE[type_2].name);
  meta_event.name = META_TABLE[type_2].name;

  uint8_t data[data_len];
  for (int i = 0; i < data_len; i++) {
    returned_value = fread(&data[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return meta_event;
    }
  }
  meta_event.data = data;

  g_prev_status = 0x80;
  return meta_event;
} /* parse_meta_event() */

midi_event_t parse_midi_event(FILE *file_ptr_in, uint8_t type) {
  midi_event_t midi_event = {NULL, 0, 0, NULL};

  if (feof(file_ptr_in)) {
    return midi_event;
  }

  uint8_t status = 0;
  int returned_value = fread(&status, sizeof(uint8_t), 1, file_ptr_in);
  if (returned_value != 1) {
    return midi_event;
  }

  if (status < 0x80) {
    midi_event.status = g_prev_status;
  }
  else {
    midi_event.status = status;
    g_prev_status = status;
  }

  assert(MIDI_TABLE[midi_event.status].status == midi_event.status);
  assert(MIDI_TABLE[midi_event.status].name);
  midi_event.name = MIDI_TABLE[midi_event.status].name;
  midi_event.data_len = MIDI_TABLE[midi_event.status].data_len;

  uint8_t data[midi_event.data_len];
  for (int i = 0; i < midi_event.data_len; i++) {
    int returned_value = fread(&data[i], sizeof(uint8_t), 1, file_ptr_in);
    if (returned_value != 1) {
      return midi_event;
    }
  }
  midi_event.data = data;

  return midi_event;
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

uint8_t event_type(event_t *event) {
  switch (event->type) {
    case META_EVENT:
      return META_EVENT_T;
    case SYS_EVENT_1:
    case SYS_EVENT_2:
      return SYS_EVENT_T;
    default:
      return MIDI_EVENT_T;
  }
} /* event_type() */

void free_song(song_data_t *song) {
  if (song) {
    free(song->path);
    song->path = NULL;
    free_track_node(song->track_list);
    song->track_list = NULL;
    free(song);
    song = NULL;
  }
}

void free_track_node(track_node_t *track_node) {
  if (track_node) {
    if ((!track_node->next_track) || (track_node == track_node->next_track)) {
      free_event_node(track_node->track->event_list);
      track_node->track->event_list = NULL;
      free(track_node->track);
      track_node->track = NULL;
      free(track_node);
      track_node = NULL;
    }
  }
} /* free_track_node() */

void free_event_node(event_node_t *event_node) {
  if (event_node) {
    if ((!event_node->next_event) || (event_node == event_node->next_event)) {
      free(event_node->event);
      event_node->event = NULL;
      free(event_node);
      event_node = NULL;
    }
  }
} /* free_event_node() */

uint16_t end_swap_16(uint8_t endian[2]) {
  uint16_t result = 0;
  result |= endian[1];
  result <<= 8;
  result |= endian[0];
  return result;
} /* end_swap_16() */

uint32_t end_swap_32(uint8_t endian[4]) {
  uint32_t result = 0;
  for (int i = 3; i > 0; i--) {
    result |= endian[i];
    result <<= 8;
  }
  result |= endian[0];
  return result;
} /*end_swap_32() */
