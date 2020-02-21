/* Stephanie Yoshimoto, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

#include "hw5.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
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
    employee_t read_employee = BAD_EMPLOYEE;
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
  int returned_value = fseek(in_file_ptr, file_position, SEEK_SET);
  if (returned_value != 0) {
    return WRITE_ERR;
  }
  returned_value = fwrite(&employee, file_position * sizeof(employee_t), 1,
                          in_file_ptr);
  if (returned_value != 1) {
    return WRITE_ERR;
  }
  return OK;
} /* write_employee() */

int hire_employee(FILE *file_ptr, employee_t employee) {
  assert((file_ptr != NULL) && (check_valid_employee(employee) == OK));
  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    int returned_value = fread(&read_employee, sizeof(employee_t), 1, file_ptr);
    if (returned_value != 1) {
      return NO_OFFICE;
    }

    if (read_employee.id_number == FREE_OFFICE) {
      returned_value = fseek(file_ptr, -1 * sizeof(employee_t), SEEK_CUR);
      if (returned_value != 0) {
        return WRITE_ERR;
      }
      returned_value = fwrite(&employee, sizeof(employee_t), 1, file_ptr);
      if (returned_value != 1) {
        return WRITE_ERR;
      }
      return employee.id_number;
    }
  }
  return NO_OFFICE;
} /* hire_employee() */

int fire_employee(FILE *in_file_ptr, employee_t employee) {
  assert((in_file_ptr != NULL) && (check_valid_employee(employee)));
  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    int returned_value = fread(&read_employee, sizeof(employee_t), 1,
      in_file_ptr);
    if (returned_value != 1) {
      return NO_EMPLOYEE;
    }

    if ((read_employee.id_number == employee.id_number) &&
        (strcmp(read_employee.first_name, employee.first_name) == 0) &&
        (strcmp(read_employee.last_name, employee.last_name) == 0)) {
      int id_number = read_employee.id_number;
      read_employee.id_number = FREE_OFFICE;
      returned_value = fseek(in_file_ptr, -1 * sizeof(employee_t), SEEK_CUR);
      if (returned_value != 0) {
        return WRITE_ERR;
      }
      returned_value = fwrite(&read_employee, sizeof(employee_t), 1,
        in_file_ptr);
      if (returned_value != 1) {
        return WRITE_ERR;
      }
      return id_number;
    }
  }
  return OK;
} /* fire_employee() */

float percent_occupancy(FILE *in_file_ptr, float salary) {
  assert((in_file_ptr != NULL) && (salary >= 0.0));
  int returned_value = fseek(in_file_ptr, 0, SEEK_END);
  if (returned_value != 0) {
    return NO_EMPLOYEE;
  }
  long file_length = ftell(in_file_ptr);
  int employees = (file_length / sizeof(employee_t));
  int employees_with_greater_salary = 0;
  returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return NO_EMPLOYEE;
  }

  while (true) {
    employee_t read_employee = BAD_EMPLOYEE;
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != 1) {
      /* EOF is reached */

      break;
    }

    if (read_employee.salary > salary) {
      employees_with_greater_salary++;
    }
  }

  return (float) ((employees_with_greater_salary / employees) * 100);
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

/*int main() {
  printf("%lu\n", sizeof(employee_t));
  return 0;
}*/
