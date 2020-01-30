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
    file_pointer = NULL;
    return FILE_READ_ERR;
  }

  returned_value = fscanf(file_pointer, "%[^\n]", name);
  if (returned_value == 0 || returned_value == -1) {
    fclose(file_pointer);
    file_pointer = NULL;
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
    file_pointer = NULL;
    return BAD_RECORD;
  }

  fclose(file_pointer);
  file_pointer = NULL;
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
    input_pointer = NULL;
    output_pointer = NULL;
    return FILE_READ_ERR;
  }

  returned_value = fscanf(input_pointer, "%[^\n]", name);
  if (returned_value == 0 || returned_value == -1) {
    fclose(input_pointer);
    fclose(output_pointer);
    input_pointer = NULL;
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
  fclose(input_pointer);
  input_pointer = fopen(in_file, "r");
  returned_value = fscanf(input_pointer, "%[^\n]", name);
  if (returned_value == 0 || returned_value == -1) {
    fclose(input_pointer);
    fclose(output_pointer);
    return NO_DATA_POINTS;
  }

  while ((returned_value = fscanf(input_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month, &day, &temp_year, &sleep_hours, &moving_minutes,
    &workout_minutes)) == 6) {
    if (temp_month == month && temp_year == year) {
      fprintf(output_pointer, "%-6d|", day);
      total_hours += sleep_hours;
      entries++;
      if (sleep_hours == 0) {
        fprintf(output_pointer, "\n");
        continue;
      }
      else if (sleep_hours > 10) {
        fprintf(output_pointer, "--------------------\n");
        continue;
      }
      while (sleep_hours > 0.5) {
        fprintf(output_pointer, "-");
        sleep_hours -= 0.5;
      }
      fprintf(output_pointer, "|\n");
    }
  }

  if (returned_value != 0 && returned_value != -1) {
    fclose(input_pointer);
    input_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return BAD_RECORD;
  }

  fprintf(output_pointer, "Average Sleep Hours: %.2f hours", (total_hours /
    entries));
  fclose(input_pointer);
  input_pointer = NULL;
  fclose(output_pointer);
  output_pointer = NULL;
  return 0;
}

int compare_sleep_hours(char *in_file_1, char *in_file_2, char *out_file) {
  char name1[MAX_NAME_LEN];
  char name2[MAX_NAME_LEN];
  int year = 0;
  int temp_year_1 = 0;
  int temp_year_2 = 0;
  int month = 0;
  int temp_month_1 = 0;
  int temp_month_2 = 0;
  int day = 0;
  float sleep_hours_1 = 0;
  float sleep_hours_2 = 0;
  float moving_minutes = 0;
  float workout_minutes = 0;
  float total_hours_1 = 0;
  float total_hours_2 = 0;
  float entries_1 = 0;
  float entries_2 = 0;
  int returned_value_1 = 1;
  int returned_value_2 = 1;
  FILE *in_1_pointer = fopen(in_file_1, "r");
  FILE *in_2_pointer = fopen(in_file_2, "r");
  FILE *output_pointer = fopen(out_file, "w");
  if ((in_1_pointer == NULL) || (in_2_pointer == NULL) || 
    (output_pointer == NULL)) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return FILE_READ_ERR;
  }

  returned_value_1 = fscanf(in_1_pointer, "%[^\n]", name1);
  returned_value_2 = fscanf(in_2_pointer, "%[^\n]", name2);
  if ((returned_value_1 == 0) || (returned_value_1 == -1) || 
    (returned_value_2 == 0) || (returned_value_2 == 0)) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }

  while ((returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month_1, &day, &temp_year_1, &sleep_hours_1, &moving_minutes,
    &workout_minutes)) == 6) {
    if (temp_year_1 >= year) {
      year = temp_year_1;
      if (temp_month_1 >= month) {
        month = temp_month_1;
      }
    }
  }

  fprintf(output_pointer, "Name: %s\nName: %s\nMonth: %d, Year: %d\n", name1,
    name2, month, year);
  fprintf(output_pointer, "HOUR: 0 1 2 3 4 5 6 7 8 9 10\n");
  fclose(in_1_pointer);
  in_1_pointer = fopen(in_file_1, "r");
  returned_value_1 = fscanf(in_1_pointer, "%[^\n]", name1);
  if ((returned_value_1 == 0) || (returned_value_1 == -1)) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }

  returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month_1, &day, &temp_year_1, &sleep_hours_1, &moving_minutes,
    &workout_minutes);
  returned_value_2 = fscanf(in_2_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month_2, &day, &temp_year_2, &sleep_hours_2, &moving_minutes,
    &workout_minutes);
  while ((returned_value_1 == 6) || (returned_value_2 == 6)) {
    if ((temp_month_1 == month) && (temp_year_1 == year) &&
          (temp_month_1 == temp_month_2) && (temp_year_1 == temp_year_2)) {
      if (returned_value_1 == 6) {
        fprintf(output_pointer, "%-6d|", day);
        total_hours_1 += sleep_hours_1;
        entries_1++;
        if (sleep_hours_1 == 0) {
          fprintf(output_pointer, "\n");
         }
        else if (sleep_hours_1 > 10) {
          fprintf(output_pointer, "--------------------\n");
        }
        else {
          while (sleep_hours_1 > 0.5) {
            fprintf(output_pointer, "-");
            sleep_hours_1 -= 0.5;
          }
          fprintf(output_pointer, "|\n");
        }
      }
      else {
        fprintf(output_pointer, "%-6d|\n", day);
      }

      if (returned_value_2 == 6) {
        fprintf(output_pointer, "      |");
        total_hours_2 += sleep_hours_2;
        entries_2++;
        if (sleep_hours_2 == 0) {
          fprintf(output_pointer, "\n");
        }
        else if (sleep_hours_2 > 10) {
          fprintf(output_pointer, "--------------------\n");
        }
        else {
          while (sleep_hours_2 > 0.5) {
            fprintf(output_pointer, "-");
            sleep_hours_2 -= 0.5;
          }
          fprintf(output_pointer, "|\n");
        }
      }
      else {
        fprintf(output_pointer, "\n");
      }
    }
    returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month_1, &day, &temp_year_1, &sleep_hours_1, &moving_minutes,
    &workout_minutes);
    returned_value_2 = fscanf(in_2_pointer, "%d/%d/%d|%f|%f|%f\n",
    &temp_month_2, &day, &temp_year_2, &sleep_hours_2, &moving_minutes,
    &workout_minutes);
  }

  if (((returned_value_1 != 0) && (returned_value_1 != -1)) ||
    ((returned_value_2 != 0) && (returned_value_2 != -1))) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return BAD_RECORD;
  }

  fprintf(output_pointer, "Average Sleep Hours of %s: %.2f hours\n", name1,
    (total_hours_1 / entries_1));
  fprintf(output_pointer, "Average Sleep Hours of %s: %.2f hours", name2,
    (total_hours_2 / entries_2));
  fclose(in_1_pointer);
  in_1_pointer = NULL;
  fclose(in_2_pointer);
  in_2_pointer = NULL;
  fclose(output_pointer);
  output_pointer = NULL;
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
