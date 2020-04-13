/* Stephanie Yoshimoto, parser.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "parser.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#define IDENTIFIER_LEN (4)
#define UINT16_ARR (2)
#define UINT32_ARR (4)
#define BYTE (8)
#define LENGTH (6)
#define BOUND (32768)
#define FORMAT_0 (0)
#define FORMAT_1 (1)
#define FORMAT_2 (2)
#define DEFAULT_VAL (0)
#define VAR_LEN_SHIFT (7)
#define FREAD_SUCCESS (1)
#define FSEEK_SUCCESS (0)
#define MIDI_MIN (128)
#define MIDI_MAX (255)
#define MASK (128)
#define END_OF_TRACK "End of Track"
#define MTHD "MThd"
#define MTRK "MTrk"

uint8_t g_prev_type = DEFAULT_VAL;

/*
 * Read in all data from file and return it is a song struct.
 */

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
  strcpy(song->path, file_path);
  parse_header(file_ptr_in, song);

  song->track_list = NULL;
  song->track_list = malloc(sizeof(track_node_t));
  assert(song->track_list);
  song->track_list->next_track = NULL;
  song->track_list->track = NULL;
  track_node_t *list = song->track_list;

  for (int i = 0; i <= song->num_tracks; i++) {
    parse_track(file_ptr_in, song);
  }

  song->track_list = list;
  track_node_t *temp = list;

  while (list) {
    temp = list;
    list = list->next_track;
    if (!list->track) {
      free(list->next_track);
      list->next_track = NULL;
      free(list);
      list = NULL;
      temp->next_track = NULL;
    }
  }

  assert(feof(file_ptr_in));
  fclose(file_ptr_in);
  file_ptr_in = NULL;
  return song;
} /* parse_file() */

/*
 * Parse the header chunk portion of file.
 */

void parse_header(FILE *file_ptr_in, song_data_t *song) {
  assert((file_ptr_in) && (song));
  char chunk_type[IDENTIFIER_LEN + 1];
  int returned_value = fread(chunk_type, sizeof(chunk_type) - 1, 1,
                             file_ptr_in);
  assert(returned_value == FREAD_SUCCESS);
  assert(strcmp(chunk_type, MTHD) == 0);

  uint8_t length_arr[UINT32_ARR];
  for (int i = 0; i < UINT32_ARR; i++) {
    returned_value = fread(&length_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    assert(returned_value == FREAD_SUCCESS);
  }
  uint32_t length = end_swap_32(length_arr);
  assert(length == LENGTH);

  uint8_t format_arr[UINT16_ARR];
  for (int i = 0; i < UINT16_ARR; i++) {
    returned_value = fread(&format_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    assert(returned_value == FREAD_SUCCESS);
  }
  uint16_t format = end_swap_16(format_arr);
  assert((format == FORMAT_0) || (format == FORMAT_1) || (format == FORMAT_2));

  uint8_t tracks_arr[UINT16_ARR];
  for (int i = 0; i < UINT16_ARR; i++) {
    returned_value = fread(&tracks_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    assert(returned_value == FREAD_SUCCESS);
  }
  uint16_t num_tracks = end_swap_16(tracks_arr);

  uint8_t division_arr[UINT16_ARR];
  for (int i = 0; i < UINT16_ARR; i++) {
    returned_value = fread(&division_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    assert(returned_value == FREAD_SUCCESS);
  }
  uint16_t division = end_swap_16(division_arr);

  song->format = (uint8_t) format;
  song->num_tracks = num_tracks;
  if ((division & BOUND) == 0) {
    song->division.uses_tpq = true;
    song->division.ticks_per_qtr = division;
  }
  else {
    song->division.uses_tpq = false;
    song->division.ticks_per_frame = (division >> BYTE);
    song->division.frames_per_sec = ((uint8_t) division);
  }
} /* parse_header() */

/*
 * Parse a track, including event data.
 */

void parse_track(FILE *file_ptr_in, song_data_t *song) {
  assert((file_ptr_in) && (song));
  char chunk_type[IDENTIFIER_LEN + 1];
  int returned_value = fread(chunk_type, sizeof(chunk_type) - 1, 1,
                             file_ptr_in);
  if (returned_value != FREAD_SUCCESS) {
    /* EOF reached */

    return;
  }
  assert(strcmp(chunk_type, MTRK) == 0);

  if (song) {
    while (song->track_list) {
      if (!song->track_list->next_track) {
        break;
      }
      else {
        song->track_list = song->track_list->next_track;
      }
    }
  }
  else {
    return;
  }

  track_t *track = NULL;
  track = malloc(sizeof(track_t));
  assert(track);

  uint8_t length_arr[UINT32_ARR];
  for (int i = 0; i < UINT32_ARR; i++) {
    returned_value = fread(&length_arr[i], sizeof(uint8_t), 1, file_ptr_in);
    assert(returned_value == FREAD_SUCCESS);
  }
  uint32_t length = end_swap_32(length_arr);
  track->length = length;

  track->event_list = NULL;
  track->event_list = malloc(sizeof(event_node_t));
  assert(track->event_list);
  track->event_list->next_event = NULL;
  song->track_list->track = track;
  song->track_list->next_track = malloc(sizeof(track_node_t));
  assert(song->track_list->next_track);
  song->track_list->next_track->track = NULL;
  song->track_list->next_track->next_track = NULL;

  event_node_t *event_list = track->event_list;
  event_node_t *event_list_copy = track->event_list;
  int position_diff = 0;
  for (int i = 0; i < length; i += position_diff) {
    int start_position = ftell(file_ptr_in);
    event_list->event = parse_event(file_ptr_in);
    event_list->next_event = NULL;
    if (!event_list->event) {
      break;
    }
    else if ((event_list->event->type == META_EVENT) &&
             (strcmp(event_list->event->meta_event.name, END_OF_TRACK) ==
             0)) {
      break;
    }

    event_list->next_event = malloc(sizeof(event_node_t));
    assert(event_list->next_event);
    event_list->next_event->next_event = NULL;
    event_list = event_list->next_event;

    int end_position = ftell(file_ptr_in);
    position_diff = end_position - start_position;
  }

  track->event_list = event_list_copy;
} /* parse_track() */

/*
 * Parses each event by calling respective function.
 */

event_t *parse_event(FILE *file_ptr_in) {
  assert(file_ptr_in);
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
  assert(returned_value == FREAD_SUCCESS);
  event->type = type;

  switch (event_type(event)) {
    case META_EVENT_T: {
      event->meta_event = parse_meta_event(file_ptr_in);
      break;
    }
    case SYS_EVENT_T: {
      event->sys_event = parse_sys_event(file_ptr_in, event->type);
      break;
    }
    case MIDI_EVENT_T: {
      event->midi_event = parse_midi_event(file_ptr_in, event->type);
      break;
    }
  }

  return event;
} /* parse_event() */

/*
 * Parses SysEx event from file and reads into event SysEx struct.
 */

sys_event_t parse_sys_event(FILE *file_ptr_in, uint8_t type) {
  assert(file_ptr_in);
  sys_event_t sys_event = {0, NULL};

  if (feof(file_ptr_in)) {
    return sys_event;
  }

  uint32_t data_len = parse_var_len(file_ptr_in);
  sys_event.data_len = data_len;

  uint8_t *data = NULL;
  if (sys_event.data_len != 0) {
    data = malloc(sizeof(uint8_t) * data_len);
    assert(data);
    for (int i = 0; i < data_len; i++) {
      int returned_value = fread(&data[i], sizeof(uint8_t), 1, file_ptr_in);
      assert(returned_value == FREAD_SUCCESS);
    }
  }
  sys_event.data = data;

  g_prev_type = DEFAULT_VAL;
  return sys_event;
} /* parse_sys_event() */

/*
 * Parses meta event from file and reads into meta event struct.
 */

meta_event_t parse_meta_event(FILE *file_ptr_in) {
  assert(file_ptr_in);
  meta_event_t meta_event = {NULL, 0, NULL};

  if (feof(file_ptr_in)) {
    return meta_event;
  }

  uint8_t type_2 = 0;
  int returned_value = fread(&type_2, sizeof(uint8_t), 1, file_ptr_in);
  assert(returned_value == FREAD_SUCCESS);

  uint32_t data_len = parse_var_len(file_ptr_in);
  if (META_TABLE[type_2].data_len != 0) {
    assert(data_len == META_TABLE[type_2].data_len);
  }
  meta_event.data_len = data_len;

  /* if event is not valid, name should be NULL in table */

  assert(META_TABLE[type_2].name);
  meta_event.name = META_TABLE[type_2].name;

  uint8_t *data = NULL;
  if (meta_event.data_len != 0) {
    data = malloc(sizeof(uint8_t) * meta_event.data_len);
    assert(data);
    for (int i = 0; i < data_len; i++) {
      returned_value = fread(&data[i], sizeof(uint8_t), 1, file_ptr_in);
      assert(returned_value == FREAD_SUCCESS);
    }
  }
  meta_event.data = data;

  g_prev_type = DEFAULT_VAL;
  return meta_event;
} /* parse_meta_event() */

/*
 * Parses Midi event from file and returns a Midi struct.
 */

midi_event_t parse_midi_event(FILE *file_ptr_in, uint8_t type) {
  assert((file_ptr_in) && (type < MIDI_MAX));
  midi_event_t midi_event = {NULL, 0, 0, NULL};

  if (feof(file_ptr_in)) {
    return midi_event;
  }

  if (type >= MIDI_MIN) {
    assert(MIDI_TABLE[type].name);
    midi_event.name = MIDI_TABLE[type].name;
    midi_event.data_len = MIDI_TABLE[type].data_len;
    midi_event.status = MIDI_TABLE[type].status;
    g_prev_type = type;
  }
  else {
    /* type is a data byte */

    int returned_value = fseek(file_ptr_in, -1, SEEK_CUR);
    if (returned_value != FSEEK_SUCCESS) {
      return midi_event;
    }

    midi_event.status = MIDI_TABLE[g_prev_type].status;
    midi_event.name = MIDI_TABLE[g_prev_type].name;
    midi_event.data_len = MIDI_TABLE[g_prev_type].data_len;
  }

  uint8_t *data = NULL;
  if (midi_event.data_len != 0) {
    data = malloc(sizeof(uint8_t) * midi_event.data_len);
    assert(data);
    for (int i = 0; i < midi_event.data_len; i++) {
      int returned_value = fread(&data[i], sizeof(uint8_t), 1, file_ptr_in);
      assert(returned_value == FREAD_SUCCESS);
    }
  }
  midi_event.data = data;

  return midi_event;
} /* parse_midi_event() */

/*
 * Parses a variable length quantity from file.
 */

uint32_t parse_var_len(FILE *file_ptr_in) {
  assert(file_ptr_in);
  uint8_t mask = MASK;
  uint32_t parsed = 0;
  uint8_t temp = 0;
  while (mask == MASK) {
    int returned_value = fread(&temp, sizeof(uint8_t), 1, file_ptr_in);
    assert(returned_value == FREAD_SUCCESS);

    mask = temp & MASK;
    if (mask == MASK) {
      temp <<= 1;
      parsed <<= VAR_LEN_SHIFT;
      parsed |= temp;
    }
  }

  temp <<= 1;
  parsed <<= VAR_LEN_SHIFT;
  parsed |= temp;

  parsed >>= 1;
  return parsed;
} /* parse_var_len() */

/*
 * Determines type of event using internal data.
 */

uint8_t event_type(event_t *event) {
  assert(event);
  switch (event->type) {
    case META_EVENT: {
      return META_EVENT_T;
    }
    case SYS_EVENT_1: {
      return SYS_EVENT_T;
    }
    case SYS_EVENT_2: {
      return SYS_EVENT_T;
    }
    default: {
      return MIDI_EVENT_T;
    }
  }
} /* event_type() */

/*
 * Frees a song struct.
 */

void free_song(song_data_t *song) {
  if (song) {
    free(song->path);
    song->path = NULL;
    track_node_t *list = song->track_list;
    while (list) {
      track_node_t *temp = list->next_track;
      free_track_node(list);
      list = temp;
    }
    song->track_list = NULL;
    free(song);
    song = NULL;
  }
} /* free_song() */

/*
 * Frees a track struct, including event list.
 */

void free_track_node(track_node_t *track_node) {
  if (track_node) {
    if (track_node->track) {
      event_node_t *event_list = track_node->track->event_list;
      while (event_list) {
        event_node_t *temp = event_list->next_event;
        free_event_node(event_list);
        event_list = temp;
      }
      track_node->track->event_list = NULL;
      free(track_node->track);
    }
    track_node->track = NULL;
    free(track_node);
    track_node = NULL;
  }
} /* free_track_node() */

/*
 * Frees an event node depending on type of event.
 */

void free_event_node(event_node_t *event_node) {
  if (event_node) {
    if (event_node->event) {
      switch (event_node->event->type) {
        case SYS_EVENT_1: {
          if (event_node->event->sys_event.data_len == 0) {
            break;
          }
          free(event_node->event->sys_event.data);
          event_node->event->sys_event.data = NULL;
          break;
        }
        case SYS_EVENT_2: {
          if (event_node->event->sys_event.data_len == 0) {
            break;
          }
          free(event_node->event->sys_event.data);
          event_node->event->sys_event.data = NULL;
          break;
        }
        case META_EVENT: {
          if (event_node->event->meta_event.data_len == 0) {
            break;
          }
          free(event_node->event->meta_event.data);
          event_node->event->meta_event.data = NULL;
          break;
        }
        default: {
          if (event_node->event->midi_event.data_len == 0) {
            break;
          }
          free(event_node->event->midi_event.data);
          event_node->event->midi_event.data = NULL;
          break;
        }
      }
      free(event_node->event);
    }
    event_node->event = NULL;
    free(event_node);
    event_node = NULL;
  }
} /* free_event_node() */

/*
 * Changes endianness of a 2 byte data type.
 */

uint16_t end_swap_16(uint8_t endian[2]) {
  uint16_t result = 0;
  result |= endian[0];
  result <<= BYTE;
  result |= endian[1];
  return result;
} /* end_swap_16() */

/*
 * Changes endianness of a 4 byte data type.
 */

uint32_t end_swap_32(uint8_t endian[4]) {
  uint8_t first_half[UINT16_ARR] = {endian[0], endian[1]};
  uint16_t first_digit = end_swap_16(first_half);
  uint8_t second_half[UINT16_ARR] = {endian[2], endian[3]};
  uint16_t second_digit = end_swap_16(second_half);

  uint32_t result = 0;
  result |= first_digit;
  result <<= BYTE;
  result <<= BYTE;
  result |= second_digit;
  return result;
} /* end_swap_32() */
