#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw2.h"

/*  This function prints out all of the characters in the file named
 *  filename. It returns FILE_READ_ERR if it cannot read the specified
 *  file. Otherwise, it returns OK to indicate that the funciton was
 *  successful.
 */

int print_out(char *filename) {
  FILE *file_ptr = NULL;
  file_ptr = fopen(filename, "r");
  if (file_ptr == NULL) {
    return (FILE_READ_ERR);
  }

  int character = '\0';
  while ((character = fgetc(file_ptr)) != EOF) {
    putchar(character);
  }

  fclose(file_ptr);
  file_ptr = NULL;
  return (OK);
} /* print_out() */

/*  This function is used to run the functions implemented in hw2.c.
 *  User input is used to determine which function to run what input
 *  is passed to it. Upon completion, the function returns zero.
 */

int main(int argc, char **argv) {
  while (1) {
    printf("\n\n\n\n");
    printf("Test Menu\n");
    printf("1. Get average sleep hours\n");
    printf("2. Create Sleep Log\n");
    printf("3. Create a combined Sleep Log for 2 people\n");
    printf("4. Get Average Calories\n");
    printf("5. Create a log comparing calories for 2 people\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");

    int response = 0;
    scanf("%d", &response);
    getchar();

    int month = 0;
    int year = 0;
    float avg = 0;
    int err_code = 0;
    char input_filename[MAX_FILE_LEN] = "";
    char input_filename1[MAX_FILE_LEN] = "";
    char input_filename2[MAX_FILE_LEN] = "";
    char result_filename[MAX_FILE_LEN] = "";
    switch (response) {
      case 1:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
        printf("Enter year: ");
        scanf("%d", &year);
        printf("Enter month as a number (ex. January -> 1): ");
        scanf("%d", &month);
        avg = get_average_sleep_hours(input_filename, year, month);
        if (avg >= 0) {
          printf("Average Sleep in %s for month %d and year %d is %f\n",
              input_filename, month, year, avg);
        } else {
          printf("Error! (code %d)\n", (int)avg);
        }
        break;

      case 2:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
        printf("Enter output file name: ");
        scanf("%s", result_filename);
        err_code = get_sleep_log(input_filename, result_filename);
        if (err_code >= 0) {
          printf("Sleep log of %s created in %s\n",
              input_filename, result_filename);
        } else {
          printf("Error! (code %d)\n", err_code);
        }
        break;

      case 3:
        printf("Enter 1st input file name: ");
        scanf("%s", input_filename1);
        printf("Enter 2nd input file name: ");
        scanf("%s", input_filename2);
        printf("Enter output file name: ");
        scanf("%s", result_filename);
        err_code = compare_sleep_hours(input_filename1, input_filename2,
                                      result_filename);
        if (err_code >= 0) {
          printf("Combined sleep log of %s and %s created in %s\n",
              input_filename1, input_filename2, result_filename);
        } else {
          printf("Error! (code %d)\n", err_code);
        }
        break;

      case 4:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
        printf("Enter year: ");
        scanf("%d", &year);
        printf("Enter month as a number (ex. January -> 1): ");
        scanf("%d", &month);
        avg = get_average_calories(input_filename, year, month);
        if (avg >= 0) {
          printf("Average calories in %s for month %d and year %d is %f\n",
              input_filename, month, year, avg);
        } else {
          printf("Error! (code %d)\n", (int)avg);
        }
        break;

      case 5:
        printf("Enter 1st input file name: ");
        scanf("%s", input_filename1);
        printf("Enter 2nd input file name: ");
        scanf("%s", input_filename2);
        printf("Enter year: ");
        scanf("%d", &year);
        printf("Enter month as a number (ex. January -> 1): ");
        scanf("%d", &month);
        printf("Enter output file name: ");
        scanf("%s", result_filename);
        err_code = compare_activity_log(input_filename1, input_filename2, year,
                                       month, result_filename);
        if (err_code >= 0) {
          printf("Combined activity log of %s and %s created in %s\n",
              input_filename1, input_filename2, result_filename);
        } else {
          printf("Error! (code %d)\n", err_code);
        }
        break;

      case 6:
        printf("GooD bYe :^) \n\n");
        return (0);

      default:
        printf("Invalid choice!\n");
        break;
    }
  }
  return (0);
} /* main() */
