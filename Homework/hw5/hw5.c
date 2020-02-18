/* Stephanie Yoshimoto, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

/* Add any includes here */

#include "hw5.h"

#include <assert.h>

employee_t read_employee(FILE *in_file_ptr, int employee) {
  assert((in_file_ptr != NULL) && (employee >= 0));
  for (int i = 0; i <= employee; i++) {
    employee_t read_employee = {0, "", "", TECHNICIAN, 0.0, ""};
    int returned_value = fread(&read_employee, sizeof(employee_t), 1,
                              in_file_ptr);
    if (returned_value != 1) {
      return BAD_EMPLOYEE;
    }

    if (i == employee) {
      return read_employee;
    }
  }
  return BAD_EMPLOYEE;
} /* read_employee() */

int write_employee(FILE *in_file_ptr, employee_t employee, int file_position) {
  return OK;
} /* write_employee() */

int hire_employee(FILE *in_file_ptr, employee_t employee) {
  return OK;
} /* hire_employee() */

int fire_employee(FILE *in_file_ptr, employee_t employee) {
  return OK;
} /* fire_employee() */

float percent_occupancy(FILE *file_in_ptr, float salary) {
  return 0.0;
} /* percent_occupancy() */

float average_salary_by_title(FILE *in_file_ptr, enum title_t title) {
  return 0.0;
} /* average_salary_by_title() */

employee_t find_employee_by_id(FILE *in_file_ptr, int id_number) {
  return BAD_EMPLOYEE;
} /* find_employee_by_id() */

employee_t find_employee_by_name(FILE *in_file_ptr, char *name,
                                 char *last_name) {
  return BAD_EMPLOYEE;
} /* find_employee_by_name() */

int give_raise(FILE *in_file_ptr, int id_number, float salary_increase) {
  return OK;
} /* give_raise() */

int schedule_meeting(FILE *in_file_ptr, int id_number_1, int id_number_2) {
  return OK;
} /* schedule_meeting() */
