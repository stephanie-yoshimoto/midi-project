#ifndef HW2_H
#define HW2_H

#define MAX_FILE_LEN (30) /* The maximum file name length */
#define MAX_NAME_LEN (40) /* The maximum length of a student's name */

/* Error Codes */

#define OK              (0)   /* No errors, everything as should be     */
#define BAD_RECORD (-1) /* Record is somehow malformed */
#define BAD_DATE (-2) /* Timestamp is invalid (month not 1-12, day not 1-31) */
#define FILE_READ_ERR (-3) /* Error reading from a file */
#define FILE_WRITE_ERR (-4) /* Error writing to a file */
#define NO_DATA_POINTS (-5) /* No data points */
#define RECORDS_MISMATCH (-6) /* Input must be the same length */

#define access(x,y) (printf("\n\n\n\n\nDo not use access().\n"), abort(), 1)


/* Function Prototypes */
float get_average_sleep_hours(char *, int, int);
int get_sleep_log(char *, char *);
int compare_sleep_hours(char *, char *, char *);
float get_average_calories(char *, int, int);
int compare_activity_log(char *, char *, int, int, char *);


#endif // HW2_H
