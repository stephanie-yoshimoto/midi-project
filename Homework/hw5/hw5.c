/* Stephanie Yoshimoto, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

#include "hw5.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define FREAD_FWRITE_SUCCESS (1)
#define FSEEK_SUCCESS (0)
#define ALIGN_HOURS (8)
#define ALIGN_DAY (100)
#define PERCENTAGE (100)

/*
 * Performs the assertion checks to check if employee fields are valid.
 */

bool check_valid_employee(employee_t employee) {
  for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (employee.first_name[i] == '\0') {
      break;
    }

    if (i == MAX_NAME_LEN - 1) {
      assert(false);
    }
  }

  for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (employee.last_name[i] == '\0') {
      break;
    }

    if (i == MAX_NAME_LEN - 1) {
      assert(false);
    }
  }

  if (employee.id_number == FREE_OFFICE) {
    return true;
  }

  bool valid_entry = false;
  for (int i = 0; i < N_DAYS; i++) {
    for (int j = 0; j < N_HOURS; j++) {
      valid_entry = ((employee.schedule[i][j] == 'A') ||
                     (employee.schedule[i][j] == 'B'));
      if (valid_entry == false) {
        assert(false);
      }
    }
  }
  return ((employee.id_number > 0) && ((employee.title == TECHNICIAN) ||
          (employee.title == MANAGER) || (employee.title == SALESPERSON)) &&
          (employee.salary > 0) && (valid_entry));
} /* check_valid_employee() */

/*
 * Reads the employee record specified by second parameter, returns employee
 * if found.
 */

employee_t read_employee(FILE *in_file_ptr, int employee_record_num) {
  assert((in_file_ptr != NULL) && (employee_record_num >= 0));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return BAD_EMPLOYEE;
  }

  employee_t read_employee = BAD_EMPLOYEE;
  for (int i = 0; i <= employee_record_num; i++) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1,
                           in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      return BAD_EMPLOYEE;
    }

    if (i == employee_record_num) {
      /* employee found */

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
         (check_valid_employee(employee)));
  int returned_value = fseek(in_file_ptr, 0, SEEK_END);
  if (returned_value != FSEEK_SUCCESS) {
    return WRITE_ERR;
  }
  long eof_position = ftell(in_file_ptr);
  if (eof_position < (file_position * sizeof(employee_t))) {
    return WRITE_ERR;
  }

  returned_value = fseek(in_file_ptr, file_position * sizeof(employee_t),
                         SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return WRITE_ERR;
  }

  /* writes employee parameter to file */

  returned_value = fwrite(&employee, sizeof(employee_t), 1, in_file_ptr);
  if (returned_value != FREAD_FWRITE_SUCCESS) {
    return WRITE_ERR;
  }
  return OK;
} /* write_employee() */

/*
 * Searches file for first free office, adds employee to file at location.
 */

int hire_employee(FILE *in_file_ptr, employee_t employee) {
  assert((in_file_ptr != NULL) && (check_valid_employee(employee)));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return NO_OFFICE;
  }

  int office_number = -1;
  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      return NO_OFFICE;
    }

    office_number++;
    if (read_employee.id_number == FREE_OFFICE) {
      /* rewind file by a struct in order to write over free office employee */

      returned_value = fseek(in_file_ptr, -1 * sizeof(employee_t), SEEK_CUR);
      if (returned_value != FSEEK_SUCCESS) {
        return WRITE_ERR;
      }
      returned_value = fwrite(&employee, sizeof(employee_t), 1, in_file_ptr);
      if (returned_value != FREAD_FWRITE_SUCCESS) {
        return WRITE_ERR;
      }
      return office_number;
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
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return NO_EMPLOYEE;
  }

  int office_number = -1;
  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1,
                           in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      return NO_EMPLOYEE;
    }

    office_number++;
    if ((read_employee.id_number == employee.id_number) &&
        (strcmp(read_employee.first_name, employee.first_name) == 0) &&
        (strcmp(read_employee.last_name, employee.last_name) == 0)) {
      read_employee.id_number = FREE_OFFICE;

      /* rewinds to change employee's ID number to free office */

      returned_value = fseek(in_file_ptr, -1 * sizeof(employee_t), SEEK_CUR);
      if (returned_value != FSEEK_SUCCESS) {
        return WRITE_ERR;
      }
      returned_value = fwrite(&read_employee, sizeof(employee_t), 1,
                              in_file_ptr);
      if (returned_value != FREAD_FWRITE_SUCCESS) {
        return WRITE_ERR;
      }
      return office_number;
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
  if (returned_value != FSEEK_SUCCESS) {
    return NO_EMPLOYEE;
  }

  employee_t read_employee = BAD_EMPLOYEE;
  int total_employees = 1;
  int employees_with_greater_salary = 0;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      break;
    }

    if (read_employee.id_number != FREE_OFFICE) {
      /* only counts employees without free offices */

      total_employees++;
      if (read_employee.salary > salary) {
        employees_with_greater_salary++;
      }
    }
  }

  return (((float) employees_with_greater_salary / (float) total_employees) *
          ((float) PERCENTAGE));
} /* percent_occupancy() */

/*
 * Calculates average salary earned per employee specified by title.
 */

float average_salary_by_title(FILE *in_file_ptr, enum title_t title) {
  assert((in_file_ptr != NULL) && ((title == TECHNICIAN) || (title ==
          MANAGER) || (title == SALESPERSON)));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return NO_EMPLOYEE;
  }

  int employees_with_same_title = 0;
  float cumulative_salaries = 0.0;
  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      break;
    }

    if ((read_employee.title == title) && (read_employee.id_number !=
        FREE_OFFICE)) {
      employees_with_same_title++;
      cumulative_salaries += read_employee.salary;
    }
  }

  if (employees_with_same_title == 0) {
    /* no employees match title parameter */

    return NO_EMPLOYEE;
  }

  return (cumulative_salaries / (float) employees_with_same_title);
} /* average_salary_by_title() */

/*
 * Searches file for employee specified by ID number parameter.
 */

employee_t find_employee_by_id(FILE *in_file_ptr, int id_number) {
  assert((in_file_ptr != NULL) && (id_number > 0));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return BAD_EMPLOYEE;
  }

  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      break;
    }

    if (read_employee.id_number == id_number) {
      return read_employee;
    }
  }
  return BAD_EMPLOYEE;
} /* find_employee_by_id() */

/*
 * Searches file for employee whos first/last names match parameters.
 */

employee_t find_employee_by_name(FILE *in_file_ptr, char *first_name,
                                 char *last_name) {
  assert((in_file_ptr != NULL) && (first_name != NULL) && (last_name != NULL));
  assert((strlen(first_name) < MAX_NAME_LEN) && (strlen(last_name) <
         MAX_NAME_LEN));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return BAD_EMPLOYEE;
  }

  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      break;
    }

    if ((strcmp(read_employee.first_name, first_name) == 0) &&
        (strcmp(read_employee.last_name, last_name) == 0)) {
      return read_employee;
    }
  }
  return BAD_EMPLOYEE;
} /* find_employee_by_name() */

/*
 * Finds employee specified by ID number, increases their salary with specified
 * parameter.
 */

int give_raise(FILE *file_ptr, int id_number, float salary_increase) {
  assert((file_ptr != NULL) && (id_number > 0));
  int returned_value = fseek(file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return NO_EMPLOYEE;
  }

  employee_t read_employee = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&read_employee, sizeof(employee_t), 1, file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      break;
    }

    if (read_employee.id_number == id_number) {
      read_employee.salary += salary_increase;
      if (read_employee.salary <= 0.0) {
        read_employee.salary -= salary_increase;
        return fire_employee(file_ptr, read_employee);
      }

      /* after salary is increase, file is rewinded so previous struct can */
      /* be overwritten */

      returned_value = fseek(file_ptr, -1 * sizeof(employee_t), SEEK_CUR);
      if (returned_value != FSEEK_SUCCESS) {
        break;
      }
      returned_value = fwrite(&read_employee, sizeof(employee_t), 1, file_ptr);
      if (returned_value != FREAD_FWRITE_SUCCESS) {
        break;
      }

      return OK;
    }
  }
  return NO_EMPLOYEE;
} /* give_raise() */

/*
 * Using the ID numbers of 2 employees, finds first available time in week
 * to schedule a meeting.
 */

int schedule_meeting(FILE *in_file_ptr, int id_number_1, int id_number_2) {
  assert((in_file_ptr != NULL) && (id_number_1 > 0) && (id_number_2 > 0));
  int returned_value = fseek(in_file_ptr, 0, SEEK_SET);
  if (returned_value != FSEEK_SUCCESS) {
    return NO_EMPLOYEE;
  }

  employee_t employee_1 = BAD_EMPLOYEE;
  while (true) {
    returned_value = fread(&employee_1, sizeof(employee_t), 1, in_file_ptr);
    if (returned_value != FREAD_FWRITE_SUCCESS) {
      return NO_EMPLOYEE;
    }

    if ((employee_1.id_number == id_number_1) || (employee_1.id_number ==
        id_number_2)) {
      if (employee_1.id_number == id_number_2) {
        /* changes value of id_number_2 if first employee matched the second */
        /* ID number rather than the first ID number */

        id_number_2 = id_number_1;
      }
      employee_t employee_2 = BAD_EMPLOYEE;
      while (true) {
        returned_value = fread(&employee_2, sizeof(employee_t), 1, in_file_ptr);
        if (returned_value != FREAD_FWRITE_SUCCESS) {
          return NO_EMPLOYEE;
        }

        if (employee_2.id_number == id_number_2) {
          for (int i = 0; i < N_DAYS; i++) {
            for (int j = 0; j < N_HOURS; j++) {
              if ((employee_1.schedule[i][j] == 'A') &&
                  (employee_2.schedule[i][j] == 'A')) {
                return ((ALIGN_DAY * i) + (j + ALIGN_HOURS));
              }
            }
          }
          return NO_OVERLAP;
        }
      }
    }
  }
  return NO_EMPLOYEE;
} /* schedule_meeting() */
