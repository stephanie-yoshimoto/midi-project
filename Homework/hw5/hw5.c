/* Stephanie Yoshimoto, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

#include "hw5.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * Performs the assertion checks to check if employee fields are valid.
 */

int check_valid_employee(employee_t employee) {
  int last_char = strlen(employee.first_name);
  if (employee.first_name[last_char + 1] != '\0') {
    assert(false);
  }
  last_char = strlen(employee.last_name);
  if (employee.last_name[last_char + 1] != '\0') {
    assert(false);
  }
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

/*
 * Reads the employee record specified by second parameter, returns employee
 * if found.
 */

employee_t read_employee(FILE *in_file_ptr, int employee_record_num) {
  assert((in_file_ptr != NULL) && (employee_record_num >= 0));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return BAD_EMPLOYEE;
  }
  for (int i = 0; i <= employee_record_num; i++) {
    employee_t read_employee = BAD_EMPLOYEE;
    returned_value = fread(&read_employee, sizeof(employee_t), 1,
                           in_file_ptr);
    if (returned_value != 1) {
      return BAD_EMPLOYEE;
    }

    if (i == employee_record_num) {
      return read_employee;
    }
  }
  return BAD_EMPLOYEE;
} /* read_employee() */

/*
 * Writes specified employee information to specified file position.
 */

int write_employee(FILE *in_file_ptr, employee_t employee, int file_position) {
  assert((in_file_ptr != NULL) && (file_position >= 0) &&
         (check_valid_employee(employee) == OK));
  int returned_value = fseek(in_file_ptr, file_position, SEEK_SET);
  if (returned_value != 0) {
    return WRITE_ERR;
  }
  returned_value = fwrite(&employee, sizeof(employee_t), 1,
                          in_file_ptr);
  if (returned_value != 1) {
    return WRITE_ERR;
  }
  return OK;
} /* write_employee() */

/*
 * Searches file for first free office, adds employee to file at location.
 */

int hire_employee(FILE *file_ptr, employee_t employee) {
  assert((file_ptr != NULL) && (check_valid_employee(employee) == OK));
  employee_t read_employee = BAD_EMPLOYEE;
  int returned_value = fseek(file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return NO_OFFICE;
  }
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, file_ptr);
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

/*
 * Searches for specified employee, fires employee by setting id_number to
 * FREE_OFFICE.
 */

int fire_employee(FILE *in_file_ptr, employee_t employee) {
  assert((in_file_ptr != NULL) && (check_valid_employee(employee)));
  employee_t read_employee = BAD_EMPLOYEE;
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return NO_EMPLOYEE;
  }
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1,
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
  return NO_EMPLOYEE;
} /* fire_employee() */

/*
 * Finds percentages of offices occupied in which the salary of the employee is
 * greater than parameter.
 */

float percent_occupancy(FILE *in_file_ptr, float salary) {
  assert((in_file_ptr != NULL) && (salary >= 0.0));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  employee_t read_employee = BAD_EMPLOYEE;
  int total_employees = 0;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != 1) {
      break;
    }

    if (read_employee.id_number != FREE_OFFICE) {
      total_employees++;
    }
  }

  int employees_with_greater_salary = 0;
  returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return NO_EMPLOYEE;
  }

  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != 1) {
      /* EOF is reached */

      break;
    }

    if (read_employee.salary > salary) {
      employees_with_greater_salary++;
    }
  }

  return (float) ((employees_with_greater_salary / total_employees) * 100);
} /* percent_occupancy() */

/*
 * Calculates average salary earned per employee specified by title.
 */

float average_salary_by_title(FILE *in_file_ptr, enum title_t title) {
  assert((in_file_ptr != NULL) && ((title == TECHNICIAN) || (title ==
          MANAGER) || (title == SALESPERSON)));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return NO_EMPLOYEE;
  }
  float employees_with_same_title = 0;
  float cumulative_salaries = 0.0;
  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != 1) {
      break;
    }

    if ((read_employee.id_number != FREE_OFFICE) && (read_employee.title ==
        title)) {
      employees_with_same_title++;
    }
  }

  if (employees_with_same_title == 0) {
    return NO_EMPLOYEE;
  }

  returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != 0) {
    return NO_EMPLOYEE;
  }

  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != 1) {
      break;
    }

    if (read_employee.title == title) {
      cumulative_salaries += read_employee.salary;
    }
  }

  return (cumulative_salaries / employees_with_same_title);
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

int main() {
  printf("%lu\n", sizeof(employee_t));
  FILE *fp = fopen("output.txt", "w+");
  for (int i = 0; i < 10; i++) {
    employee_t employee = {34, "John", "Purdue", TECHNICIAN, 100.00, ""};
    fseek(fp, 0, SEEK_END);
    fwrite(&employee, sizeof(employee_t), 1, fp);
  }

  employee_t employee = {34, "John", "Purdue", TECHNICIAN, 100.00, ""};
  /*read_employee(fp, 0);
  write_employee(fp, employee, 100);
  hire_employee(fp, employee);
  fire_employee(fp, employee);
  percent_occupancy(fp, 50);*/
  average_salary_by_title(fp, TECHNICIAN);
  return 0;
}
