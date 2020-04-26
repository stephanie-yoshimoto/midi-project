/* Stephanie Yoshimoto, alterations.c, CS 24000, Spring 2020
 * Last updated April 9, 2020
 */

#include "alterations.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#define MODIFIED (1)
#define NOT_MODIFIED (0)
#define PROGRAM_CHANGE_EVENT (1)
#define NON_PROGRAM_CHANGE_EVENT (0)
#define FSEEK_SUCCESS (0)
#define NOTE (1)
#define NO_NOTE (0)

int change_event_octave(event_t *event, int *num_octaves) {
  uint8_t type = event_type(event);
  if ((type == MIDI_EVENT_T) && (event->midi_event.status >= 0x80) &&
      (event->midi_event.status <= 0xAF)) {
    /* note on/off or polyphonic key event */

    int oct_inc = event->midi_event.data[0] + (OCTAVE_STEP * *num_octaves);
    if ((oct_inc >= 0) && (oct_inc <= 127)) {
      event->midi_event.data[0] = (uint8_t) oct_inc;
      return MODIFIED;
    }
  }
  return NOT_MODIFIED;
} /* change_event_octave() */

int bytes_in_var_len(uint32_t num) {
  if (num < 0x80) {
    return 1;
  }
  else if (num < 0x4000) {
    return 2;
  }
  else if (num < 0x200000) {
    return 3;
  }
  else {
    return 4;
  }
} /* bytes_in_var_len() */

int change_event_time(event_t *event, float *multiplier) {
  int prev_delta_time = bytes_in_var_len(event->delta_time);
  event->delta_time *= *multiplier;
  int new_delta_time = bytes_in_var_len(event->delta_time);
  return new_delta_time - prev_delta_time;
} /* change_event_time() */

int change_event_instrument(event_t *event, remapping_t instrument_table) {
  uint8_t type = event_type(event);
  if ((type == MIDI_EVENT_T) && (event->midi_event.status >= 0xC0) &&
      (event->midi_event.status <= 0xCF)) {
    /* event is program change event */

    event->midi_event.data[0] = instrument_table[event->midi_event.data[0]];
    return PROGRAM_CHANGE_EVENT;
  }
  return NON_PROGRAM_CHANGE_EVENT;
} /* change_event_instrument() */

int change_event_note(event_t *event, remapping_t note_table) {
  uint8_t type = event_type(event);
  if (type == MIDI_EVENT_T) {
    if ((event->midi_event.status >= 0x80) && (event->midi_event.status <=
        0xAF)) {
      /* event is either note off, note on, or polyphonic key */

      event->midi_event.data[0] = note_table[event->midi_event.data[0]];
      return NOTE;
    }
  }
  return NO_NOTE;
} /* change_event_note() */

int apply_to_events(song_data_t *song, event_func_t func, void *data) {
  int sum_return_values = 0;
  if (song) {
    track_node_t *track_list = song->track_list;
    while (track_list) {
      if (track_list->track) {
        event_node_t *event_list = track_list->track->event_list;
        while (event_list) {
          sum_return_values += (*func)(event_list->event, data);
          event_list = event_list->next_event;
        }
      }
      track_list = track_list->next_track;
    }
  }
  return sum_return_values;
} /* apply_to_events() */

int change_octave(song_data_t *song, int num_octaves) {
  return apply_to_events(song, (event_func_t) change_event_octave,
                         &num_octaves);
} /* change_octaves() */

int warp_time(song_data_t *song, float multiplier) {
  return apply_to_events(song, (event_func_t) change_event_time, &multiplier);
} /* warp_time() */

int remap_instruments(song_data_t *song, remapping_t instrument_table) {
  return apply_to_events(song, (event_func_t) change_event_instrument,
                         instrument_table);
} /* remap_instruments() */

int remap_notes(song_data_t *song, remapping_t note_table) {
  return apply_to_events(song, (event_func_t) change_event_note,
                         note_table);
} /* remap_notes() */

void add_round(song_data_t *song, int track_index, int octave_diff,
               unsigned int time_delay, uint8_t instrument) {
  assert((song) && (song->format != 2) && (track_index >= 0) &&
         (track_index < song->num_tracks));

  track_node_t *original_list = song->track_list;
  while (original_list) {
    if (!original_list->next_track) {
      break;
    }
    else {
      original_list = original_list->next_track;
    }
  }

  song_data_t *song_copy = parse_file(song->path);
  assert(song_copy);
  track_node_t *track_list = song_copy->track_list;
  track_node_t *prev_node = song_copy->track_list;
  bool appended = false;
  track_node_t *new_track_node = NULL;
  for (int i = 0; (i <= track_index) && (track_list); i++) {
    if (i == track_index) {
      prev_node->next_track = track_list->next_track;
      if (track_index == 0) {
        song_copy->track_list = prev_node->next_track;
      }
      original_list->next_track = track_list;
      track_list->next_track = NULL;
      new_track_node = track_list;
      appended = true;
      free_song(song_copy);
      break;
    }
    prev_node = track_list;
    track_list = track_list->next_track;
  }

  if (!appended) {
    return;
  }

  /* calculates smallest channel number not already in song */

  track_list = song->track_list;
  uint8_t smallest_channel_no = 0x10;
  uint8_t channel_arr[0x10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  while (track_list) {
    if (!track_list->next_track) {
      /* don't account for track that was just added */

      break;
    }
    else if (track_list->track) {
      event_node_t *event_list = track_list->track->event_list;
      while (event_list) {
        if ((event_type(event_list->event) == MIDI_EVENT_T) &&
            (event_list->event->midi_event.status >= 0x80) &&
            (event_list->event->midi_event.status <= 0xEF)) {
          uint8_t temp = event_list->event->midi_event.status & 0xF;
          if (temp < smallest_channel_no) {
            smallest_channel_no = temp;
          }

          channel_arr[temp] = 1;
        }
        event_list = event_list->next_event;
      }
    }
    track_list = track_list->next_track;
  }

  int channels_used = 0;
  for (int i = 0; i < 0x10; i++) {
    if (channel_arr[i] == 1) {
      channels_used++;
    }
  }
  assert(channels_used < 0x10);
  for (int i = 0; i < 0x10; i++) {
    if (channel_arr[i] == 0) {
      smallest_channel_no = i;
      break;
    }
  }

  /* changes octave, instruments, sets smallest channel number */

  if (new_track_node->track) {
    event_node_t *temp_events = new_track_node->track->event_list;
    if (temp_events) {
      if (temp_events->event) {
        int prev_length = bytes_in_var_len(temp_events->event->delta_time);
        temp_events->event->delta_time += time_delay;
        int new_length = bytes_in_var_len(temp_events->event->delta_time);
        new_length -= prev_length;
        new_track_node->track->length += new_length;
      }
    }

    while (temp_events) {
      change_event_octave(temp_events->event, &octave_diff);
      remapping_t instrument_map = {};
      for (int i = 0; i <= 0xFF; i++) {
        instrument_map[i] = instrument;
      }
      change_event_instrument(temp_events->event, instrument_map);

      if ((event_type(temp_events->event) == MIDI_EVENT_T) &&
          (temp_events->event->midi_event.status >= 0x80) &&
          (temp_events->event->midi_event.status <= 0xEF)) {
        uint8_t old_status = temp_events->event->midi_event.status;
        uint8_t new_status = old_status;
        new_status &= 0xF0;
        new_status |= smallest_channel_no;
        temp_events->event->midi_event.status = new_status;
        temp_events->event->type = new_status;
      }

      temp_events = temp_events->next_event;
    }
  }

  if (new_track_node->track) {
    song->num_tracks++;
    song->format = 1;
  }
} /* add_round() */

/*
 * Function called prior to main that sets up random mapping tables
 */

void build_mapping_tables()
{
  for (int i = 0; i <= 0xFF; i++) {
    I_BRASS_BAND[i] = 61;
  }

  for (int i = 0; i <= 0xFF; i++) {
    I_HELICOPTER[i] = 125;
  }

  /*for (int i = 0; i <= 0xFF; i++) {
    I_CHOIR[i] = 53;
  }

  for (int i = 0; i <= 0xFF; i++) {
    I_ACOUSTIC[i] = 24;
  }

  for (int i = 0; i <= 0xFF; i++) {
    I_TELEPHONE[i] = 124;
  }

  for (int i = 0; i <= 0xFF; i++) {
    I_SEASHORE[i] = 122;
  }*/

  for (int i = 0; i <= 0xFF; i++) {
    N_LOWER[i] = i;
  }
  //  Swap C# for C
  for (int i = 1; i <= 0xFF; i += 12) {
    N_LOWER[i] = i-1;
  }
  //  Swap F# for G
  for (int i = 6; i <= 0xFF; i += 12) {
    N_LOWER[i] = i+1;
  }
} /* build_mapping_tables() */
