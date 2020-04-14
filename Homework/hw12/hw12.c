/* Stephanie Yoshimoto, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

#include "hw12.h"

#include <assert.h>
#include <malloc.h>
#include <malloc_debug.h>
#include <string.h>

/*
 * Creates a CPU node, sets values according to parameters.
 */

void create_cpu_data(void **ptr, const char *model, const char *manufacturer,
                     int speed, int cores) {
  assert((ptr) && (*ptr == NULL) && (model) && (manufacturer));
  cpu_t *cpu = (cpu_t *)(*ptr);
  cpu = NULL;
  cpu = malloc(sizeof(cpu_t));
  assert(cpu);
  cpu->model = malloc(strlen(model) + 1);
  assert(cpu->model);
  strcpy(cpu->model, model);
  cpu->manufacturer = malloc(strlen(manufacturer) + 1);
  assert(cpu->manufacturer);
  strcpy(cpu->manufacturer, manufacturer);
  cpu->speed = speed;
  cpu->cores = cores;
  *ptr = cpu;
} /* create_cpu_data() */

/*
 * Frees all data associated with CPU node.
 */

void delete_cpu_data(void **data) {
  assert((data) && (*data));
  cpu_t *cpu = (cpu_t *)(*data);
  free(cpu->model);
  cpu->model = NULL;
  free(cpu->manufacturer);
  cpu->manufacturer = NULL;
  free(cpu);
  cpu = NULL;
  *data = NULL;
} /* delete_cpu_data() */

/*
 * Compares overall speed of CPUs, returns values similar to those of strcmp.
 */

int compare_cpu_data(void *data_1, void *data_2) {
  assert((data_1) && (data_2));
  cpu_t *cpu_1 = (cpu_t *)(data_1);
  int speed_1 = cpu_1->speed * cpu_1->cores;
  cpu_t *cpu_2 = (cpu_t *)(data_2);
  int speed_2 = cpu_2->speed * cpu_2->cores;
  if (speed_1 < speed_2) {
    return -1;
  }
  else if (speed_1 > speed_2) {
    return 1;
  }
  else {
    return 0;
  }
} /* compare_cpu_data() */

/*
 * Creates a memory node, sets values according to parameters.
 */

void create_memory_data(void **ptr, const char *model,
                        const char *manufacturer, int size, int speed,
                        int ddr_gen) {
  assert((ptr) && (*ptr == NULL) && (model) && (manufacturer));
  memory_t *memory = (memory_t *)(*ptr);
  memory = NULL;
  memory = malloc(sizeof(memory_t));
  assert(memory);
  memory->model = malloc(strlen(model) + 1);
  assert(memory->model);
  strcpy(memory->model, model);
  memory->manufacturer = malloc(strlen(manufacturer) + 1);
  assert(memory->manufacturer);
  strcpy(memory->manufacturer, manufacturer);
  memory->size = size;
  memory->speed = speed;
  memory->ddr_gen = ddr_gen;
  *ptr = memory;
} /* create_memory_data() */

/*
 * Frees all data associated with memory node.
 */

void delete_memory_data(void **data) {
  assert((data) && (*data));
  memory_t *memory = (memory_t *)(*data);
  free(memory->model);
  memory->model = NULL;
  free(memory->manufacturer);
  memory->manufacturer = NULL;
  free(memory);
  memory = NULL;
  *data = NULL;
} /* delete_memory_data() */

/*
 * Compares overall memory data of node, returns values similar to those of
 * strcmp.
 */

int compare_memory_data(void *data_1, void *data_2) {
  assert((data_1) && (data_2));
  memory_t *memory_1 = (memory_t *)(data_1);
  memory_t *memory_2 = (memory_t *)(data_2);
  int model_cmp = strcmp(memory_1->model, memory_2->model);
  if (model_cmp > 0) {
    return 1;
  }
  else if (model_cmp < 0) {
    return -1;
  }
  int manufacturer_cmp = strcmp(memory_1->manufacturer,
                                memory_2->manufacturer);
  if (manufacturer_cmp > 0) {
    return 1;
  }
  else if (manufacturer_cmp < 0) {
    return -1;
  }
  else {
    return 0;
  }
} /* compare_memory_data() */

/*
 * Dynamically allocates memory for new node, assigns values accordingly.
 */

void create_node(struct node **node, void *data_structure,
                 void (*print)(void *data), void (*delete)(void **node),
                 int (*compare)(void *data_1, void *data_2)) {
  assert((node) && (*node == NULL) && (data_structure) && (print) &&
         (delete) && (compare));

  *node = malloc(sizeof(struct node));
  assert(*node);
  (*node)->left = NULL;
  (*node)->right = NULL;
  (*node)->data = data_structure;
  (*node)->print = print;
  (*node)->delete = delete;
  (*node)->compare = compare;
} /* create_node() */

/*
 * Frees all memory associated with node with callback function.
 */

void delete_node(struct node **root) {
  assert((root) && (*root) && ((*root)->left == NULL) &&
         ((*root)->right == NULL));
  void (*delete)(void **root) = (*root)->delete;
  delete(&(*root)->data);
  free(*root);
  *root = NULL;
} /* delete_node() */

void insert_node(struct node **root, struct node *new_node) {
  assert((root) && (new_node));
  if (!*root) {
    *root = new_node;
    return;
  }

  int result = (*root)->compare(new_node->data, (*root)->data);
  if (result <= 0) {
    if (!(*root)->left) {
      (*root)->left = new_node;
      return;
    }
    insert_node(&(*root)->left, new_node);
  }
  else if (result > 0) {
    if (!(*root)->right) {
      (*root)->right = new_node;
      return;
    }
    insert_node(&(*root)->right, new_node);
  }
} /* insert_node() */

void search_tree(struct node *root, void *data_structure, int *num_duplicates) {
  if (!root) {
    return;
  }

  int result = root->compare(data_structure, root->data);
  if (result <= 0) {
    if (result == 0) {
      (*num_duplicates)++;
    }

    search_tree(root->left, data_structure, num_duplicates);
  }
  else if (result > 0) {
    search_tree(root->right, data_structure, num_duplicates);
  }
} /* search_tree() */

struct node **find_nodes(struct node *root, void *data_structure,
                         int *num_duplicates) {
  assert((root) && (data_structure) && (num_duplicates));
  struct node **array_of_dups = NULL;
  search_tree(root, data_structure, num_duplicates);
  (*num_duplicates)--;
  array_of_dups = malloc(sizeof(struct node) * *num_duplicates);
  assert(*array_of_dups);


  return array_of_dups;
} /* find_nodes() */

void remove_node(struct node **root, struct node *remove_node) {
  assert((root) && (remove_node));

}

void delete_tree(struct node **root) {
  assert(root);
  if (!*root) {
    return;
  }

  if ((*root)->left) {
    delete_tree(&(*root)->left);
  }
  if ((*root)->right) {
    delete_tree(&(*root)->right);
  }
  delete_node(root);
  *root = NULL;
} /* delete_tree() */
