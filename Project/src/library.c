/* Stephanie Yoshimoto, library.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "library.h"

#include <assert.h>
#include <ftw.h>
#include <malloc.h>
#include <malloc_debug.h>
#include <string.h>

#define SUCCESS (0)
#define WRITE_FAIL (-1)

tree_node_t *g_song_library = NULL;

tree_node_t **find_parent_pointer(tree_node_t **root, const char *song_name) {
  tree_node_t *head = *root;
  if ((!root) || (!*root)) {
    return NULL;
  }
  else if (strcmp((*root)->song_name, song_name) == 0) {
    *root = head;
    return root;
  }
  else if (strcmp((*root)->song_name, song_name) < 0) {
    *root = head;
    return find_parent_pointer(&((*root)->right_child), song_name);
  }
  else {
    *root = head;
    return find_parent_pointer(&((*root)->left_child), song_name);
  }
} /* find_parent_pointer() */

int tree_insert(tree_node_t **root, tree_node_t *insert_node) {
  tree_node_t *head = *root;
  if (!*root) {
    *root = insert_node;
    return INSERT_SUCCESS;
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) == 0) {
    *root = head;
    return DUPLICATE_SONG;
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) < 0) {
    *root = head;
    return tree_insert(&((*root)->right_child), insert_node);
  }
  else {
    *root = head;
    return tree_insert(&((*root)->left_child), insert_node);
  }
} /* tree_insert() */

int remove_song_from_tree(tree_node_t **root, const char *song_name) {
  tree_node_t **root_copy = root;
  tree_node_t **target = find_parent_pointer(root, song_name);
  if (!target) {
    root = root_copy;
    return SONG_NOT_FOUND;
  }
  else {
    tree_node_t *left = (*target)->left_child;
    tree_node_t *right = (*target)->right_child;
    free_node(*target);
    tree_insert(root_copy, left);
    tree_insert(root_copy, right);
    root = root_copy;
    return DELETE_SUCCESS;
  }
} /* remove_song_from_tree() */

void free_node(tree_node_t *node) {
  if (node->song_name) {
    free(node->song_name);
    node->song_name = NULL;
  }

  free_song(node->song);
  node->song = NULL;
  free(node);
  node = NULL;
} /* free_node() */

void print_node(tree_node_t *node, FILE *file_ptr_out) {
  fprintf(file_ptr_out, "%s\n", node->song_name);
} /* print_node() */

void print_node_no_file(tree_node_t *node, void *ptr) {
  FILE *file_ptr_out = (FILE *)(ptr);
  print_node(node, file_ptr_out);
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

int analyze_file(const char *file_path, const struct stat *stat_ptr, int flag) {
  song_data_t *song = parse_file(file_path);
  if (song) {
    tree_node_t *new_node = malloc(sizeof(tree_node_t));
    assert(new_node);

    char copy_file_name[strlen(file_path) + 1];
    strcpy(copy_file_name, file_path);
    int last_char = strlen(copy_file_name) - 1;
    if ((copy_file_name[last_char] != 'd') || (copy_file_name[last_char - 1] !=
        'i') || (copy_file_name[last_char - 2] != 'm') ||
        (copy_file_name[last_char - 3] != '.')) {
      return WRITE_FAIL;
    }

    int temp_index = 0;
    for (int i = 0; copy_file_name[i] != '\0'; i++) {
      if (copy_file_name[i] == '/') {
        temp_index = i;
      }
    }
    char *song_name = &copy_file_name[temp_index + 1];
    new_node->song_name = malloc(strlen(song_name) + 1);
    assert(new_node->song_name);
    strcpy(new_node->song_name, song_name);
    new_node->song = song;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    int insert_result = tree_insert(&g_song_library, new_node);
    if (insert_result == DUPLICATE_SONG) {
      assert(false);
    }
    return SUCCESS;
  }
  else {
    return WRITE_FAIL;
  }
}

void make_library(const char *directory) {
  ftw(directory, analyze_file, 0);
  /*analyze_file(directory, NULL, 0);*/
}
