#include <string.h>
#include <stdio.h>

#include "hw5.h"

void print_employee(employee_t emp);
employee_t read_employee_from_file(char *);
void print_return_value(int, char *);

/* This function is used to run the different functions implemented in file
 * hw5.c. User input is used to determine which function to run and what input
 * is passed to it. Upon completion, the function returns zero.
 */

int main() {
  char database_file[100] = "";
  FILE * database_fp = 0;
  char emp_file[100] = "";
  char first_name[MAX_NAME_LEN] = "";
  char last_name[MAX_NAME_LEN] = "";
  int return_code = 0;
  float min_sal = 0;
  int emp_id = 0;
  int title = 0;
  float salary_delta = 0;
  int emp_id1 = 0;
  int emp_id2 = 0;
  employee_t emp = {FREE_OFFICE, "", "", -1, -1.0, {{'A'}}};

  printf("\nEnter the name of an input file to use: ");
  fgets(database_file, sizeof(database_file), stdin);
  *(strchr(database_file, '\n')) = '\0';

  database_fp = fopen(database_file, "rb+");
  if (database_fp == NULL) {
    printf("Unable to open %s for input.\n", database_file);
    return 1;
  }

  while (1) {

    printf("\nOPTIONS:\n"
           "0) Quit\n"
           "1) read_employee()\n"
           "2) write_employee()\n"
           "3) hire_employee()\n"
           "4) fire_employee()\n"
           "5) percent_occupancy()\n"
           "6) average_salary_by_title()\n"
           "7) find_employee_by_id()\n"
           "8) find_employee_by_name()\n"
           "9) give_raise()\n"
           "10) schedule_meeting()\n"
           "Select a function: ");
    int choice = 0;
    int status = fscanf(stdin, "%d", &choice);
    /* To prevent infinite loop incase of non integer input. */
    while (getchar() != '\n'){

    }
    if (status <= 0) {
      printf("\nInvalid input! Try again...\n");
      continue;
    }
    if (choice == 0) {
      printf("\nGoodbye!\n\n");
      break;
    }

    int record_num = 0;
    switch (choice) {
      case 0:
        printf("\nGoodbye!\n\n");
        break;
      case 1:
        printf("\nEnter employee record number: ");
        fscanf(stdin, "%d", &record_num);
        emp = read_employee(database_fp, record_num);
        print_employee(emp);
        break;
      case 2:
        printf("\nEnter employee record number: ");
        fscanf(stdin, "%d", &record_num);
        printf("Enter a file containing an employee representation: ");
        fscanf(stdin, "%s", emp_file);
        emp = read_employee_from_file(emp_file);
        printf("Read the following information:\n");
        print_employee(emp);
        return_code = write_employee(database_fp, emp, record_num);
        print_return_value(return_code, "write_employee");
        break;
      case 3:
        printf("\nEnter a file containing an employee representation: ");
        fscanf(stdin, "%s", emp_file);
        emp = read_employee_from_file(emp_file);
        printf("Read the following information:\n");
        print_employee(emp);
        return_code = hire_employee(database_fp, emp);
        print_return_value(return_code, "hire_employee");
        break;
      case 4:
        printf("\nEnter a file containing an employee representation: ");
        fscanf(stdin, "%s", emp_file);
        emp = read_employee_from_file(emp_file);
        printf("Read the following information:\n");
        print_employee(emp);
        return_code = fire_employee(database_fp, emp);
        print_return_value(return_code, "fire_employee");
        break;
      case 5:
        printf("\nEnter a minimum salary: ");
        fscanf(stdin, "%f", &min_sal);
        float percent = percent_occupancy(database_fp, min_sal);
        printf("Office is %f percent occupied.", percent);
        break;
      case 6:
        printf("\nEnter a title (as an integer): ");
        fscanf(stdin, "%d", &title);
        float salary = average_salary_by_title(database_fp, title);
        printf("Average salary: %f.", salary);
        break;
      case 7:
        printf("\nEnter an employee ID: ");
        fscanf(stdin, "%d", &emp_id);
        emp = find_employee_by_id(database_fp, emp_id);
        print_employee(emp);
        break;
      case 8:
        printf("\nEnter a first name: ");
        fscanf(stdin, "\n%39[^\n]", first_name);
        printf("Enter a last name: ");
        fscanf(stdin, "\n%39[^\n]", last_name);
        emp = find_employee_by_name(database_fp, first_name, last_name);
        print_employee(emp);
        break;
      case 9:
        printf("\nEnter an employee ID: ");
        fscanf(stdin, "%d", &emp_id);
        printf("Enter change in salary: ");
        fscanf(stdin, "%f", &salary_delta);
        return_code = give_raise(database_fp, emp_id, salary_delta);
        print_return_value(return_code, "give_raise");
        break;
      case 10:
        printf("\nEnter an employee ID: ");
        fscanf(stdin, "%d", &emp_id1);
        printf("Enter an employee ID: ");
        fscanf(stdin, "%d", &emp_id2);
        return_code = schedule_meeting(database_fp, emp_id1, emp_id2);
        printf("\nschedule_meeting returned %d.\n", return_code);
        break;
      default:
        printf("\nInvalid selection.\n");
        break;
    }
  }

  return 0;
} /* main() */


/*
 * Function to read in information about an employee
 */

employee_t read_employee_from_file(char *file_name) {
  FILE *fp = fopen(file_name, "r");
  employee_t emp = BAD_EMPLOYEE;
  if (fp == NULL){
    return emp;
  }
  if (fscanf(fp, "%d\n", &emp.id_number) != 1){
    return emp;
  }
  if (fscanf(fp, "%39[^\n]\n", emp.first_name) != 1){
    return emp;
  }
  if (fscanf(fp, "%39[^\n]\n", emp.last_name) != 1){
    return emp;
  }
  if (fscanf(fp, "%d\n", ((int *)&emp.title)) != 1){
    return emp;
  }
  if (fscanf(fp, "%f\n", &emp.salary) != 1){
    return emp;
  }
  for (int i = 0; i < N_DAYS; i++){
    for (int j = 0; j < N_HOURS; j++){
      if (fscanf(fp, "%c", &emp.schedule[i][j]) != 1){
        return emp;
      }
    }
    if (fscanf(fp, "\n") != 0){
      return emp;
    }
  }
  return emp;
} /* read_employee_from_file() */


/*
 * Function to print employee information
 */

void print_employee(employee_t emp) {
  printf("\nID: %d\nFirst Name: %s\nLast Name: %s\n", emp.id_number,
         emp.first_name, emp.last_name);
  switch (emp.title){
    case TECHNICIAN:
      printf("Title: Technician\n");
      break;
    case MANAGER:
      printf("Title: Manager\n");
      break;
    case SALESPERSON:
      printf("Title: Salesperson\n");
      break;
    default:
      printf("Title: INVALID\n");
      break;
  }
  printf("Salary: %.2f\n", emp.salary);
  printf("Schedule:\n");
  for (int i = 0; i < N_DAYS; i++){
    for (int j = 0; j < N_HOURS; j++){
      printf("%c", emp.schedule[i][j]);
    }
    printf("\n");
  }
} /* print_employee() */


/*
 * Function to print appropriate error codes.
 */

void print_return_value(int val, char *function) {
  printf("Value returned by %s(): %d ", function, val);
  switch(val) {
    case OK:
      printf("(OK)\n");
      break;
    case NO_EMPLOYEE:
      printf("(NO_EMPLOYEE)\n");
      break;
    case NO_OVERLAP:
      printf("(NO_OVERLAP)\n");
      break;
    case WRITE_ERR:
      printf("(WRITE_ERR)\n");
      break;
    case NO_OFFICE:
      printf("(NO_OFFICE)\n");
      break;
    default:
      printf("\n");
      break;
  }
} /* print_return_value() */
