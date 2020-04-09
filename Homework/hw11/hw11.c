/* Stephanie Yoshimoto, hw11.c, CS 24000, Spring 2020
 * Last updated April 04, 2020
 */

#include "hw11.h"

#include <assert.h>
#include <malloc.h>

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
    tree_node_t *found_left = search_node(root_node->left_child_ptr,
                                          hash_value);
    tree_node_t *found_right = search_node(root_node->right_child_ptr,
                                           hash_value);
    if (found_left) {
      return found_left;
    }
    else {
      return found_right;
    }
  }
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

/*
 * Creates a hash list of binary tree in preorder traversal.
 */

hash_list_t *get_hash_list_prefix(tree_node_t *root_node) {
  assert(root_node);

  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;

  hash_list_t *left = NULL;
  if (root_node->left_child_ptr) {
    left = get_hash_list_prefix(root_node->left_child_ptr);
  }
  hash_list_t *right = NULL;
  if (root_node->right_child_ptr) {
    right = get_hash_list_prefix(root_node->right_child_ptr);
  }

  new_hash_list->next_hash_ptr = left;
  if (left) {
    while (left->next_hash_ptr) {
      left = left->next_hash_ptr;
    }
    left->next_hash_ptr = right;
  }
  else {
    new_hash_list->next_hash_ptr = right;
  }
  return new_hash_list;
} /* get_hash_list_prefix() */

/*
 * Creates a hash list of binary tree in postorder traversal.
 */

hash_list_t *get_hash_list_postfix(tree_node_t *root_node) {
  assert(root_node);

  hash_list_t *left = NULL;
  if (root_node->left_child_ptr) {
    left = get_hash_list_postfix(root_node->left_child_ptr);
  }
  hash_list_t *right = NULL;
  if (root_node->right_child_ptr) {
    right = get_hash_list_postfix(root_node->right_child_ptr);
  }

  hash_list_t *head_hash_list = NULL;
  if (left) {
    head_hash_list = left;
    while (left->next_hash_ptr) {
      left = left->next_hash_ptr;
    }
    left->next_hash_ptr = right;
  }
  else {
    head_hash_list = right;
  }

  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;

  if (left) {
    if (left->next_hash_ptr) {
      while (left->next_hash_ptr) {
        left = left->next_hash_ptr;
      }
      left->next_hash_ptr = new_hash_list;
    }
    else {
      left->next_hash_ptr = new_hash_list;
    }
  }
  else if (right) {
    while (right->next_hash_ptr) {
      right = right->next_hash_ptr;
    }
    right->next_hash_ptr = new_hash_list;
  }
  else {
    head_hash_list = new_hash_list;
  }
  return head_hash_list;
} /* get_hash_list_postfix() */

/*
 * Creates a hash list of binary tree in forward traversal.
 */

hash_list_t *get_hash_list_forward(tree_node_t *root_node) {
  assert(root_node);

  hash_list_t *left = NULL;
  if (root_node->left_child_ptr) {
    left = get_hash_list_forward(root_node->left_child_ptr);
  }

  hash_list_t *head = left;
  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;
  if (!left) {
    head = new_hash_list;
  }
  else {
    while (left->next_hash_ptr) {
      left = left->next_hash_ptr;
    }
    left->next_hash_ptr = new_hash_list;
  }

  hash_list_t *right = NULL;
  if (root_node->right_child_ptr) {
    right = get_hash_list_forward(root_node->right_child_ptr);
  }

  if (!left) {
    head->next_hash_ptr = right;
  }
  else {
    if (new_hash_list) {
      new_hash_list->next_hash_ptr = right;
    }
    else {
      left->next_hash_ptr = right;
    }
  }

  return head;
} /* get_hash_list_forward() */

/*
 * Creates a hash list of binary tree in reverse traversal.
 */

hash_list_t *get_hash_list_reverse(tree_node_t *root_node) {
  assert(root_node);

  hash_list_t *right = NULL;
  if (root_node->right_child_ptr) {
    right = get_hash_list_reverse(root_node->right_child_ptr);
  }

  hash_list_t *head = right;
  hash_list_t *new_hash_list = NULL;
  new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list);
  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;
  if (!right) {
    head = new_hash_list;
  }
  else {
    while (right->next_hash_ptr) {
      right = right->next_hash_ptr;
    }
    right->next_hash_ptr = new_hash_list;
  }

  hash_list_t *left = NULL;
  if (root_node->left_child_ptr) {
    left = get_hash_list_reverse(root_node->left_child_ptr);
  }

  if (!right) {
    head->next_hash_ptr = left;
  }
  else {
    if (new_hash_list) {
      new_hash_list->next_hash_ptr = left;
    }
    else {
      right->next_hash_ptr = left;
    }
  }

  return head;
} /* get_hash_list_reverse() */

/*
 * Frees all nodes and their data in linked list hash list.
 */

void delete_hash_list(hash_list_t *del_hash_list) {
  if (del_hash_list) {
    hash_list_t *temp_list = del_hash_list->next_hash_ptr;
    free(del_hash_list);
    del_hash_list = NULL;
    delete_hash_list(temp_list);
  }
} /* delete_hash_list() */
