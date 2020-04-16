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
      search_tree(root->left, data_structure, num_duplicates);
      search_tree(root->right, data_structure, num_duplicates);
      return;
    }
    else {
      search_tree(root->left, data_structure, num_duplicates);
    }
  }
  else if (result > 0) {
    search_tree(root->right, data_structure, num_duplicates);
  }
} /* search_tree() */

void populate_array(struct node *root, void *data_structure,
                    struct node **array_of_dups, int length) {
  if (!root) {
    return;
  }

  int result = root->compare(data_structure, root->data);
  if (result <= 0) {
    if (result == 0) {
      for (int i = 0; i < length; i++) {
        if (array_of_dups[i] == NULL) {
          array_of_dups[i] = root;
          break;
        }
      }
      populate_array(root->left, data_structure, array_of_dups, length);
      populate_array(root->right, data_structure, array_of_dups, length);
      return;
    }
    else {
      populate_array(root->left, data_structure, array_of_dups, length);
    }
  }
  else if (result > 0) {
    populate_array(root->right, data_structure, array_of_dups, length);
  }
} /* populate_array() */

struct node **find_nodes(struct node *root, void *data_structure,
                         int *num_duplicates) {
  assert((root) && (data_structure) && (num_duplicates));
  struct node *root_copy = root;
  *num_duplicates = 1;
  search_tree(root, data_structure, num_duplicates);
  (*num_duplicates)--;
  if (*num_duplicates == 0) {
    return NULL;
  }

  struct node **array_of_dups = NULL;
  array_of_dups = malloc(sizeof(struct node *) * *num_duplicates);
  assert(array_of_dups);
  for (int i = 0; i < *num_duplicates; i++) {
    array_of_dups[i] = NULL;
  }
  populate_array(root_copy, data_structure, array_of_dups, *num_duplicates);
  return array_of_dups;
} /* find_nodes() */

struct node *find_parent_node(struct node **root, struct node *node) {
  if (!*root) {
    return NULL;
  }

  if ((*root)->left == node) {
    return *root;
  }
  else if ((*root)->right == node) {
    return *root;
  }
  else {
    int result = (*root)->compare(node->data, (*root)->data);
    if (result <= 0) {
      if ((*root)->left) {
        return find_parent_node(&(*root)->left, node);
      }
    }
    else if (result > 0) {
      if ((*root)->right) {
        return find_parent_node(&(*root)->right, node);
      }
    }
  }
  return NULL;
} /* find_parent_node() */

void printInorder(struct node** node)
{
     if (*node == NULL)
          return;

     printInorder(&(*node)->left);
     printf("%p\n", *node);
     printInorder(&(*node)->right);
}

void remove_node(struct node **root, struct node *node) {
  assert((root) && (node));
  if (!*root) {
    return;
  }
printf("anotha one\n");
  if (node == *root) {
    struct node *remove = *root;
    if (!(*root)->left) {
      *root = (*root)->right;
    }
    else if (!(*root)->right) {
      *root = (*root)->left;
    }
    else {
      struct node *right_branch = node->right;
      *root = (*root)->left;
      struct node *to_delete = node;
      node = *root;
      while (node) {
        if (!node->right) {
          break;
        }
        node = node->right;
      }
      node->right = right_branch;
      node = to_delete;
    }
    remove->left = NULL;
    remove->right = NULL;
    delete_node(&remove);
    return;
  }

  printInorder(root);
  struct node *found_node = find_parent_node(root, node);
  *root = found_node;
  if (found_node->left == node) {
    printf("left\n");
    struct node *right_branch = node->right;
    if (node->left) {
      found_node->left = node->left;
      if (right_branch) {
        struct node *temp = found_node->left;
        while (temp) {
          if (!temp->right) {
            break;
          }
          temp = temp->right;
        }
        temp->right = right_branch;
      }
    }
    else {
      found_node->right = right_branch;
    }
  }
  else {
    /* found node's right child is node to delete */

    printf("right\n");
    struct node *left_branch = node->left;
    if (node->right) {
      found_node->right = node->right;
      if (left_branch) {
        struct node *temp = found_node->right;
        while (temp) {
          if (!temp->left) {
            break;
          }
          temp = temp->left;
        }
        temp->left = left_branch;
      }
    }
    else {
      found_node->left = left_branch;
    }
  }
  node->left = NULL;
  node->right = NULL;
  found_node = node;
  found_node->right = NULL;
  delete_node(&found_node);
  printInorder(root);
} /* remove_node() */

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
