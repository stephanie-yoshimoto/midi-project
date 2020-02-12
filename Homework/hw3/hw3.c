/* Stephanie Yoshimoto, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

#include "hw3.h"

#include <stdio.h>
#include <string.h>

#define DEALERSHIPS (1)
#define SALESPEOPLE (2)
#define PRICES (3)
#define SALES (4)

int g_dealership_count = 0;
char g_dealerships[MAX_RECORDS][MAX_NAME_LEN];
char g_salespeople[MAX_RECORDS][MAX_NAME_LEN];
int g_prices[MAX_RECORDS][NUM_PRICE_COLS];
int g_sales[MAX_RECORDS][NUM_SALES_COLS];

/*
 * This function clears the global arrays if the input file passed in to
 * read_tables() contains invalid data.
 */

void clear_records() {
  for (int i = 0; i < MAX_RECORDS; i++) {
    if (g_dealerships[i][0] != '\0') {
      for (int j = 0; j < MAX_NAME_LEN; j++) {
        g_dealerships[i][j] = '\0';
      }
    }

    if (g_salespeople[i][0] != '\0') {
      for (int j = 0; j < MAX_NAME_LEN; j++) {
        g_salespeople[i][j] = '\0';
      }
    }

    if ((g_prices[i][0] != 0) || (g_prices[i][1] != 0) ||
        (g_prices[i][2] != 0) || (g_prices[i][3] != 0)) {
      g_prices[i][0] = 0;
      g_prices[i][1] = 0;
      g_prices[i][2] = 0;
      g_prices[i][3] = 0;
    }

    if ((g_sales[i][0] != 0) || (g_sales[i][1] != 0) ||
        (g_sales[i][2] != 0) || (g_sales[i][3] != 0) ||
        (g_sales[i][4] != 0)) {
      g_sales[i][0] = 0;
      g_sales[i][1] = 0;
      g_sales[i][2] = 0;
      g_sales[i][3] = 0;
      g_sales[i][4] = 0;
    }
  }
} /* clear_records() */

/*
 * Reads input from given file name into global arrays, returns appropriate
 * error code if any errors encountered.
 */

int read_tables(char *file_name) {
  FILE *in_file_ptr = NULL;
  in_file_ptr = fopen(file_name, "r");
  if (in_file_ptr == NULL) {
    return FILE_READ_ERROR;
  }

  char dealership[MAX_NAME_LEN];
  char salesperson[MAX_NAME_LEN];
  int sedan_price = 0;
  int suv_price = 0;
  int truck_price = 0;
  int motorcycle_price = 0;
  int sedan_sales = 0;
  int suv_sales = 0;
  int truck_sales = 0;
  int motorcycle_sales = 0;
  float commission = 0.0;
  int returned_value = 0;
  int read_this_iteration = 0;
  while ((returned_value = fscanf(in_file_ptr,
    "%39[^\n#]#%39[^\n#]#%d#%d#%d#%d#%d#%d#%d#%d#%f\n", dealership, salesperson,
    &sedan_price, &suv_price, &truck_price, &motorcycle_price, &sedan_sales,
    &suv_sales, &truck_sales, &motorcycle_sales, &commission)) == 11) {
    read_this_iteration++;
    int i = 0;
    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_dealerships[i][0] == '\0') {
        strcpy(g_dealerships[i], dealership);
        break;
      }
    }

    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_salespeople[i][0] == '\0') {
        strcpy(g_salespeople[i], salesperson);
        break;
      }
    }

    for (i = 0; i < MAX_RECORDS; i++) {
      if ((g_prices[i][0] == 0) && (g_prices[i][1] == 0) &&
          (g_prices[i][2] == 0) && (g_prices[i][3] == 0)) {
        g_prices[i][0] = sedan_price;
        g_prices[i][1] = suv_price;
        g_prices[i][2] = truck_price;
        g_prices[i][3] = motorcycle_price;
        break;
      }
    }

    for (i = 0; i < MAX_RECORDS; i++) {
      if ((g_sales[i][0] == 0) && (g_sales[i][1] == 0) &&
          (g_sales[i][2] == 0) && (g_sales[i][3] == 0)) {
        g_sales[i][0] = sedan_sales;
        g_sales[i][1] = suv_sales;
        g_sales[i][2] = truck_sales;
        g_sales[i][3] = motorcycle_sales;
        g_sales[i][4] = commission * 100;
        break;
      }
    }
  }

  g_dealership_count = read_this_iteration;
  fclose(in_file_ptr);
  in_file_ptr = NULL;
  if (read_this_iteration > MAX_RECORDS) {
    clear_records();
    return OUT_OF_BOUNDS;
  }
  else if ((read_this_iteration == 0) && (returned_value == EOF)) {
    clear_records();
    return NO_DATA_POINTS;
  }
  else if (returned_value != EOF) {
    /* encountered unexpected data type in input file */

    clear_records();
    return RECORD_ERROR;
  }
  return read_this_iteration;
} /* read_tables() */

/*
 * Prints total car sales for each dealership to the output file.
 */

int show_total_sales(char *out_file) {
  FILE *out_file_ptr = NULL;
  out_file_ptr = fopen(out_file, "w");
  if (out_file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    int total_sales = g_sales[i][0] + g_sales[i][1] + g_sales[i][2] +
      g_sales[i][3];

    /* prints total sales for dealership to output file */

    fprintf(out_file_ptr, "%s: %d\n", g_dealerships[i], total_sales);
  }

  fclose(out_file_ptr);
  out_file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }

  return OK;
} /* show_total_sales() */

/*
 * Writes the average car price for each dealership to output file, with each
 * car price rounded to two decimal places.
 */

int show_average_prices(char *out_file) {
  FILE *out_file_ptr = NULL;
  out_file_ptr = fopen(out_file, "w");
  if (out_file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    int total_prices = g_prices[i][0] + g_prices[i][1] + g_prices[i][2] +
      g_prices[i][3];

    /* prints average sale per dealership to output file */

    fprintf(out_file_ptr, "%s: %.2f\n", g_dealerships[i], (total_prices /
      (float) 4));
  }

  fclose(out_file_ptr);
  out_file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return OK;
} /* show_average_prices() */

/*
 * Calculates the total revenue for specified dealership, where revenue is the
 * sum of the price of each car multiplied by the number of sales.
 */

int calculate_revenue(char *dealership) {
  int revenue = 0;
  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    if (strcmp(&g_dealerships[i][0], dealership) == 0) {
      /* dealership in global array matches dealership parameter*/

      revenue = (g_prices[i][0] * g_sales[i][0]) + (g_prices[i][1] *
        g_sales[i][1]) + (g_prices[i][2] * g_sales[i][2]) + (g_prices[i][3] *
        g_sales[i][3]);
      break;
    }
  }

  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  else if (revenue == 0) {
    /* iterated through g_dealerships, could not find dealership parameter */
    /* so revenue was never changed */

    return NO_SUCH_NAME;
  }
  return revenue;
} /* calculate_revenue() */

/*
 * Calculates the salary of specified salesperson by multiplying revenue of
 * the employee's dealership by the salesperson's commission.
 */

float employee_salary(char *salesperson) {
  float salary = 0.0;
  int salary_changed = 0;
  for (int i = 0; (g_dealerships[i][0] != '\0') && (g_salespeople[i][0] !=
    '\0') && (i < MAX_RECORDS); i++) {
    if (strcmp(&g_salespeople[i][0], salesperson) == 0) {
      float commission = g_sales[i][4] / (float) 100;
      salary = calculate_revenue(&g_dealerships[i][0]) * commission;
      salary_changed = 1;
      break;
    }
  }

  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  else if (salary_changed == 0) {
    /* iterated through g_salespeople, could not find salesperson parameter */
    /* so salary was never changed */

    return NO_SUCH_NAME;
  }
  return salary;
} /* employee_salary() */

/*
 * Finds the maximum salary out of all salespeople.
 */

float calculate_max_salary() {
  float max_salary = 0.0;
  for (int i = 0; (g_salespeople[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    float temp_salary = employee_salary(&g_salespeople[i][0]);
    if (temp_salary > max_salary) {
      /* the maximum salary value is changed if the recently calculated */
      /* salary is greater */

      max_salary = temp_salary;
    }
  }

  if (max_salary == 0.0) {
    return NO_DATA_POINTS;
  }
  return max_salary;
} /* calculate_max_salary() */

/*
 * Displays the most common sale per dealership in specified output file.
 */

int show_most_common_sale(char *out_file) {
  FILE *out_file_ptr = NULL;
  out_file_ptr = fopen(out_file, "w");
  if (out_file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  int vehicle_most_sold = -1;
  int largest_value = -1;
  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    for (int j = 0; j < 4; j++) {
      if (g_sales[i][j] > largest_value) {
        /* current array element is larger than current most common sale */

        vehicle_most_sold = j;
        largest_value = g_sales[i][j];
      }
      else if (g_sales[i][j] == largest_value) {
        /* more than one vehicle is most sold by dealership */

        vehicle_most_sold = -1;
      }
    }
    char *most_common_sale = "";
    switch (vehicle_most_sold) {
      case -1: {
        most_common_sale = "More than one";
        break;
      }
      case 0: {
        most_common_sale = "Sedan";
        break;
      }
      case 1: {
        most_common_sale = "SUV";
        break;
      }
      case 2: {
        most_common_sale = "Truck";
        break;
      }
      case 3: {
        most_common_sale = "Motorcycle";
        break;
      }
    }
    fprintf(out_file_ptr, "%s: %s\n", g_dealerships[i], most_common_sale);

    /* resets values for next iteration through g_dealerships */

    vehicle_most_sold = -1;
    largest_value = -1;
  }
  fclose(out_file_ptr);
  out_file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return OK;
} /* show_most_common_sale() */

/*
 * Writes the specified data in start and end columns for specified table to
 * output file, separated by commas. If the table index is either 1 or 2, the
 * entire row is printed to the output file.
 */

int write_tables(char *out_file, int table_index, int start_col, int end_col) {
  if ((table_index < DEALERSHIPS) || (table_index > SALES)) {
    return INVALID_TABLE_INDEX;
  }
  else if ((end_col < start_col) || (start_col < 0) ||
           (end_col > MAX_NAME_LEN)) {
    return INVALID_COLUMN;
  }
  else if ((table_index == PRICES) && ((start_col < 0) || (end_col >
           (NUM_PRICE_COLS - 1)))) {
    return INVALID_COLUMN;
  }
  else if ((table_index == SALES) && ((start_col < 0) || (end_col >
           (NUM_SALES_COLS - 1)))) {
    return INVALID_COLUMN;
  }
  else if (((table_index == DEALERSHIPS) || (table_index == SALESPEOPLE)) &&
           ((start_col != 0) || (end_col != 0))) {
    /* for char* arrays, there is only one column, which should be column 0 */

    return INVALID_COLUMN;
  }

  FILE *out_file_ptr = NULL;
  out_file_ptr = fopen(out_file, "w");
  if (out_file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  switch (table_index) {
    /* prints char* arrays if table_index is 1 or 2 */

    case DEALERSHIPS: {
      for (int i = 0; g_dealerships[i][0] != '\0'; i++) {
        for (int j = 0; g_dealerships[i][j] != '\0'; j++) {
          fprintf(out_file_ptr, "%c", g_dealerships[i][j]);
        }
        fprintf(out_file_ptr, "\n");
      }
      break;
    }
    case SALESPEOPLE: {
      for (int i = 0; g_salespeople[i][0] != '\0'; i++) {
        for (int j = 0; g_salespeople[i][j] != '\0'; j++) {
          fprintf(out_file_ptr, "%c", g_salespeople[i][j]);
        }
        fprintf(out_file_ptr, "\n");
      }
      break;
    }
  }

  if ((table_index == PRICES) || (table_index == SALES)) {
    for (int i = 0; (g_dealerships[i][0] != '\0') && (g_salespeople[i][0] !=
    '\0') && (i < MAX_RECORDS); i++) {
      for (int j = start_col; j <= end_col; j++) {
        switch (table_index) {
          /* prints each specified element up to ending column in tables 3/4 */

          case PRICES: {
            fprintf(out_file_ptr, "%d", g_prices[i][j]);
            break;
          }
          case SALES: {
            fprintf(out_file_ptr, "%d", g_sales[i][j]);
            break;
          }
        }

        if (j != end_col) {
          fprintf(out_file_ptr, ",");
        }
      }
      fprintf(out_file_ptr, "\n");
    }
  }

  fclose(out_file_ptr);
  out_file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return OK;
} /* write_tables() */
