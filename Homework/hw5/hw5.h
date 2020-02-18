#ifndef HW5_H
#define HW5_H

#include <stdio.h>

/* Constant definitions */
#define MAX_NAME_LEN    (20)  /* Max length of any string */
#define FREE_OFFICE     (-10)
#define N_DAYS          (5)
#define N_HOURS         (9)

/* Error codes */
#define OK              (-1)   /* No errors, everything as should be */
#define NO_EMPLOYEE     (-2)
#define NO_OVERLAP      (-3)
#define WRITE_ERR       (-4)
#define NO_OFFICE       (-5)

#define BAD_EMPLOYEE ((employee_t){\
  .id_number = FREE_OFFICE,\
  .first_name = "BOGUS",\
  .last_name = "BOGUS",\
  .title = -1,\
  .salary = -1.0,\
  .schedule[N_DAYS - 1][N_HOURS - 1] = 'A'})

enum title_t {
  TECHNICIAN,
  MANAGER,
  SALESPERSON
};

/* Structure Declarations */
typedef struct {
  int id_number;
  char first_name[MAX_NAME_LEN];
  char last_name[MAX_NAME_LEN];
  enum title_t title;
  float salary;
  char schedule[N_DAYS][N_HOURS];
} employee_t;

/* Function prototypes */
employee_t read_employee(FILE *, int);
int write_employee(FILE *, employee_t, int);
int hire_employee(FILE *, employee_t);
int fire_employee(FILE *, employee_t);
float percent_occupancy(FILE *, float);
float average_salary_by_title(FILE *, enum title_t);
employee_t find_employee_by_id(FILE *, int);
employee_t find_employee_by_name(FILE *, char *, char *);
int give_raise(FILE *, int, float);
int schedule_meeting(FILE *, int, int);

#endif // HW5_H
