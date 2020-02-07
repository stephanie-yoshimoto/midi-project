/* Stephanie Yoshimoto, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

#include "hw3.h"

#include <stdio.h>
#include <string.h>

int g_dealership_count = 0;
char g_dealerships[MAX_RECORDS][MAX_NAME_LEN];
char g_salespeople[MAX_RECORDS][MAX_NAME_LEN];
int g_prices[MAX_RECORDS][NUM_PRICE_COLS];
int g_sales[MAX_RECORDS][NUM_SALES_COLS];

int read_tables(char *file_name) {
  FILE *file_ptr = NULL;
  file_ptr = fopen(file_name, "r");
  if (file_ptr == NULL) {
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
  float commission = 0.0f;
  int returned_value = 0;
  int read_this_iteration = 0;
  while ((returned_value = fscanf(file_ptr,
    "%39[^\n#]#%39[^\n#]#%d#%d#%d#%d#%d#%d#%d#%d#%f\n", dealership, salesperson,
    &sedan_price, &suv_price, &truck_price, &motorcycle_price, &sedan_sales,
    &suv_sales, &truck_sales, &motorcycle_sales, &commission) == 11)) {
    read_this_iteration++;
    int i = 0;
    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_dealerships[i][0] == '\0') {
        strcpy(g_dealerships[i], dealership);
        break;
      }
    }
    if (i == MAX_RECORDS) {
      returned_value = 12;
      break;
    }

    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_salespeople[i][0] == '\0') {
        strcpy(g_salespeople[i], salesperson);
        break;
      }
    }
    if (i == MAX_RECORDS) {
      returned_value = 12;
      break;
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
    if (i == MAX_RECORDS) {
      returned_value = 12;
      break;
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
    if (i == MAX_RECORDS) {
      returned_value = 12;
      break;
    }
  }

  g_dealership_count = read_this_iteration;
  fclose(file_ptr);
  file_ptr = NULL;
  if (returned_value == 12) {
    return OUT_OF_BOUNDS;
  }
  else if ((read_this_iteration == 0) && ((returned_value == 0) ||
    (returned_value == -1))) {
    return NO_DATA_POINTS;
  }
  else if ((returned_value != 0) && (returned_value != -1)) {
    return RECORD_ERROR;
  }
  /*else if ((read_this_iteration != 0) && ((returned_value == EOF))) {
    return OK;
  }*/

  return read_this_iteration;
} /* read_tables() */

int show_total_sales(char *out_file) {
  FILE *file_ptr = NULL;
  file_ptr = fopen(out_file, "w");
  if (file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    int total_sales = g_sales[i][0] + g_sales[i][1] + g_sales[i][2] +
      g_sales[i][3];
    fprintf(file_ptr, "%s: %d\n", g_dealerships[i], total_sales);
  }

  fclose(file_ptr);
  file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }

  return OK;
} /* show_total_scores() */

int show_average_prices(char *out_file) {
  FILE *file_ptr = NULL;
  file_ptr = fopen(out_file, "w");
  if (file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    int total_prices = g_prices[i][0] + g_prices[i][1] + g_prices[i][2] +
      g_prices[i][3];
    fprintf(file_ptr, "%s: %.2f\n", g_dealerships[i], (total_prices /
      (float) 4));
  }

  fclose(file_ptr);
  file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return OK;
} /* show_average_prices() */

int calculate_revenue(char *dealership) {
  int revenue = 0;
  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    if (strcmp(&g_dealerships[i][0], dealership) == 0) {
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
    return NO_SUCH_NAME;
  }
  return revenue;
} /* calculate_revenue() */

float employee_salary(char *salesperson) {
  int salary = 0;
  for (int i = 0; (g_dealerships[i][0] != '\0') && (g_salespeople[i][0] !=
    '\0') && (i < MAX_RECORDS); i++) {
    if (strcmp(&g_salespeople[i][0], salesperson) == 0) {
      salary = calculate_revenue(&g_dealerships[i][0]) * g_sales[i][4];
      break;
    }
  }

  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  else if (salary == 0) {
    return NO_SUCH_NAME;
  }
  return salary;
} /* employee_salary() */

float calculate_max_salary() {
  float max_salary = 0;
  for (int i = 0; (g_salespeople[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    float temp_salary = employee_salary(&g_salespeople[i][0]);
    if (temp_salary > max_salary) {
      max_salary = temp_salary;
    }
  }

  if (max_salary == 0) {
    return NO_DATA_POINTS;
  }
  return max_salary;
} /* calculate_max_salary() */

int show_most_common_sale(char *out_file) {
  FILE *file_ptr = NULL;
  file_ptr = fopen(out_file, "w");
  if (file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  int vehicle_most_sold = -1;
  int largest_value = -1;
  for (int i = 0; (g_dealerships[i][0] != '\0') && (i < MAX_RECORDS); i++) {
    for (int j = 0; j < 4; j++) {
      if (g_sales[i][j] > largest_value) {
        vehicle_most_sold = j;
        largest_value = g_sales[i][j];
      }
      else if (g_sales[i][j] == largest_value) {
        vehicle_most_sold = -1;
      }
    }
    char *most_common_sale;
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
      default: {
        fclose(file_ptr);
        file_ptr = NULL;
        return RECORD_ERROR;
      }
    }
    fprintf(file_ptr, "%s: %s\n", g_dealerships[i], most_common_sale);
    vehicle_most_sold = -1;
    largest_value = -1;
  }
  fclose(file_ptr);
  file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return OK;
} /* show_most_common_salary() */

int write_tables(char *out_file, int table_index, int start_col, int end_col) {
  if ((table_index < 1) || (table_index > 4)) {
    return INVALID_TABLE_INDEX;
  }
  else if ((end_col < start_col) || (start_col < 0) ||
    (end_col > MAX_NAME_LEN)) {
    return INVALID_COLUMN;
  }
  else if ((table_index == 3) && ((start_col < 0) || (end_col >
    (NUM_PRICE_COLS - 1)))) {
    return INVALID_COLUMN;
  }
  else if ((table_index == 4) && ((start_col < 0) || (end_col >
    (NUM_SALES_COLS - 1)))) {
    return INVALID_COLUMN;
  }

  FILE *file_ptr = NULL;
  file_ptr = fopen(out_file, "w");
  if (file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (int i = 0; (g_dealerships[i][0] != '\0') && (g_salespeople[i][0] !=
    '\0') && (i < MAX_RECORDS); i++) {
    for (int j = start_col; j <= end_col; j++) {
      switch (table_index) {
        case 1: {
          if (g_dealerships[i][j]) {
            fprintf(file_ptr, "%c", g_dealerships[i][j]);
          }
          else {
            fclose(file_ptr);
            file_ptr = NULL;
            return RECORD_ERROR;
          }
          break;
        }
        case 2: {
          if (g_salespeople[i][j]) {
            fprintf(file_ptr, "%c", g_salespeople[i][j]);
          }
          else {
            fclose(file_ptr);
            file_ptr = NULL;
            return RECORD_ERROR;
          }
          break;
        }
        case 3: {
          fprintf(file_ptr, "%d", g_prices[i][j]);
          break;
        }
        case 4: {
          fprintf(file_ptr, "%d", g_sales[i][j]);
          break;
        }
      }

      if (j != end_col) {
        fprintf(file_ptr, ",");
      }
    }
    fprintf(file_ptr, "\n");
  }

  fclose(file_ptr);
  file_ptr = NULL;
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return OK;
} /* write_tables() */

int main() {
  read_tables("no_data.txt");
  read_tables("test_data_files/Bogus_826");
  read_tables("test_data_files/Empty_712");
  read_tables("test_data_files/Input_303");
  read_tables("test_data_files/Long_983");
  show_total_sales("output.txt");
  show_average_prices("output.txt");
  show_most_common_sale("output.txt");
  write_tables("output.txt", 4, 1, 3);
  calculate_revenue("Lafayette Motor Vehicles");
  calculate_revenue("does not exist");
  return 0;
}