#ifndef HW8_H
#define HW8_H

/* Error codes */
#define NON_WRITABLE_FILE (-1)

typedef struct operation {
  int line_num;
  char *new_text;

  struct operation *next_operation;
} operation_t;

/* Function prototypes */
operation_t *add_new_operation(operation_t *, char *, int);
operation_t *get_nth_operation(operation_t *, int);
operation_t *undo_nth_operation(operation_t *, int);
void redo_n_operations(operation_t *, operation_t *, int);
void free_list(operation_t *);
int list_len(operation_t *);
operation_t *doc_last_line(operation_t *);
operation_t *interleave_operations(operation_t *, operation_t *);
int write_document(char *, operation_t *);

#endif // HW8_H
