/* Stephanie Yoshimoto, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

#include "hw12.h"

#include <assert.h>
#include <malloc.h>
#include <malloc_debug.h>
#include <string.h>

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

void delete_cpu_data(void **node) {
  assert((node) && (*node));
  cpu_t *cpu = (cpu_t *)(*node);
  free(cpu->model);
  cpu->model = NULL;
  free(cpu->manufacturer);
  cpu->manufacturer = NULL;
  free(cpu);
  cpu = NULL;
} /* delete_cpu_data() */

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

void delete_memory_data(void **ptr) {
  assert((ptr) && (*ptr));
} /* delete_memory_data() */

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

  int speed_1 = memory_1->size * memory_1->speed * memory_1->ddr_gen;
  int speed_2 = memory_2->size * memory_2->speed * memory_2->ddr_gen;
  if (speed_1 < speed_2) {
    return -1;
  }
  else if (speed_1 > speed_2) {
    return 1;
  }
  else {
    return 0;
  }
} /* compare_memory_data() */

void create_node(struct node **node, void *data_structure,
                 void (*print)(void *data), void (*delete)(void **node),
                 int (*compare)(void *data_1, void *data_2)) {
  assert((node) && (*node == NULL) && (data_structure) && (print) &&
         (delete) && (compare));

  *node = malloc(sizeof(struct node));
  assert(*node);
  (*node)->left = NULL;
  (*node)->right = NULL;
  (*node)->print = print;
  (*node)->delete = delete;
  (*node)->compare = compare;
} /* create_node() */

void delete_node(struct node **root) {
  assert((root) && (*root) && ((*root)->left) && ((*root)->right));

} /* delete_node() */

void insert_node(struct node **root, struct node *insert_node) {
  assert((root) && (insert_node));
  if (!*root) {
    *root = insert_node;
  }
} /* insert_node() */

struct node **find_nodes(struct node *root, void *data_structure,
                         int *num_duplicates) {
  return NULL;
} /* find_nodes() */

void remove_node(struct node **root, struct node *remove_node) {

}

void delete_tree(struct node **root) {
  assert(root);
} /* delete_tree() */
