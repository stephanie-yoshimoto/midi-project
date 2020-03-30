/* Stephanie Yoshimoto, library.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "library.h"

#include <ftw.h>
#include <malloc.h>
#include <string.h>

tree_node_t *g_song_library;

tree_node_t **find_parent_pointer(tree_node_t **root, const char *song_name) {
  if (root == NULL) {
    return NULL;
  }
  else if (strcmp((*root)->song_name, song_name) == 0) {
    return root;
  }
  else if (strcmp((*root)->song_name, song_name) < 0) {
    return find_parent_pointer(&((*root)->right_child), song_name);
  }
  else {
    return find_parent_pointer(&((*root)->left_child), song_name);
  }
} /* find_parent_pointer() */

int tree_insert(tree_node_t **root, tree_node_t *insert_node) {
  if (root == NULL) {
    root = &insert_node;
    return INSERT_SUCCESS;
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) == 0) {
    return DUPLICATE_SONG;
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) < 0) {
    return tree_insert(&((*root)->right_child), insert_node);
  }
  else {
    return tree_insert(&((*root)->left_child), insert_node);
  }
} /* tree_insert() */

int remove_song_from_tree(tree_node_t **root, const char *song_name) {
  tree_node_t **root_copy = root;
  tree_node_t **target = find_parent_pointer(root, song_name);
  if (!target) {
    return SONG_NOT_FOUND;
  }
  else {
    tree_node_t *left = (*target)->left_child;
    tree_node_t *right = (*target)->right_child;
    free_node(*target);
    tree_insert(root_copy, left);
    tree_insert(root_copy, right);
    return DELETE_SUCCESS;
  }
} /* remove_song_from_tree() */

void free_node(tree_node_t *node) {
  if (node->song_name) {
    free(node->song_name);
    node->song_name = NULL;
  }

  free_song(node->song);
} /* free_node() */

void print_node(tree_node_t *node, FILE *file_ptr_out) {
  fprintf(file_ptr_out, "%s\n", node->song_name);
} /* print_node() */

void print_node_no_file(tree_node_t *node, void *ptr) {
  FILE *file_ptr_out = (FILE *)(ptr);
  fprintf(file_ptr_out, "%s\n", node->song_name);
} /* print_node_no_file() */

void traverse_pre_order(tree_node_t *root, void *data,
                        traversal_func_t visit) {
  if (!root) {
    return;
  }
  else {
    (*visit)(root, data);
    traverse_pre_order(root->left_child, data, visit);
    traverse_pre_order(root->right_child, data, visit);
  }
} /* traverse_pre_order() */

void traverse_in_order(tree_node_t *root, void *data, traversal_func_t visit) {
  if (!root) {
    return;
  }
  else {
    traverse_in_order(root->left_child, data, visit);
    (*visit)(root, data);
    traverse_in_order(root->right_child, data, visit);
  }
} /* traverse_in_order() */

void traverse_post_order(tree_node_t *root, void *data,
                       traversal_func_t visit) {
  if (!root) {
    return;
  }
  else {
    traverse_post_order(root->left_child, data, visit);
    traverse_post_order(root->right_child, data, visit);
    (*visit)(root, data);
  }
} /* traverse_post_order() */

void free_library(tree_node_t *root) {
  if (!root) {
    return;
  }
  else {
    free_library(root->left_child);
    free_library(root->right_child);
    free_node(root);
  }
} /* free_library() */

void write_song_list(FILE *fp, tree_node_t *root) {
  traverse_in_order(root, fp, print_node_no_file);
} /* write_song_list() */

int analyze_file(const char *file_name, const struct stat *stat_ptr, int flag) {
  return 0;
}

void make_library(const char *directory) {
  /* for every file in directory, check if last chars are .mid */
  /* if so, use first function to make sure that the duplicate is not already */
  /* there (should return null) */
  /* use insert node to insert new midi file to g_song_library tree */

  /* returns 0 on success, -1 if error */
  ftw(directory, analyze_file, 1);
}
