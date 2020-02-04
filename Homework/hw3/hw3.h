#ifndef HW3_H
#define HW3_H

/* Constant definitions */
#define MAX_NAME_LEN (40)
#define MAX_FILE_LEN (30)
#define MAX_RECORDS (100)

#define NUM_PRICE_COLS (4)
#define NUM_SALES_COLS (5)

/* Error codes */
#define OK (0)
#define FILE_READ_ERROR (-1)
#define FILE_WRITE_ERROR (-2)
#define RECORD_ERROR (-3)
#define INVALID_TABLE_INDEX (-4)
#define INVALID_COLUMN (-5)
#define NO_SUCH_NAME (-6)
#define OUT_OF_BOUNDS (-7)
#define NO_DATA_POINTS (-8)

/* Function prototypes */
int read_tables(char *);
int show_total_sales(char *);
int show_average_prices(char *);
int calculate_revenue(char *);
float employee_salary(char *);
float calculate_max_salary(void);
int show_most_common_sale(char *);
int write_tables(char *, int, int, int);

/* Global variables */
extern int g_dealership_count;
extern char g_dealerships[MAX_RECORDS][MAX_NAME_LEN];
extern char g_salespeople[MAX_RECORDS][MAX_NAME_LEN];
extern int g_prices[MAX_RECORDS][NUM_PRICE_COLS];
extern int g_sales[MAX_RECORDS][NUM_SALES_COLS];

#endif // HW3_H
