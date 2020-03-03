/* Stephanie Yoshimoto, hw7.c, CS 24000, Spring 2020
 * Last updated February 19, 2020
 */

#include "hw7.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int g_password_count = 0;
password_t g_password_array[MAX_PASSWORDS];

/*
 * Reads records into char arrays, which are read into structs, which are
 * read into global array.
 */

int read_passwords(char *filename) {
  assert(filename != NULL);
  FILE *in_file_ptr = NULL;
  in_file_ptr = fopen(filename, "r");
  if (in_file_ptr == NULL) {
    return NON_READABLE_FILE;
  }

  int returned_value = fseek(in_file_ptr, 0, SEEK_END);
  if (returned_value != 0) {
    fclose(in_file_ptr);
    in_file_ptr = NULL;
    return NON_READABLE_FILE;
  }

  if (ftell(in_file_ptr) == 0) {
    fclose(in_file_ptr);
    in_file_ptr = NULL;
    return NO_PASSWORDS;
  }
  returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    fclose(in_file_ptr);
    in_file_ptr = NULL;
    return NON_READABLE_FILE;
  }

  int passwords_read = 0;
  while (feof(in_file_ptr) == 0) {
    char buffer_code_name[MAX_BUFF_LEN] = "";
    char buffer_passcode_name[MAX_BUFF_LEN] = "";
    char buffer_passcode_value[MAX_BUFF_LEN] = "";
    char buffer_next_member[MAX_BUFF_LEN] = "";
    if (fscanf(in_file_ptr, "%49[^&\n]&%49[^&\n]&%49[^&\n]&%49[^\n]\n",
        buffer_code_name, buffer_passcode_name, buffer_passcode_value,
        buffer_next_member) != 4) {
      if (feof(in_file_ptr) == 0) {
        /* not at end of file, but did not read records correctly */

        fclose(in_file_ptr);
        in_file_ptr = NULL;
        return BAD_RECORD;
      }
      break;
    }

    if ((strlen(buffer_code_name) > MAX_BUFF_LEN) ||
        (strlen(buffer_passcode_name) > MAX_BUFF_LEN) ||
        (strlen(buffer_passcode_value) > MAX_BUFF_LEN) ||
        (strlen(buffer_next_member) > MAX_BUFF_LEN)) {
      fclose(in_file_ptr);
      in_file_ptr = NULL;
      return BAD_RECORD;
    }

    if (passwords_read == MAX_PASSWORDS) {
      fclose(in_file_ptr);
      in_file_ptr = NULL;
      return TOO_MUCH_DATA;
    }

    /* passed checks, copy into temporary variables to be put into struct */
    /* terminating with NUL character */

    char temp_code_name[MAX_NAME_LEN] = "";
    char temp_passcode_name[MAX_NAME_LEN] = "";
    char temp_passcode_val[MAX_NAME_LEN] = "";
    char temp_next_member[MAX_NAME_LEN] = "";
    strncpy(temp_code_name, buffer_code_name, MAX_NAME_LEN - 1);
    temp_code_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(temp_passcode_name, buffer_passcode_name, MAX_NAME_LEN - 1);
    temp_passcode_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(temp_passcode_val, buffer_passcode_value, MAX_NAME_LEN - 1);
    temp_passcode_val[MAX_NAME_LEN - 1] = '\0';
    strncpy(temp_next_member, buffer_next_member, MAX_NAME_LEN - 1);
    temp_next_member[MAX_NAME_LEN - 1] = '\0';

    password_t temp_password = {"", "", "", "", NULL};
    strcpy(temp_password.code_name, temp_code_name);
    strcpy(temp_password.passcode_name, temp_passcode_name);
    strcpy(temp_password.passcode_value, temp_passcode_val);
    strcpy(temp_password.next_member, temp_next_member);
    g_password_array[passwords_read] = temp_password;
    passwords_read++;
  }
  g_password_count = passwords_read;
  fclose(in_file_ptr);
  in_file_ptr = NULL;
  return passwords_read;
} /* read_passwords() */

/*
 * Connects pointers to next array members to members whose code name matches
 * name of next member element.
 */

void connect_members() {
  for (int i = 0; i < MAX_PASSWORDS; i++) {
    char find_member[MAX_NAME_LEN] = "";
    strcpy(find_member, g_password_array[i].next_member);
    for (int j = 0; j < MAX_PASSWORDS; j++) {
      if (strcmp(g_password_array[j].code_name, find_member) == 0) {
        if (i != j) {
          g_password_array[i].next_member_ptr = &g_password_array[j];
        }
        else {
          g_password_array[i].next_member_ptr = NULL;
        }
        break;
      }
    }
  }
} /* connect_members() */

/*
 * Checks if parameter is a spy, and if so, changes members in array
 * accordingly to account for spy data.
 */

int isolate_spy(password_t *potential_spy) {
  assert(potential_spy != NULL);

  int the_soup_check = 0;
  if (strncmp("the", potential_spy->passcode_name, 3) == 0) {
    int last_char = strlen(potential_spy->passcode_name) - 1;
    if ((potential_spy->passcode_name[last_char] == 'p') &&
        (potential_spy->passcode_name[last_char - 1] == 'u') &&
        (potential_spy->passcode_name[last_char - 2] == 'o') &&
        (potential_spy->passcode_name[last_char - 3] == 's')) {
      the_soup_check = 1;
    }
  }

  int records_changed = 0;
  if ((strstr(potential_spy->passcode_value, "rooster") != NULL) ||
      (the_soup_check) || (strlen(potential_spy->passcode_value) <
      strlen(potential_spy->passcode_name))) {
    for (int j = 0; j < MAX_PASSWORDS; j++) {
      if (strcmp(g_password_array[j].next_member, potential_spy->code_name) ==
          0) {
        strcpy(g_password_array[j].next_member, potential_spy->next_member);
      }

      if (g_password_array[j].next_member_ptr == potential_spy) {
        g_password_array[j].next_member_ptr = potential_spy->next_member_ptr;
        if ((g_password_array[j].next_member_ptr != NULL) &&
            (strcmp(g_password_array[j].next_member_ptr->code_name,
            g_password_array[j].code_name) == 0)) {
          g_password_array[j].next_member_ptr = NULL;
        }

        records_changed++;
      }
    }
    potential_spy->next_member_ptr = NULL;
  }
  else {
    return NOT_A_SPY;
  }
  return records_changed;
} /* isolate_spy() */

/*
 * Attempts to reach second parameter from first parameter using pointers
 * to next member. Spy count incremented accordingly. If not possible to send
 * message, appropriate error code returned.
 */

int send_message(password_t *sender, password_t *recipient) {
  assert((sender != NULL) && (recipient != NULL));
  if ((sender == recipient) || (sender->next_member_ptr == sender)) {
    return MESSAGE_NOT_SENT;
  }

  char member_to_find[MAX_NAME_LEN] = "";
  strcpy(member_to_find, sender->code_name);
  int spy_count = 0;
  for (int i = 0; (i < MAX_PASSWORDS) && (i < g_password_count); i++) {
    if (strcmp(g_password_array[i].code_name, member_to_find) == 0) {
      int the_soup_check = 0;
      if (strncmp("the", g_password_array[i].passcode_name, 3) == 0) {
        int last_char = strlen(g_password_array[i].passcode_name) - 1;
        if ((g_password_array[i].passcode_name[last_char] == 'p') &&
            (g_password_array[i].passcode_name[last_char - 1] == 'u') &&
            (g_password_array[i].passcode_name[last_char - 2] == 'o') &&
            (g_password_array[i].passcode_name[last_char - 3] == 's')) {
          the_soup_check = 1;
        }
      }

      if ((strstr(g_password_array[i].passcode_value, "rooster") != NULL) ||
          (the_soup_check) || (strlen(g_password_array[i].passcode_value) <
          strlen(g_password_array[i].passcode_name))) {
        spy_count++;
      }

      if (g_password_array[i].next_member_ptr != NULL) {
        if (strcmp(member_to_find, recipient->code_name) == 0) {
          /* reached the recipient from the sender */

          break;
        }
        else {
          strcpy(member_to_find,
                 g_password_array[i].next_member_ptr->code_name);
          if (strcmp(member_to_find, sender->code_name) == 0) {
            /* possible infinite loop (next member to find is sender) */

            return MESSAGE_NOT_SENT;
          }

          /* resets i to begin searching at beginning of global array */

          i = -1;
        }
      }
      else {
        break;
      }
    }
  }
  return spy_count;
} /* send_message() */
