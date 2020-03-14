/* Stephanie Yoshimoto, hw8.c, CS 24000, Spring 2020
 * Last updated February 26, 2020
 */

#include "hw8.h"

#include <assert.h>
#include <malloc_debug.h>
#include <stdio.h>
/*#include <stdlib.h>*/
#include <string.h>

operation_t *add_new_operation(operation_t *list, char *new_text,
                               int line_num) {
  assert((new_text != NULL) && (line_num >= 0));
  operation_t *new_operation = malloc(sizeof(operation_t));
  assert(new_operation);/*
  char *text = malloc(strlen(new_text) + 1);
  assert(text);
  new_operation->line_num = line_num;
  new_operation->new_text = text;
  strcpy(new_operation->new_text, new_text);
  operation_t *temp = list;
  while (temp != NULL) {
    if (temp->next_operation == NULL) {
      temp->next_operation = new_operation;
      break;
    }
    else {
      temp = temp->next_operation;
    }
  }*/
  return new_operation;
} /* add_new_operation() */

int list_len(operation_t *list) {
  if (list == NULL) {
    return 0;
  }
  int num_operations = 0;
  while (list) {
    num_operations++;
    if (list->next_operation) {
      list = list->next_operation;
    }
    else {
      break;
    }
  }
  return num_operations;
} /* list_len() */

operation_t *get_nth_operation(operation_t *list, int nth_operation) {
  assert((list) && (nth_operation >= 0));
  int current_operation = 0;
  while (list) {
    if (current_operation == nth_operation) {
      return list;
    }

    if (!list->next_operation) {
      break;
    }
    else {
      list = list->next_operation;
      current_operation++;
    }
  }
  return NULL;
} /* get_nth_operation() */

operation_t *undo_nth_operation(operation_t *list, int nth_operation) {
  assert((list) && (nth_operation >= 0));
  /*int current_operation = 0;
  while (list) {
    printf("%d %s\n", list->line_num, list->new_text);*/
    /*if (current_operation == nth_operation - 1) {
      operation_t *prev = NULL;
      operation_t *next = NULL;
      operation_t *current = list;
      printf("\n%d %s\n", current->line_num, current->new_text);
      printf("%d %s\n", current->next_operation->line_num,
             current->next_operation->new_text);
      while (current) {
        next = current->next_operation;
        current->next_operation = prev;
        prev = current;
        current = next;
      }
      return prev;
    }

    current_operation++;
    if (!list->next_operation) {
      break;
    }
    else {
      list = list->next_operation;
    }
  }*/
  return NULL;
} /* undo_nth_operation() */

void redo_n_operations(operation_t *list_1, operation_t *list_2,
                       int operations) {
  assert((list_1) && (list_2) && (operations >= 0));
  /*operation_t *temp = list_2;
  int operations_list_2 = 0;
  while (temp) {
    operations_list_2++;
    temp = temp->next_operation;
  }
  assert(operations <= operations_list_2);

  while (list_1->next_operation) {
    list_1 = list_1->next_operation;
  }

  int index = operations_list_2 - operations;
  int current_position = 0;
  while (list_2) {
    if (current_position == index) {
      operation_t *prev = NULL;
      operation_t *next = NULL;
      operation_t *current = list_2;
      while (current) {
        next = current->next_operation;
        current->next_operation = prev;
        prev = current;
        current = next;
      }
      list_1->next_operation = prev;
      return;
    }

    current_position++;
    list_2 = list_2->next_operation;
  }*/
} /* redo_n_operations() */

void free_list(operation_t *list) {
  while (list) {
    operation_t *current = list;
    free(current->new_text);

    if (list->next_operation) {
      list = list->next_operation;
    }
    else {
      free(current);
      break;
    }

    free(current);
  }
} /* free_list() */

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

    if (temp->next_operation) {
      temp = temp->next_operation;
      current_operation++;
    }
    else {
      break;
    }
  }

  current_operation = 0;
  while (list) {
    if (current_operation == last_line) {
      return list;
    }

    if (list->next_operation) {
      list = list->next_operation;
      current_operation++;
    }
    else {
      break;
    }
  }
  return NULL;
} /* doc_last_line() */

operation_t *interleave_operations(operation_t *list_1, operation_t *list_2) {
  assert((list_1) && (list_2));
  operation_t *head = list_1;
  operation_t *temp = list_1;
  while (1) {
    if (list_2) {
      temp->next_operation = list_2;
      temp = temp->next_operation;
      list_2 = list_2->next_operation;
    }
    temp->next_operation = list_1;
    temp = temp->next_operation;

    if (list_2 == NULL) {
      break;
    }
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
  int operations_written = 0;
  operation_t *temp = list;
  while (temp) {
    int lines_read = 0;
    if (fseek(file_ptr_out, 0, SEEK_SET) == 0) {
      for (lines_read = 0; feof(file_ptr_out) == 0; lines_read++) {
        if (lines_read == temp->line_num) {
          fprintf(file_ptr_out, "%s\n", temp->new_text);
          operations_written++;
          break;
        }
        else {
          fprintf(file_ptr_out, "\n");
        }
      }

      if (lines_read < temp->line_num) {
        int returned_value = fseek(file_ptr_out, -1, SEEK_END);
        if (returned_value != 0) {
          fclose(file_ptr_out);
          file_ptr_out = NULL;
          return operations_written;
        }
        for (int i = lines_read; i <= temp->line_num; i++) {
          if (i == temp->line_num) {
            fprintf(file_ptr_out, "%s\n", temp->new_text);
            operations_written++;
            break;
          }
          else {
            fprintf(file_ptr_out, "\n");
          }
        }
      }
    }
    else {
      fclose(file_ptr_out);
      file_ptr_out = NULL;
      return operations_written;
    }
    temp = temp->next_operation;
  }
  fclose(file_ptr_out);
  file_ptr_out = NULL;
  return operations_written;
} /* write_document() */
