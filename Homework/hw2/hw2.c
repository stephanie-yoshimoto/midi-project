/* Stephanie Yoshimoto, hw2.c, CS 24000, Spring 2020
 * Last updated January 22, 2020
 */

/* Add any includes here */
#include <stdio.h>

#include "hw2.h"

float get_average_sleep_hours(char *file_name, int year, int month) {
  char name[MAX_NAME_LEN];
  int day = 0;
  int desired_year = year;
  int desired_month = month;
  float sleep_hours = 0;
  float moving_minutes = 0;
  float workout_minutes = 0;
  float total_hours = 0;
  int entries = 0;
  int returned_value = 1;
  FILE *file_pointer = fopen(file_name, "r");
  if (file_pointer == NULL) {
    fclose(file_pointer);
    return FILE_READ_ERR;
  }

  returned_value = fscanf(file_pointer, "%[^\n]", name);
  if (returned_value == 0 || returned_value == -1) {
    fclose(file_pointer);
    return NO_DATA_POINTS;
  }

  while ((returned_value = fscanf(file_pointer, "%d/%d/%d|%f|%f|%f\n",
    &desired_month, &day, &desired_year, &sleep_hours, &moving_minutes,
    &workout_minutes)) == 6) {
    if (desired_year == year && desired_month == month) {
      total_hours += sleep_hours;
      entries++;
    }
  }

  if (returned_value != 0 && returned_value != -1) {
    fclose(file_pointer);
    return BAD_RECORD;
  }

  fclose(file_pointer);
  return (total_hours / entries);
}

int get_sleep_log(char *in_file, char *out_file) {
  char name[MAX_NAME_LEN];
  int year = 0;
  int temp_year = 0;
  int month = 0;
  int temp_month = 0;
  int day = 0;
  float sleep_hours = 0;
  float moving_minutes = 0;
  float workout_minutes = 0;
  float total_hours = 0;
  float entries = 0;
  int returned_value = 1;
  FILE *input_pointer = fopen(in_file, "r");
  FILE *output_pointer = fopen(out_file, "w");
  if (input_pointer == NULL || output_pointer == NULL) {
    fclose(input_pointer);
    fclose(output_pointer);
    return FILE_READ_ERR;
  }

  returned_value = fscanf(input_pointer, "%[^\n]", name);
  if (returned_value == 0 || returned_value == -1) {
    fclose(input_pointer);
    fclose(output_pointer);
    return NO_DATA_POINTS;
  }

  while ((returned_value = fscanf(input_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month, &day, &temp_year, &sleep_hours, &moving_minutes,
    &workout_minutes)) == 6) {
    if (temp_year >= year) {
      year = temp_year;
      if (temp_month >= month) {
        month = temp_month;
      }
    }
  }

  fprintf(output_pointer, "Name: %s, Month: %d, Year: %d\n", name, month,
    year);
  fprintf(output_pointer, "HOUR: 0 1 2 3 4 5 6 7 8 9 10\n");
//  fprintf(output_pointer, "%-6d|\n", day);
  return OK;
}

int compare_sleep_hours(char *in_file_1, char *in_file_2, char *out_file) {
  return 0;
}

float get_average_calories(char *file_name, int year, int month) {
  return 0.0f;
}

int compare_activity_log(char *in_file_1, char *in_file_2, int year, int month, char *out_file) {
  return 0;
}


/* Remember, you don't need a main function!
 * it is provided by hw2_main.c or hw2_test.o
 */
