/* Stephanie Yoshimoto, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

#include "hw5.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

int check_valid_employee(employee_t employee) {
  assert((strlen(employee.first_name) == (sizeof(employee.first_name) - 1)) &&
         (strlen(employee.last_name) == (sizeof(employee.last_name) - 1)));
  for (int i = 0; i < N_DAYS; i++) {
    for (int j = 0; j < N_HOURS; j++) {
      bool valid_entry = ((employee.schedule[i][j] == 'A') ||
                          (employee.schedule[i][j] == 'B'));
      assert((employee.id_number == FREE_OFFICE) || ((employee.id_number > 0) &&
         ((employee.title == TECHNICIAN) || (employee.title == MANAGER) ||
         (employee.title == SALESPERSON)) && (employee.salary > 0) &&
         (valid_entry)));
    }
  }
  return OK;
} /* check_valid_employee() */

employee_t read_employee(FILE *in_file_ptr, int employee) {
  assert((in_file_ptr != NULL) && (employee >= 0));
  for (int i = 1; i <= employee; i++) {
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
  assert((in_file_ptr != NULL) && (file_position >= 0) &&
         (check_valid_employee(employee) == OK));
  int returned_value = fwrite(&employee, file_position * sizeof(employee_t), 1,
                              in_file_ptr);
  if (returned_value != 1) {
    return WRITE_ERR;
  }
  return OK;
} /* write_employee() */

int hire_employee(FILE *file_ptr, employee_t employee) {
  assert((file_ptr != NULL) && (check_valid_employee(employee) == OK));
  employee_t read_employee = {0, "", "", TECHNICIAN, 0.0, ""};
  while (true) {
    int returned_value = fread(&read_employee, sizeof(employee_t), 1, file_ptr);
    if (returned_value != 1) {
      return NO_OFFICE;
    }

    if (read_employee.id_number == FREE_OFFICE) {
      returned_value = fwrite(&employee, sizeof(employee_t), 1, file_ptr);
      if (returned_value != -1) {
        return WRITE_ERR;
      }
      return employee.id_number;
    }
  }
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
