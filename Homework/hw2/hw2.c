/* Stephanie Yoshimoto, hw2.c, CS 24000, Spring 2020
 * Last updated January 22, 2020
 */

#include "hw2.h"

#include <stdio.h>

/*
 * This function prints the dashes for an individual to an output file.
 */

void print_dashes(FILE *output_pointer, float sleep_hours) {
  if (sleep_hours == 0) {
    /* prints new line, moves onto next date */

    fprintf(output_pointer, "\n");
  }
  else if (sleep_hours > 10) {
    /* prints string of dashes without closing pipe if hours of sleep > 10 */

    fprintf(output_pointer, "--------------------\n");
  }
  else {
    while (sleep_hours > 0.5) {
      /* prints dashes one at a time, then prints closing pipe */

      fprintf(output_pointer, "-");
      sleep_hours -= 0.5;
    }
    fprintf(output_pointer, "|\n");
  }
} /* print_dashes() */

/*
 * This function takes in the name of a file and opens it to collect the data
 * for the individuals average sleep hours.
 */

float get_average_sleep_hours(char *file_name, int year, int month) {
  if ((month < 1) || (month > 12)) {
    return BAD_DATE;
  }

  FILE *file_pointer = NULL;
  file_pointer = fopen(file_name, "r");
  if (file_pointer == NULL) {
    return FILE_READ_ERR;
  }

  char name[MAX_NAME_LEN];
  int returned_value = fscanf(file_pointer, "%39[^\n]s", name);
  int day = 0;
  int desired_year = year;
  int desired_month = month;
  float sleep_hours = 0.0;
  float total_hours = 0.0;
  int entries = 0;
  while ((returned_value = fscanf(file_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &desired_month, &day, &desired_year, &sleep_hours)) == 4) {
    if ((day < 1) || (day > 31)) {
      fclose(file_pointer);
      file_pointer = NULL;
      return BAD_DATE;
    }

    /* if year and month match, adds to total sleep hours and amount of days */

    if ((desired_year == year) && (desired_month == month)) {
      total_hours += sleep_hours;
      entries++;
    }
  }

  fclose(file_pointer);
  file_pointer = NULL;
  if (returned_value != EOF) {
    return BAD_RECORD;
  }
  else if (entries == 0) {
    return NO_DATA_POINTS;
  }
  return (total_hours / entries);
} /* get_average_sleep_hours() */

/*
 * This function reads an input file and outputs the individual's sleep data
 * per night in an output file, also recording the average hours of sleep for
 * the most recent month.
 */

int get_sleep_log(char *in_file, char *out_file) {
  FILE *input_pointer = NULL;
  FILE *output_pointer = NULL;
  input_pointer = fopen(in_file, "r");
  output_pointer = fopen(out_file, "w");
  if (input_pointer == NULL) {
    fclose(output_pointer);
    output_pointer = NULL;
    return FILE_READ_ERR;
  }
  else if (output_pointer == NULL) {
    fclose(input_pointer);
    input_pointer = NULL;
    return FILE_READ_ERR;
  }

  char name[MAX_NAME_LEN];
  int returned_value = fscanf(input_pointer, "%39[^\n]s", name);
  if ((returned_value == 0) || (returned_value == -1)) {
    fclose(input_pointer);
    fclose(output_pointer);
    input_pointer = NULL;
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }

  int year = 0;
  int month = 0;
  int temp_year = 0;
  int temp_month = 0;
  int day = 0;
  float sleep_hours = 0.0;
  float total_hours = 0.0;
  while ((returned_value = fscanf(input_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month, &day, &temp_year, &sleep_hours)) == 4) {
    /* finds the most recent month and year */

    if (temp_year >= year) {
      if (temp_year > year) {
        month = temp_month;
      }
      else {
        if (temp_month > month) {
          month = temp_month;
        }
      }
      year = temp_year;
    }
  }

  if ((month < 1) || (month > 12)) {
    fclose(input_pointer);
    input_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return BAD_DATE;
  }

  /* outputs name/month/year data collected to output file */

  fprintf(output_pointer, "Name: %s, Month: %d, Year: %d\n", name, month,
    year);
  fprintf(output_pointer, "HOUR: 0 1 2 3 4 5 6 7 8 9 10\n");

  /* resets the input file's pointer to read data from beginning */

  fclose(input_pointer);
  input_pointer = NULL;
  input_pointer = fopen(in_file, "r");
  if (input_pointer == NULL) {
    fclose(output_pointer);
    output_pointer = NULL;
    return FILE_READ_ERR;
  }
  fscanf(input_pointer, "%*39[^\n]s");

  float entries = 0.0;
  while ((returned_value = fscanf(input_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month, &day, &temp_year, &sleep_hours)) == 4) {
    if ((temp_month == month) && (temp_year == year)) {
      if ((day < 1) || (day > 31)) {
        fclose(input_pointer);
        input_pointer = NULL;
        fclose(output_pointer);
        output_pointer = NULL;
        return BAD_DATE;
      }

      /* prints the initial pipe char and day number, collects sleep hours */
      /* data to be averaged later */

      fprintf(output_pointer, "%-6d|", day);
      total_hours += sleep_hours;
      entries++;
      print_dashes(output_pointer, sleep_hours);
    }
  }

  fclose(input_pointer);
  input_pointer = NULL;
  if (returned_value != EOF) {
    fclose(output_pointer);
    output_pointer = NULL;
    return BAD_RECORD;
  }
  else if (entries == 0) {
    /* executed if no data points found in file */

    fclose(output_pointer);
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }
  fprintf(output_pointer, "Average Sleep Hours: %.2f hours", (total_hours /
    entries));
  fclose(output_pointer);
  output_pointer = NULL;
  return OK;
} /* get_sleep_log() */

/*
 * This function compares the hours of sleep within two input files, recording
 * the compared data in an output file.
 */

int compare_sleep_hours(char *in_file_1, char *in_file_2, char *out_file) {
  FILE *in_1_pointer = NULL;
  FILE *in_2_pointer = NULL;
  FILE *output_pointer = NULL;
  in_1_pointer = fopen(in_file_1, "r");
  in_2_pointer = fopen(in_file_2, "r");
  output_pointer = fopen(out_file, "w");
  if (in_1_pointer == NULL) {
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return FILE_READ_ERR;
  }
  else if (in_2_pointer == NULL) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    in_2_pointer = NULL;
    fclose(output_pointer);
    return FILE_READ_ERR;
  }
  else if (output_pointer == NULL) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    return FILE_WRITE_ERR;
  }

  char name1[MAX_NAME_LEN];
  char name2[MAX_NAME_LEN];
  int returned_value_1 = fscanf(in_1_pointer, "%39[^\n]s", name1);
  int returned_value_2 = fscanf(in_2_pointer, "%39[^\n]s", name2);
  if ((returned_value_1 == EOF) || (returned_value_2 == EOF)) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }

  /* finds most recent month and year */

  int year = 0;
  int month = 0;
  int temp_year_1 = 0;
  int day = 0;
  int temp_month_1 = 0;
  float sleep_hours_1 = 0.0;
  while ((returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month_1, &day, &temp_year_1, &sleep_hours_1)) == 4) {
    if (temp_year_1 >= year) {
      if (temp_year_1 > year) {
        month = temp_month_1;
      }
      else {
        if (temp_month_1 > month) {
          month = temp_month_1;
        }
      }
      year = temp_year_1;
    }
  }

  /* outputs names/month/year data collected to output file */

  fprintf(output_pointer, "Name: %s\nName: %s\nMonth: %d, Year: %d\n", name1,
    name2, month, year);
  fprintf(output_pointer, "HOUR: 0 1 2 3 4 5 6 7 8 9 10\n");

  /* resets input file 1's pointer to read data from beginning */

  fclose(in_1_pointer);
  in_1_pointer = fopen(in_file_1, "r");
  fscanf(in_1_pointer, "%*39[^\n]s");
  returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month_1, &day, &temp_year_1, &sleep_hours_1);

  int temp_year_2 = 0;
  int temp_month_2 = 0;
  float sleep_hours_2 = 0.0;
  returned_value_2 = fscanf(in_2_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month_2, &day, &temp_year_2, &sleep_hours_2);
  if ((returned_value_1 == EOF) && (returned_value_2 == EOF)) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }

  float entries_1 = 0.0;
  float entries_2 = 0.0;
  float total_hours_1 = 0.0;
  float total_hours_2 = 0.0;
  while ((returned_value_1 == 4) && (returned_value_2 == 4)) {
    if ((temp_month_1 == month) && (temp_year_1 == year) &&
          (temp_month_1 == temp_month_2) && (temp_year_1 == temp_year_2)) {
      if (returned_value_1 == 4) {
        /* prints the initial pipe char and day number, collects sleep hours */
        /* data to be averaged later */

        fprintf(output_pointer, "%-6d|", day);
        total_hours_1 += sleep_hours_1;
        entries_1++;
        print_dashes(output_pointer, sleep_hours_1);
      }
      else {
        /* executed if no data for first file for certain day */

        fprintf(output_pointer, "%-6d|\n", day);
      }

      /* evaluates sleep hours for second file */

      if (returned_value_2 == 4) {
        fprintf(output_pointer, "      |");
        total_hours_2 += sleep_hours_2;
        entries_2++;
        print_dashes(output_pointer, sleep_hours_2);
      }
      else {
        /* executed if no data for individual in second file */

        fprintf(output_pointer, "\n");
      }
    }

    /* collects data from next lines in each file */

    returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month_1, &day, &temp_year_1, &sleep_hours_1);
    returned_value_2 = fscanf(in_2_pointer, "%d/%d/%d|%f|%*f|%*f\n",
    &temp_month_2, &day, &temp_year_2, &sleep_hours_2);
  }

  fclose(in_1_pointer);
  in_1_pointer = NULL;
  fclose(in_2_pointer);
  in_2_pointer = NULL;
  if ((returned_value_1 == EOF) && (returned_value_2 == EOF)) {
    /* prints average sleep hours to output file (input matched, no errors) */

    fprintf(output_pointer, "Average Sleep Hours of %s: %.2f hours\n", name1,
    (total_hours_1 / entries_1));
    fprintf(output_pointer, "Average Sleep Hours of %s: %.2f hours", name2,
    (total_hours_2 / entries_2));
    fclose(output_pointer);
    output_pointer = NULL;
    return OK;
  }
  else if ((returned_value_1 != EOF) ^ (returned_value_2 != EOF)) {
    /* one file is longer than the other */

    fclose(output_pointer);
    output_pointer = NULL;
    return RECORDS_MISMATCH;
  }

  fclose(output_pointer);
  output_pointer = NULL;
  return BAD_RECORD;
} /* compare_sleep_hours() */

/*
 * This function calculates the average calories burned in the given year and
 * monnth passed in, found in the input file.
 */

float get_average_calories(char *file_name, int year, int month) {
  if ((month < 1) || (month > 12)) {
    return BAD_DATE;
  }

  FILE *file_pointer = NULL;
  file_pointer = fopen(file_name, "r");
  if (file_pointer == NULL) {
    return FILE_READ_ERR;
  }

  char name[MAX_NAME_LEN];
  int day = 0;
  int temp_month = 0;
  int temp_year = 0;
  int temp_moving_mins = 0;
  int temp_workout_mins = 0;
  float total_calories_burned = 0.0;
  int entries = 0;
  int returned_value = fscanf(file_pointer, "%39[^\n]s", name);
  while ((returned_value = fscanf(file_pointer, "%d/%d/%d|%*f|%d|%d\n",
    &temp_month, &day, &temp_year, &temp_moving_mins,
    &temp_workout_mins)) == 5) {
    if ((day < 1) || (day > 31) || (temp_month < 1) || (temp_month > 12)) {
      fclose(file_pointer);
      file_pointer = NULL;
      return BAD_DATE;
    }

    /* if year and month match, adds to total calories and amount of days */

    if ((temp_month == month) && (temp_year == year)) {
      total_calories_burned += (2.5 * temp_moving_mins) +
        (11.3 * temp_workout_mins);
      entries++;
    }
  }

  fclose(file_pointer);
  file_pointer = NULL;
  if (returned_value != EOF) {
    return BAD_RECORD;
  }

  if (entries == 0) {
    /* no entries if month is not in file or if no data points in file */

    return NO_DATA_POINTS;
  }
  return (total_calories_burned / entries);
} /* get_average_calories() */

/*
 * This function compares the average calories burned for two individuals'
 * data stored in different input files, then prints the compared results to
 * an output file for specified month and year.
 */

int compare_activity_log(char *in_file_1, char *in_file_2, int year, int month,
                         char *out_file) {
  if ((month < 1) || (month > 12)) {
    return BAD_DATE;
  }
  FILE *in_1_pointer = NULL;
  FILE *in_2_pointer = NULL;
  FILE *output_pointer = NULL;
  in_1_pointer = fopen(in_file_1, "r");
  in_2_pointer = fopen(in_file_2, "r");
  output_pointer = fopen(out_file, "w");
  if (in_1_pointer == NULL) {
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return FILE_READ_ERR;
  }
  else if (in_2_pointer == NULL) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return FILE_READ_ERR;
  }
  else if (output_pointer == NULL) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    return FILE_WRITE_ERR;
  }

  char name1[MAX_NAME_LEN];
  char name2[MAX_NAME_LEN];
  int temp_year_1 = 0;
  int temp_year_2 = 0;
  int temp_month_1 = 0;
  int temp_month_2 = 0;
  int day = 0;
  float moving_mins_1 = 0.0;
  float moving_mins_2 = 0.0;
  float workout_mins_1 = 0.0;
  float workout_mins_2 = 0.0;
  fscanf(in_1_pointer, "%39[^\n]s", name1);
  fscanf(in_2_pointer, "%39[^\n]s", name2);
  fprintf(output_pointer, "Name: %s\nName: %s\nMonth: %d, Year: %d\n", name1,
    name2, month, year);
  int returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%*f|%f|%f\n",
    &temp_month_1, &day, &temp_year_1, &moving_mins_1, &workout_mins_1);
  int returned_value_2 = fscanf(in_2_pointer, "%d/%d/%d|%*f|%f|%f\n",
    &temp_month_2, &day, &temp_year_2, &moving_mins_2, &workout_mins_2);
  if ((returned_value_1 == EOF) && (returned_value_2 == EOF)) {
    fclose(in_1_pointer);
    in_1_pointer = NULL;
    fclose(in_2_pointer);
    in_2_pointer = NULL;
    fclose(output_pointer);
    output_pointer = NULL;
    return NO_DATA_POINTS;
  }
  while ((returned_value_1 == 5) && (returned_value_2 == 5)) {
    if ((temp_month_1 == month) && (temp_month_2 == month) &&
      (temp_year_1 == year) && (temp_year_2 == year)) {
      if ((day < 1) || (day > 31)) {
        fclose(in_1_pointer);
        in_1_pointer = NULL;
        fclose(in_2_pointer);
        in_2_pointer = NULL;
        fclose(output_pointer);
        output_pointer = NULL;
        return BAD_DATE;
      }

      /* prints formatting for file, calculates calories per person per day */

      fprintf(output_pointer, "%-2d: ", day);
      float calories_burned_1 = (2.5 * moving_mins_1) + (11.3 * workout_mins_1);
      float calories_burned_2 = (2.5 * moving_mins_2) + (11.3 * workout_mins_2);

      /* compares calories to see which person burned more, outputs to file */

      if (calories_burned_1 == calories_burned_2) {
        fprintf(output_pointer,
        "%s and %s burned an equal amount of calories.\n", name1, name2);
      }
      else if (calories_burned_1 > calories_burned_2) {
        fprintf(output_pointer, "%s burned %.2f more calories.\n", name1,
          (calories_burned_1 - calories_burned_2));
      }
      else if (calories_burned_1 < calories_burned_2) {
        fprintf(output_pointer, "%s burned %.2f more calories.\n", name2,
          (calories_burned_2 - calories_burned_1));
      }
    }

    /* collects data from next lines in each file */

    returned_value_1 = fscanf(in_1_pointer, "%d/%d/%d|%*f|%f|%f\n",
    &temp_month_1, &day, &temp_year_1, &moving_mins_1, &workout_mins_1);
    returned_value_2 = fscanf(in_2_pointer, "%d/%d/%d|%*f|%f|%f\n",
    &temp_month_2, &day, &temp_year_2, &moving_mins_2, &workout_mins_2);
  }

  fclose(in_1_pointer);
  in_1_pointer = NULL;
  fclose(in_2_pointer);
  in_2_pointer = NULL;
  fclose(output_pointer);
  output_pointer = NULL;
  if ((returned_value_1 == EOF) && (returned_value_2 == EOF)) {
    /* input matched, no errors */

    return OK;
  }
  else if ((returned_value_1 != EOF) ^ (returned_value_2 != EOF)) {
    /* one file is longer than the other */

    return RECORDS_MISMATCH;
  }
  return BAD_RECORD;
} /* compare_activity_log() */
