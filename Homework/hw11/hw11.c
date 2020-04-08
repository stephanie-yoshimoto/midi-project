/* Stephanie Yoshimoto, hw11.c, CS 24000, Spring 2020
 * Last updated April 04, 2020
 */

#include "hw11.h"

#include <assert.h>
#include <malloc.h>
#include <malloc_debug.h>

/*
 * Creates and returns new tree node with given hash value.
 */

tree_node_t *create_node(int hash_value) {
  assert(hash_value > 0);

  tree_node_t *new_node = NULL;
  new_node = malloc(sizeof(tree_node_t));
  assert(new_node);
  new_node->hash_value = hash_value;
  new_node->left_child_ptr = NULL;
  new_node->right_child_ptr = NULL;

  return new_node;
} /* create_node() */

/*
 * Inserts new node below parent node, freeing any part of tree that was
 * there previously.
 */

void insert_node(tree_node_t **parent_node, tree_node_t *new_node,
                 int child_type) {
  assert((parent_node) && (new_node) && ((child_type == 0) ||
         (child_type == 1)));

  if (!(*parent_node)) {
    *parent_node = new_node;
  }
  else if (child_type == 0) {
    tree_node_t *left_branch = (*parent_node)->left_child_ptr;
    (*parent_node)->left_child_ptr = new_node;
    if (left_branch) {
      delete_tree(left_branch);
    }
  }
  else if (child_type == 1) {
    tree_node_t *right_branch = (*parent_node)->right_child_ptr;
    (*parent_node)->right_child_ptr = new_node;
    if (right_branch) {
      delete_tree(right_branch);
    }
  }
} /* insert_node() */

/*
 * Searches for node with given hash value, and if found, returns its pointer.
 */

tree_node_t *search_node(tree_node_t *root_node, int hash_value) {
  assert(hash_value > 0);

  if (!root_node) {
    return NULL;
  }
  else if (root_node->hash_value == hash_value) {
    return root_node;
  }
  else {
    tree_node_t *found = search_node(root_node->left_child_ptr, hash_value);
    if (!found) {
      found = search_node(root_node->right_child_ptr, hash_value);
    }
    else {
      return found;
    }
  }
  return NULL;
} /* search_node() */

/*
 * Deletes/frees all nodes in tree.
 */

void delete_tree(tree_node_t *root_node) {
  if (!root_node) {
    return;
  }
  else {
    delete_tree(root_node->left_child_ptr);
    delete_tree(root_node->right_child_ptr);
    free(root_node);
    root_node = NULL;
  }
} /* delete_tree() */

hash_list_t *get_hash_list_prefix(tree_node_t *root_node) {
  assert(root_node);

  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;
  hash_list_t *hash_list_copy = new_hash_list;
  printf("here\n");
  get_hash_list_prefix(root_node->left_child_ptr);
  get_hash_list_prefix(root_node->right_child_ptr);
  return hash_list_copy;
} /* get_hash_list_prefix() */

hash_list_t *get_hash_list_postfix(tree_node_t *root_node) {
  assert(root_node);

  get_hash_list_postfix(root_node->left_child_ptr);
  get_hash_list_postfix(root_node->right_child_ptr);
  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;
  return new_hash_list;
} /* get_hash_list_postfix() */

hash_list_t *get_hash_list_forward(tree_node_t *root_node) {
  assert(root_node);

  get_hash_list_forward(root_node->left_child_ptr);
  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;
  get_hash_list_forward(root_node->right_child_ptr);
  return new_hash_list;
} /* get_hash_list_forward() */

hash_list_t *get_hash_list_reverse(tree_node_t *root_node) {
  assert(root_node);

  get_hash_list_forward(root_node->right_child_ptr);
  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;
  get_hash_list_forward(root_node->left_child_ptr);
  return new_hash_list;
} /* get_hash_list_reverse() */

void delete_hash_list(hash_list_t *hash_list) {
  if (hash_list) {
    hash_list_t *temp_list = hash_list->next_hash_ptr;
    free(hash_list);
    hash_list = NULL;
    delete_hash_list(temp_list);
  }
} /* delete_hash_list() */
