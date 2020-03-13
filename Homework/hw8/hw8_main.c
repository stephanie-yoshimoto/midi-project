/* Includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hw8.h"

#define MAX_FILE_NAME_LEN (50)
#define MAX_OP_TEXT_LEN (50)
#define RED(x) "\033[31m"x"\033[0m"

static void print_operation(operation_t *operation);
static void print_list(operation_t *list);

/* This function is used to run the different functions implemented in file
 * hw8.c. User input is used to determine which function to run and what input
 * is passed to it. Upon completion, the function returns zero.
 */

int main() {
  char output_file[MAX_FILE_NAME_LEN] = "";
  char op_text[MAX_OP_TEXT_LEN] = "";
  int line_num = 0;
  int op_num = 0;
  int status = 0;
  int choice = 0;
  operation_t *list_primary = NULL;
  operation_t *list_undo = NULL;
  operation_t *single_op = NULL;

  //  Disable buffering to suppress memory leaks
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  while (1) {
    printf("\nOPTIONS:\n"
           "0) Quit\n"
           "1) add_new_operation()\n"
           "2) get_nth_operation()\n"
           "3) undo_nth_operation()\n"
           "4) redo_n_operations()\n" 
           "5) free_list()\n"
           "6) list_len()\n"
           "7) doc_last_line()\n"
           "8) interleave_operations()\n"
           "9) write_document()\n"
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
        printf("\nEnter text for operation: ");
        fscanf(stdin, "\n%49s", op_text);
        printf("Enter line number: ");
        fscanf(stdin, "\n%d", &line_num);
        if (list_primary){
          single_op = add_new_operation(list_primary, op_text, line_num);
        }
        else {
          list_primary = add_new_operation(list_primary, op_text, line_num);
          single_op = list_primary;
        }
        printf("\nOperation returned: ");
        print_operation(list_primary);
        printf("\nPrimary list now resembles:\n");
        print_list(list_primary);
        break;
      case 2:
        printf("\nEnter operation number: ");
        fscanf(stdin, "\n%d", &op_num);
        single_op = get_nth_operation(list_primary, op_num);
        printf("\nOperation %d in primary list resembles:", op_num);
        print_operation(single_op);
        break;
      case 3:
        printf("\nEnter operation number: ");
        fscanf(stdin, "\n%d", &op_num);
        free_list(list_undo);
        list_undo = undo_nth_operation(list_primary, op_num);
        printf("\nPrimary list now resembles:\n");
        print_list(list_primary);
        printf("\nUndo list now resembles:\n");
        print_list(list_undo);
        break;
      case 4:
        printf("\nEnter number of operations: ");
        fscanf(stdin, "\n%d", &op_num);
        if (op_num == list_len(list_undo)){
          redo_n_operations(list_primary, list_undo, op_num);
          list_undo = NULL;
        }
        else {
          redo_n_operations(list_primary, list_undo, op_num);
        }
        printf("\nPrimary list now resembles:\n");
        print_list(list_primary);
        printf("\nUndo list now resembles:\n");
        print_list(list_undo);
        break;
      case 5:
        printf("\nEnter 1 to free primary list, or 2 to free undo list: ");
        fscanf(stdin, "\n%d", &op_num);
        if (op_num == 1){
          free_list(list_primary);
          list_primary = NULL;
          printf("\nPrimary list now resembles:\n");
          print_list(list_primary);
        }
        else if (op_num == 2){
          free_list(list_undo);
          list_undo = NULL;
          printf("\nUndo list now resembles:\n");
          print_list(list_undo);
        }
        break;
      case 6:
        printf("\nPrimary list resembles:\n");
        print_list(list_primary);
        printf("\nLength = %d", list_len(list_primary));
        printf("\nUndo list resembles:\n");
        print_list(list_undo);
        printf("\nLength = %d\n", list_len(list_undo));
        break;
      case 7:
        printf("\nPrimary list resembles:\n");
        print_list(list_primary);
        printf("\nLast operation in document: ");
        print_operation(doc_last_line(list_primary));
        break;
      case 8:
        printf("\nPrimary list resembles:\n");
        print_list(list_primary);
        printf("\nUndo list resembles:\n");
        print_list(list_undo);
        printf("\nInterleaving...");
        interleave_operations(list_primary, list_undo);
        list_undo = NULL;
        printf("\nPrimary list resembles:\n");
        print_list(list_primary);
        printf("\nUndo list resembles:\n");
        print_list(list_undo);
        break;
      case 9:
        printf("\nEnter file name: ");
        fscanf(stdin, "\n%49s", output_file);
        write_document(output_file, list_primary);
        printf("\nDocument written to %s", output_file);
        break;
      default:
        printf(RED("\nInvalid input! Try again...\n"));
        break;
    } /* switch (choice) */

  } /* while (1) */
  free_list(list_primary);
  free_list(list_undo);

  return 0;
} /* main() */

/*
 * Function to print a list of operations
 */

static void print_list(operation_t *list){
  for (; list; list = list->next_operation){
    print_operation(list);
  }
} /* print_list() */


/*
 * Function to print a single operation
 */

static void print_operation(operation_t *operation){
  if (operation){
    printf("(%d [%s]) ->\n", operation->line_num, operation->new_text);
  }
  else {
    printf("(NULL)\n");
  }
} /* print_operation() */
