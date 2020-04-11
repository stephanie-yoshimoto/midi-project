/* Stephanie Yoshimoto, alterations.c, CS 24000, Spring 2020
 * Last updated April 9, 2020
 */

#include "alterations.h"

#include <assert.h>

#define MODIFIED (1)
#define NOT_MODIFIED (0)
#define PROGRAM_CHANGE_EVENT (1)
#define NON_PROGRAM_CHANGE_EVENT (0)
#define FSEEK_SUCCESS (0)
#define NOTE (1)
#define NO_NOTE (0)

int change_event_octave(event_t *event, int *num_octaves) {
  uint8_t type = event_type(event);
  if ((type == MIDI_EVENT_T) && (event->midi_event.status < 0xB0)) {
    uint8_t oct_inc = event->midi_event.data[0] + (OCTAVE_STEP * *num_octaves);
    if ((oct_inc >= 0) && (oct_inc <= 0x7F)) {
      event->midi_event.data[0] = oct_inc;
      return MODIFIED;
    }
  }
  return NOT_MODIFIED;
} /* change_event_octave() */

int change_event_time(event_t *event, float *multiplier) {
  uint32_t prev_delta_time = event->delta_time;
  event->delta_time *= *multiplier;
  uint32_t new_delta_time = event->delta_time;
  return new_delta_time - prev_delta_time;
} /* change_event_time() */

int change_event_instrument(event_t *event, remapping_t instrument_table) {
  uint8_t type = event_type(event);
  if ((type == MIDI_EVENT_T) && (event->midi_event.status >= 0xC0) &&
      (event->midi_event.status <= 0xCF)) {
    /* event is program change event */

    event->midi_event.data[0] = instrument_table[event->midi_event.status];
    return PROGRAM_CHANGE_EVENT;
  }
  return NON_PROGRAM_CHANGE_EVENT;
} /* change_event_instrument() */

int change_event_note(event_t *event, remapping_t note_table) {
  uint8_t type = event_type(event);
  if (type == MIDI_EVENT_T) {
    if ((event->midi_event.status >= 0x80) && (event->midi_event.status <=
        0xAF)) {
      /* event is note off, note on, or polyphonic key */

      event->midi_event.data[0] = note_table[event->midi_event.status];
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
  if (!song) {
    return NOT_MODIFIED;
  }

  FILE *file_ptr_in = NULL;
  file_ptr_in = fopen(song->path, "r");
  if (!file_ptr_in) {
    return NOT_MODIFIED;
  }

  int returned_value = fseek(file_ptr_in, 0, SEEK_END);
  if (returned_value != FSEEK_SUCCESS) {
    return NOT_MODIFIED;
  }
  int initial_length = ftell(file_ptr_in);
  fclose(file_ptr_in);
  file_ptr_in = NULL;

  /* what do i do with value that returns below? */

  /* is it saying that since delta time will inc/dec, then the length of */
  /* track will also change bc bytes needed for var length quantity */
  /* delta time will change in each track? */

  multiplier = 1.0 / multiplier;
  apply_to_events(song, (event_func_t) change_event_octave, &multiplier);

  file_ptr_in = fopen(song->path, "r");
  if (!file_ptr_in) {
    return NOT_MODIFIED;
  }
  returned_value = fseek(file_ptr_in, 0, SEEK_END);
  if (returned_value != FSEEK_SUCCESS) {
    return NOT_MODIFIED;
  }
  int result_length = ftell(file_ptr_in);
  fclose(file_ptr_in);
  file_ptr_in = NULL;

  return result_length - initial_length;
} /* warp_time() */

int remap_instruments(song_data_t *song, remapping_t instrument_table) {
  return apply_to_events(song, (event_func_t) change_event_instrument,
                         &instrument_table);
} /* remap_instruments() */

int remap_notes(song_data_t *song, remapping_t instrument_table) {
  return apply_to_events(song, (event_func_t) change_event_note,
                         &instrument_table);
} /* remap_notes() */

void add_round(song_data_t *song, int track_index, int octave_diff,
               unsigned int time_delay, uint8_t instrument) {
  assert((song) && (song->format != 2) && (track_index >= 0) &&
         (track_index < song->num_tracks));

  song->num_tracks++;

  /* do i also have to change format 1 to 2? */

  if (song->format == 0) {
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
