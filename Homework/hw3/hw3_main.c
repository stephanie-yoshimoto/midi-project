#include <stdio.h>
#include <string.h>
#include "hw3.h"

void print_return_value(int, char *);

/* This function is used to run the different functions implemented in file
 * hw3.c. User input is used to determine which function to run and what input
 * is passed to it. Upon completion, the function returns zero.
 */

int main() {
  char input_file[MAX_FILE_LEN];
  char dealership[MAX_NAME_LEN];
  char employee[MAX_NAME_LEN];
  char output_file[MAX_FILE_LEN];
  int choice = -1;
  int table_index = 0;
  int start_col = 0;
  int end_col = 0;
  int return_value = 0;
  int status = 0;

  while (1) {

    printf("\nOPTIONS:\n"
           "0) Quit\n"
           "1) read_tables()\n"
           "2) show_total_sales()\n"
           "3) show_average_prices()\n"
           "4) calculate_revenue()\n"
           "5) employee_salary()\n"
           "6) calculate_max_salary()\n"
           "7) show_most_common_sale()\n"
           "8) write_tables()\n"
           "Select a function: ");

    status = fscanf(stdin, "%d", &choice);
    /* To prevent infinite loop in case of non integer input. */
    while (getchar() != '\n'){
    }
    if (status <= 0) {
      printf("\nInvalid input! Try again...\n");
      continue;
    }
    if (choice == 0) {
      printf("\nGoodbye!\n\n");
      break;
    }

    switch (choice) {
      case 0:
        printf("\nGoodbye!\n\n");
        break;
      case 1:
        printf("\nEnter input file name: ");
        fscanf(stdin, "%s", input_file);
        return_value = read_tables(input_file);
        print_return_value(return_value, "input_tables");
        break;
      case 2:
        printf("\nEnter output file name: ");
        fscanf(stdin, "%s", output_file);
        return_value = show_total_sales(output_file);
        print_return_value(return_value, "show_total_sales");
        break;
      case 3:
        printf("\nEnter output file name: ");
        fscanf(stdin, "%s", output_file);
        return_value = show_average_prices(output_file);
        print_return_value(return_value, "show_average_prices");
        break;
      case 4:
        printf("\nEnter the name of a dealership: ");
        fscanf(stdin, "\n%[^\n]", dealership);
        return_value = calculate_revenue(dealership);
        print_return_value(return_value, "calculate_revenue");
        break;
      case 5:
        printf("\nEnter the name of a salesperson: ");
        //fflush(stdout);
        fscanf(stdin, "\n%[^\n]", employee);
        return_value = employee_salary(employee);
        print_return_value(return_value, "employee_salary");
        break;
      case 6:
        return_value = calculate_max_salary();
        print_return_value(return_value, "calculate_max_salary");
        break;
      case 7:
        printf("\nEnter output file name: ");
        fscanf(stdin, "%s", output_file);
        return_value = show_most_common_sale(output_file);
        print_return_value(return_value, "show_most_common_sale");
        break;
      case 8:
        printf("\nEnter output file name: ");
        fscanf(stdin, "%s", output_file);
        printf("Enter table index: ");
        fscanf(stdin, "%d", &table_index);
        printf("Enter start column: ");
        fscanf(stdin, "%d", &start_col);
        printf("Enter end column: ");
        fscanf(stdin, "%d", &end_col);
        return_value = write_tables(output_file, table_index,
                                     start_col, end_col);
        print_return_value(return_value, "write_tables");
        break;
      default:
        printf("\nInvalid selection.\n");
        break;
    }
  }

  return 0;
} /* main() */

/*
 * Function to print appropriate error codes.
 */

void print_return_value(int val, char *function) {
  printf("Value returned by %s(): %d ", function, val);
  switch(val) {
    case OK:
      printf("(OK)\n");
      break;
    case NO_SUCH_NAME:
      printf("(NO_SUCH_NAME)\n");
      break;
    case FILE_READ_ERROR:
      printf("(FILE_READ_ERROR)\n");
      break;
    case FILE_WRITE_ERROR:
      printf("(FILE_WRITE_ERROR)\n");
      break;
    case RECORD_ERROR:
      printf("(RECORD_ERROR)\n");
      break;
    case NO_DATA_POINTS:
      printf("(NO_DATA_POINTS)\n");
      printf("ERROR: Make sure input file is non empty and "
             "run input_tables() first.\n");
      break;
    case OUT_OF_BOUNDS:
      printf("(OUT_OF_BOUNDS)\n");
      break;
    case INVALID_TABLE_INDEX:
      printf("(INVALID_TABLE_INDEX)\n");
      break;
    case INVALID_COLUMN:
      printf("(INVALID_COLUMN)\n");
      break;
    default:
      printf("\n");
      break;
  }
} /* print_return_value() */
