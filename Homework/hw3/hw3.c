/* Name, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

/* Add any includes here */
#include "hw3.h"
#include <stdio.h>
#include <string.h>

int g_dealership_count;
char g_dealerships[MAX_RECORDS][MAX_NAME_LEN];
char g_salespeople[MAX_RECORDS][MAX_NAME_LEN];
int g_prices[MAX_RECORDS][NUM_PRICE_COLS];
int g_sales[MAX_RECORDS][NUM_SALES_COLS];

int read_tables(char *file_name) {
  int i = 0;
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
  int returned_value = 1;
  FILE *file_ptr = NULL;
  file_ptr = fopen(file_name, "r");
  if (file_ptr == NULL) {
    return FILE_READ_ERROR;
  }

  returned_value = fscanf(file_ptr,
    "%40[^\n#]s%40[^\n#]s%d#%d#%d#%d#%d#%d#%d#%d#%f", dealership, salesperson,
    &sedan_price, &suv_price, &truck_price, &motorcycle_price, &sedan_sales,
    &suv_sales, &truck_sales, &motorcycle_sales, &commission);

  while ((returned_value = fscanf(file_ptr,
    "%40[^\n#]s%40[^\n]s#%d#%d#%d#%d#%d#%d#%d#%d#%f", dealership, salesperson,
    &sedan_price, &suv_price, &truck_price, &motorcycle_price, &sedan_sales,
    &suv_sales, &truck_sales, &motorcycle_sales, &commission)) == 1) {
    g_dealership_count++;
    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_dealerships[i] == NULL) {
        g_dealerships[i][0] = *dealership;
        break;
      }
    }
    if (i == MAX_RECORDS) {
      returned_value = 12;
      break;
    }

    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_salespeople[i] == NULL) {
        g_salespeople[i][0] = *salesperson;
        break;
      }
    }
    if (i == MAX_RECORDS) {
      returned_value = 12;
      break;
    }

    for (i = 0; i < MAX_RECORDS; i++) {
      if (g_prices[i] == NULL) {
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
      if (g_sales[i] == NULL) {
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

  fclose(file_ptr);
  file_ptr = NULL;
  if (returned_value == 12) {
    return OUT_OF_BOUNDS;
  }
  else if (returned_value != EOF) {
    return NO_DATA_POINTS;
  }

  return OK;
} /* read_tables() */

int show_total_sales(char *out_file) {
  int i = 0;
  int total_sales = 0;
  FILE *file_ptr = NULL;
  file_ptr = fopen(out_file, "w");
  if (file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (i = 0; (g_dealerships[i] != NULL) && (i < MAX_RECORDS); i++) {
    total_sales = g_sales[i][0] + g_sales[i][1] + g_sales[i][2] +
      g_sales[i][3];
    fprintf(file_ptr, "%s: %d\n", &g_dealerships[i][0], total_sales);
  }

  fclose(file_ptr);
  file_ptr = NULL;
  return OK;
} /* show_total_scores() */

int show_average_prices(char *out_file) {
  int i = 0;
  float total_prices = 0;
  FILE *file_ptr = NULL;
  file_ptr = fopen(out_file, "w");
  if (file_ptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (i = 0; (g_dealerships[i] != NULL) && (i < MAX_RECORDS); i++) {
    total_prices = g_prices[i][0] + g_prices[i][1] + g_prices[i][2] +
      g_prices[i][3];
    fprintf(file_ptr, "%s: %.2f\n", &g_dealerships[i][0], (total_prices / 4));
  }

  fclose(file_ptr);
  file_ptr = NULL;
  return OK;
} /* show_average_prices() */

int calculate_revenue(char *dealership) {
  int i = 0;
  int revenue = 0;
  for (i = 0; i < MAX_RECORDS; i++) {
    if (strcmp(&g_dealerships[i][0], dealership) == 0) {
      revenue = (g_prices[i][0] * g_sales[i][0]) + (g_prices[i][1] *
        g_sales[i][1]) + (g_prices[i][2] * g_sales[i][2]) + (g_prices[i][3] *
        g_sales[i][3]);
      break;
    }
  }

  if (revenue == 0) {
    return NO_SUCH_NAME;
  }
  return revenue;
} /* calculate_revenue() */

float employee_salary(char *salesperson) {
  return OK;
} /* employee_salary() */

float calculate_max_salary() {
  return OK;
} /* calculate_max_salary() */

int show_most_common_sale(char *out_file) {
  return OK;
} /* show_most_common_salary() */

int write_tables(char *out_file, int table_index, int start_col, int end_col) {
  return OK;
} /* write_tables() */
