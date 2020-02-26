#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw6.h"


#define NUM_SPIES (1)

void print_return_value(int, char *);

/* This function is used to run the different functions implemented in file
 * hw6.c. User input is used to determine which function to run and what input
 * is passed to it. Upon completion, the function returns zero.
 */

int main() {
  char input_file[MAX_NAME_LEN];
  char **potential_spies = malloc(sizeof(char *));
  potential_spies[0] = malloc(MAX_NAME_LEN);
  int choice = -1;
  int return_value = 0;
  int status = 0;

  while (1) {

    printf("\nOPTIONS:\n"
           "0) Quit\n"
           "1) read_passwords()\n"
           "2) locate_spies()\n"
           "Select a function: ");

    status = fscanf(stdin, "%d", &choice);
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

    switch (choice) {
      case 0:
        printf("\nGoodbye!\n\n");
        break;
      case 1:
        printf("\nEnter input file name: ");
        fscanf(stdin, "%s", input_file);
        return_value = read_passwords(input_file);
        print_return_value(return_value, "read_passwords");
        break;
      case 2:
        printf("\nEnter the name of a potential spy: ");
        fscanf(stdin, "%9[^\n]", potential_spies[0]);
        return_value = locate_spies(potential_spies, NUM_SPIES);
        print_return_value(return_value, "locate_spies");
        break;
      default:
        printf("\nInvalid selection.\n");
        break;
    }
  }

  free(potential_spies[0]);
  free(potential_spies);
  return 0;
} /* main() */

/*
 * Function to print appropriate error codes.
 */

void print_return_value(int val, char *function) {
  printf("Value returned by %s(): %d ", function, val);
  switch(val) {
    case NON_READABLE_FILE:
      printf("(NON_READABLE_FILE)\n");
      break;
    case BAD_RECORD:
      printf("(BAD_RECORD)\n");
      break;
    case TOO_MUCH_DATA:
      printf("(TOO_MUCH_DATA)\n");
      break;
    default:
      printf("\n");
      break;
  }
} /* print_return_value() */
