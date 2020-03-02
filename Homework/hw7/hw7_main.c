/* Includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hw7.h"

/* Defines and global variables */

#define RED(x) "\033[31m"x"\033[0m"
#define MAX_FILE_NAME_LEN (100)

#define BOGUS_PASSWORD ((password_t){\
  .code_name = "BOGUS",\
  .passcode_name = "BOGUS",\
  .passcode_value = "BOGUS",\
  .next_member = "BOGUS"})

static void print_return_value(int, char *);
static password_t read_password_from_file(char *);
static int is_bogus(password_t);
static void print_password(password_t);
static password_t* find_password_ptr(password_t);

/* This function is used to run the different functions implemented in file
 * hw7.c. User input is used to determine which function to run and what input
 * is passed to it. Upon completion, the function returns zero.
 */

int main() {
  char input_file[MAX_FILE_NAME_LEN] = "";
  char password_file1[MAX_FILE_NAME_LEN] = "";
  char password_file2[MAX_FILE_NAME_LEN] = "";
  int choice = -1;
  int read_pass_ret = 0;
  int return_value = 0;
  int status = 0;

  while (1) {

    printf("\nOPTIONS:\n"
           "0) Quit\n"
           "1) read_passwords()\n"
           "2) connect_members()\n"
           "3) isolate_spy()\n"
           "4) send_message()\n"
           "Select a function: ");

    status = fscanf(stdin, "%d", &choice);
    /* To prevent infinite loop incase of non integer input. */
    while (getchar() != '\n') {
    }

    if (status <= 0) {
      printf(RED("\nInvalid input! Try again...\n"));
      continue;
    }
    if (choice == 0) {
      printf("\nGoodbye!\n\n");
      break;
    }

    switch (choice) {
      case 0:
        printf("\nGoodbye!\n\n");
        break;
      case 1:
        printf("\nEnter input file name: ");
        fscanf(stdin, "\n%s", input_file);
        read_pass_ret = read_passwords(input_file);
        print_return_value(read_pass_ret, "read_passwords");
        break;
      case 2:
        if (read_pass_ret == 0) {
          printf(RED("\nWARNING: No passwords read in. "
                 "Execute read_passwords() first.\n"));
        } else {
          connect_members();
          printf("\nMembers connected.\n");
        }
        break;
      case 3:
        if (read_pass_ret == 0) {
          printf(RED("\nWARNING: No passwords read in. "
                 "Execute read_passwords() first.\n"));
        }
        else {
          printf("Password representation:"
                 "code_name&passcode_name&passcode_value\n"
                 "Enter a file containing a password representation: ");
          fscanf(stdin, "\n%s", input_file);
          password_t pass = read_password_from_file(input_file);
          if (is_bogus(pass)) {
            break;
          }
          printf("Read the following information:\n");
          print_password(pass);

          password_t *potential_spy = find_password_ptr(pass);
          return_value = isolate_spy(potential_spy);
          print_return_value(return_value, "isolate_spy");
        }
        break;
      case 4:
        if (read_pass_ret == 0) {
          printf(RED("\nWARNING: No passwords read in. "
                 "Execute read_passwords() first.\n"));
        } else {
          printf("Password representation:"
                 "code_name&passcode_name&passcode_value\n"
                 "Enter first file containing a password representation: ");
          fscanf(stdin, "\n%s", password_file1);
          password_t password1 = read_password_from_file(password_file1);
          if (is_bogus(password1)) {
            break;
          }
          printf("Read the following information:\n");
          print_password(password1);

          printf("Password representation:"
                 "code_name&passcode_name&passcode_value\n"
                 "Enter second file containing a password representation: ");
          fscanf(stdin, "\n%s", password_file2);
          password_t password2 = read_password_from_file(password_file2);
          if (is_bogus(password2)) {
            break;
          }
          printf("Read the following information:\n");
          print_password(password2);

          password_t *password1_ptr = find_password_ptr(password1);
          password_t *password2_ptr = find_password_ptr(password2);
          return_value = send_message(password1_ptr, password2_ptr);
          print_return_value(return_value, "send_message");
        }
        break;
      default:
        printf(RED("\nInvalid input! Try again...\n"));
        break;
    } /* switch (choice) */

  } /* while (1) */

  return 0;
} /* main() */

/*
 * Function to print appropriate error codes.
 */

static void print_return_value(int val, char *function) {
  printf("Value returned by %s(): %d ", function, val);
  switch (val) {
    case NON_READABLE_FILE:
      printf("(NON_READABLE_FILE)\n");
      break;
    case NO_PASSWORDS:
      printf("(NO_PASSWORDS)\n");
      break;
    case BAD_RECORD:
      printf("(BAD_RECORD)\n");
      break;
    case TOO_MUCH_DATA:
      printf("(TOO_MUCH_DATA)\n");
      break;
    case NOT_A_SPY:
      printf("(NOT_A_SPY)\n");
      break;
    case MESSAGE_NOT_SENT:
      printf("(MESSAGE_NOT_SENT)\n");
      break;
    default:
      printf("\n");
      break;
  }
} /* print_return_value() */

/*
 * Function to read in password from a text file.
 */

static password_t read_password_from_file(char *file_name) {

#define MAX_LINES (4)

  FILE *fp = NULL;
  if ((fp = fopen(file_name, "r")) == NULL) {
    printf(RED("\nUnable to read %s for input.\n"), file_name);
    return BOGUS_PASSWORD;
  }

  password_t password = BOGUS_PASSWORD;
  char cn_buff[MAX_BUFF_LEN] = "";
  char pn_buff[MAX_BUFF_LEN] = "";
  char pv_buff[MAX_BUFF_LEN] = "";
  char nm_buff[MAX_BUFF_LEN] = "";
  int scan_ret = 0;
  if ((scan_ret = fscanf(fp, "%49[^&\n]&%49[^&\n]&%49[^&\n]&%49[^&\n]\n",
                            cn_buff, pn_buff, pv_buff, nm_buff)) < 4) {
    printf(RED("Invalid data present in file %s.\n"), file_name);
    fclose(fp);
    fp = NULL;
    return BOGUS_PASSWORD;
  }
  strncpy(password.code_name, cn_buff, MAX_NAME_LEN - 1);
  password.code_name[MAX_NAME_LEN - 1] = '\0';
  strncpy(password.passcode_name, pn_buff, MAX_NAME_LEN - 1);
  password.passcode_name[MAX_NAME_LEN - 1] = '\0';
  strncpy(password.passcode_value, pv_buff, MAX_NAME_LEN - 1);
  password.passcode_value[MAX_NAME_LEN - 1] = '\0';
  strncpy(password.next_member, nm_buff, MAX_NAME_LEN - 1);
  password.next_member[MAX_NAME_LEN - 1] = '\0';

  fclose(fp);
  fp = NULL;
  return password;
} /* read_password_from_file() */

/*
 * Function to check if password is bogus.
 */

static int is_bogus(password_t password) {
  if (!strcmp(password.code_name, "BOGUS")
      && !strcmp(password.passcode_name, "BOGUS")
      && !strcmp(password.passcode_value, "BOGUS")
      && !strcmp(password.next_member, "BOGUS")) {
    return 1;
  } else {
    return 0;
  }
} /* is_bogus() */

/*
 * Function to print the password read in.
 */

static void print_password(password_t password) {
  printf("\nCode Name: %s\nPasscode Name: %s"
         "\nPasscode Value: %s\nNext Member: %s\n\n",
         password.code_name, password.passcode_name,
         password.passcode_value, password.next_member);
} /* print_password() */

/*
 * Function to find the password pointer from array.
 */

static password_t* find_password_ptr(password_t password) {
  if (is_bogus(password)) {
    return NULL;
  }
  for (int count = 0; count < g_password_count; count++) {
    if (!strcmp(password.code_name, g_password_array[count].code_name)
        && !strcmp(password.passcode_name,
        g_password_array[count].passcode_name)
        && !strcmp(password.passcode_value,
        g_password_array[count].passcode_value)) {
      return &g_password_array[count];
    }
  }
  return NULL;
} /* find_password_ptr() */
