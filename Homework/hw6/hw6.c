/* Stephanie Yoshimoto, hw6.c, CS 24000, Spring 2020
 * Last updated February 10, 2020
 */

#include "hw6.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int g_password_count = 0;
password_t g_password_array[MAX_PASSWORDS];

int read_passwords(char *filename) {
  assert(filename != NULL);
  FILE *in_file_ptr = NULL;
  in_file_ptr = fopen(filename, "r");
  if (in_file_ptr == NULL) {
    return NON_READABLE_FILE;
  }

  int passwords_read = 0;
  char temp_code_name[MAX_NAME_LEN] = "";
  char temp_passcode_name[MAX_NAME_LEN] = "";
  char temp_passcode_val[MAX_NAME_LEN] = "";
  char buffer_1[MAX_BUFF_LEN] = "";
  char buffer_2[MAX_BUFF_LEN] = "";
  char buffer_3[MAX_BUFF_LEN] = "";
  password_t temp_password = {"", "", ""};
  int returned_value = 0;
  while (feof(in_file_ptr) == 0) {
    returned_value = fscanf(in_file_ptr, "%49[^&\n]&%49[^&\n]&%49[^\n]\n",
                            buffer_1, buffer_2, buffer_3);
    if (returned_value != 3) {
      if (feof(in_file_ptr) == 0) {
        fclose(in_file_ptr);
        in_file_ptr = NULL;
        return BAD_RECORD;
      }
      break;
    }

    if ((strlen(buffer_1) > MAX_BUFF_LEN) || (strlen(buffer_2) >
        MAX_BUFF_LEN) || (strlen(buffer_3) > MAX_BUFF_LEN)) {
      fclose(in_file_ptr);
      in_file_ptr = NULL;
      return BAD_RECORD;
    }

    if (passwords_read == MAX_PASSWORDS) {
      fclose(in_file_ptr);
      in_file_ptr = NULL;
      return TOO_MUCH_DATA;
    }

    strncpy(temp_code_name, buffer_1, MAX_NAME_LEN - 1);
    temp_code_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(temp_passcode_name, buffer_2, MAX_NAME_LEN - 1);
    temp_passcode_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(temp_passcode_val, buffer_3, MAX_NAME_LEN - 1);
    temp_passcode_val[MAX_NAME_LEN - 1] = '\0';

    strcpy(temp_password.code_name, temp_code_name);
    strcpy(temp_password.passcode_name, temp_passcode_name);
    strcpy(temp_password.passcode_value, temp_passcode_val);
    g_password_array[passwords_read] = temp_password;
    passwords_read++;
  }
  g_password_count = passwords_read;
  fclose(in_file_ptr);
  in_file_ptr = NULL;
  return passwords_read;
} /* read_passwords() */

int locate_spies(char **spies, int strings_in_array) {
  assert((spies != NULL) && (strings_in_array > 0));
  for (int i = 0; i < strings_in_array; i++) {
    if (spies[i] == NULL) {
      assert(0);
    }
  }

  int spy_count = 0;
  for (int i = 0; i < MAX_PASSWORDS; i++) {
    for (int j = 0; j < strings_in_array; j++) {
      if (strcmp(spies[j], g_password_array[i].code_name) == 0) {
        if ((strstr(g_password_array[i].passcode_value, "rooster") != NULL) ||
            ((strncmp("the", g_password_array[i].passcode_name, 3) == 0) &&
            (strstr(g_password_array[i].passcode_name, "soup") != NULL)) ||
            (strlen(g_password_array[i].passcode_value) <
            (strlen(g_password_array[i].passcode_name)))) {
          spy_count++;
          break;
        }
      }
    }
  }
  return spy_count;
} /* locate_spies() */
