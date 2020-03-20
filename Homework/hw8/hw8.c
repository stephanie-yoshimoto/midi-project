/* Stephanie Yoshimoto, hw8.c, CS 24000, Spring 2020
 * Last updated February 26, 2020
 */

#include "hw8.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHARS (200)
#define MAX_FILE_LEN (5000)

/*
 * Allocates new operation struct and adds struct to end of list.
 */

operation_t *add_new_operation(operation_t *list, char *new_text,
                               int line_num) {
  assert((new_text != NULL) && (line_num >= 0));
  operation_t *new_operation = malloc(sizeof(operation_t));
  assert(new_operation);
  char *text = malloc(strlen(new_text) + 1);
  assert(text);
  new_operation->line_num = line_num;
  new_operation->new_text = text;
  strcpy(new_operation->new_text, new_text);
  new_operation->next_operation = NULL;
  while (list) {
    if (list->next_operation == NULL) {
      list->next_operation = new_operation;
      break;
    }
    else {
      list = list->next_operation;
    }
  }
  return new_operation;
} /* add_new_operation() */

/*
 * Traverses list to find amount of operations in list.
 */

int list_len(operation_t *list) {
  if (list == NULL) {
    return 0;
  }
  int num_operations = 0;
  while (list) {
    num_operations++;
    list = list->next_operation;
  }
  return num_operations;
} /* list_len() */

/*
 * Finds operation in list at nth index.
 */

operation_t *get_nth_operation(operation_t *list, int nth_operation) {
  assert((list) && (nth_operation >= 0));
  int current_operation = 0;
  while (list) {
    if (current_operation == nth_operation) {
      return list;
    }

    list = list->next_operation;
    current_operation++;
  }
  return NULL;
} /* get_nth_operation() */

/*
 * Reverses list at nth index to undo operations.
 */

operation_t *undo_nth_operation(operation_t *list, int nth_operation) {
  assert((list) && (nth_operation >= 0));
  int current_operation = 1;
  operation_t *head = list;
  while (list) {
    if ((nth_operation == 0) && (head->next_operation == NULL)) {
      return head;
    }
    else if ((current_operation == nth_operation) || (nth_operation == 0)) {
      operation_t *prev = NULL;
      operation_t *current = list;
      while (current) {
        operation_t *next = current->next_operation;
        current->next_operation = prev;
        prev = current;
        current = next;
      }

      operation_t *temp = prev;
      if (nth_operation != 0) {
        while (prev) {
          if (!prev->next_operation->next_operation) {
            prev->next_operation = NULL;
            break;
          }
          else {
            prev = prev->next_operation;
          }
        }
      }
      return temp;
    }

    current_operation++;
    list = list->next_operation;
  }
  return NULL;
} /* undo_nth_operation() */

/*
 * Appends operations from second list onto first, removes operations from
 * second list.
 */

void redo_n_operations(operation_t *list_1, operation_t *list_2,
                       int operations) {
  assert((list_1) && (list_2) && (operations >= 0));
  operation_t *temp = list_2;
  int operations_list_2 = 0;
  while (temp) {
    /* count operations in list 2 */

    operations_list_2++;
    temp = temp->next_operation;
  }
  assert(operations <= operations_list_2);

  while (list_1->next_operation) {
    /* traverse until end of list 1 to add operations onto */

    list_1 = list_1->next_operation;
  }

  int current_position = 1;
  while (list_2) {
    int nth_operation = operations_list_2 - operations;
    if ((operations == 1) && (operations_list_2 == 1)) {
      list_1->next_operation = list_2;
      list_2->next_operation = NULL;
      return;
    }
    else if (current_position == nth_operation) {
      operation_t *prev = NULL;
      operation_t *current = list_2->next_operation;
      list_2->next_operation = NULL;
      while (current) {
        operation_t *next = current->next_operation;
        current->next_operation = prev;
        prev = current;
        current = next;
      }
      list_1->next_operation = prev;
      return;
    }

    current_position++;
    list_2 = list_2->next_operation;
  }
} /* redo_n_operations() */

/*
 * Frees operations in given list.
 */

void free_list(operation_t *list) {
  while (list) {
    operation_t *current = list;
    free(current->new_text);
    current->new_text = NULL;
    list = list->next_operation;
    free(current);
    current = NULL;
  }
  list = NULL;
} /* free_list() */

/*
 * Returns a pointer to last operation in input list.
 */

operation_t *doc_last_line(operation_t *list) {
  assert(list);
  int current_operation = 0;
  int last_line = 0;
  int greatest = 0;
  operation_t *temp = list;
  while (temp) {
    if (temp->line_num > greatest) {
      greatest = temp->line_num;
      last_line = current_operation;
    }

    temp = temp->next_operation;
    current_operation++;
  }

  current_operation = 0;
  while (list) {
    if (current_operation == last_line) {
      return list;
    }

    list = list->next_operation;
    current_operation++;
  }
  return NULL;
} /* doc_last_line() */

operation_t *interleave_operations(operation_t *list_1, operation_t *list_2) {
  assert((list_1) && (list_2));
  operation_t *head = list_1;
  while ((list_1) && (list_2)) {
    operation_t *temp_1 = list_1->next_operation;
    operation_t *temp_2 = list_2->next_operation;

    list_1->next_operation = list_2;
    if (temp_1) {
      list_2->next_operation = temp_1;
    }

    list_1 = temp_1;
    list_2 = temp_2;
  }
  return head;
} /* interleave_operations() */

int write_document(char *file_name, operation_t *list) {
  assert((file_name) && (list));
  FILE *file_ptr_out = NULL;
  file_ptr_out = fopen(file_name, "w");
  if (!file_ptr_out) {
    return NON_WRITABLE_FILE;
  }

  int lines_in_file = 0;
  operation_t *temp = list;
  while (list) {
    if (list->line_num > lines_in_file) {
      lines_in_file = list->line_num;
    }
    list = list->next_operation;
  }

  operation_t *sorted_list[lines_in_file];
  for (int i = 0; i < lines_in_file; i++) {
    sorted_list[i] = NULL;
  }

  while (temp) {
    sorted_list[temp->line_num - 1] = temp;
    temp = temp->next_operation;
  }

  char file_contents[MAX_FILE_LEN] = "";
  int operations_written = 0;
  for (int i = 0; i < lines_in_file; i++) {
    if (!sorted_list[i]) {
      strcat(file_contents, "\n");
      continue;
    }

    strcat(file_contents, "\n");
    strcat(file_contents, (sorted_list[i])->new_text);
    operations_written++;
    strcat(file_contents, "\n");
  }

  fprintf(file_ptr_out, "%s", file_contents);
  fclose(file_ptr_out);
  file_ptr_out = NULL;
  return operations_written;
} /* write_document() */

/*int main() {
  operation_t *temp = malloc(sizeof(operation_t));
  operation_t *temp_2 = malloc(sizeof(operation_t));
  temp->line_num = 2;
  temp_2->line_num = 3;
  char *hello = "hello";
  char *hello2 = "hello2";
  temp->new_text = malloc(sizeof(strlen(hello) + 1));
  strcpy(temp->new_text, hello);
  temp_2->new_text = malloc(sizeof(strlen(hello) + 1));
  strcpy(temp_2->new_text, hello2);
  temp->next_operation = temp_2;
  temp_2->next_operation = NULL;
  printf("%d\n", write_document("output.txt", temp));
  free(temp_2->new_text);
  free(temp_2);
  free(temp->new_text);
  free(temp);
  return 0;
}*/
