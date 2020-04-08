/* Stephanie Yoshimoto, library.c, CS 24000, Spring 2020
 * Last updated March 27, 2020
 */

#include "library.h"

#include <assert.h>
#include <ftw.h>
#include <malloc.h>
#include <string.h>

#define SUCCESS (0)
#define WRITE_FAIL (-1)

tree_node_t *g_song_library = NULL;

tree_node_t **find_parent_pointer(tree_node_t **root, const char *song_name) {
  if ((!root) || (!*root)) {
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
  if ((!*root) || (!root)) {
    *root = insert_node;
    return INSERT_SUCCESS;
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) == 0) {
    return DUPLICATE_SONG;
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) < 0) {
    return tree_insert(&((*root)->right_child), insert_node);
  }
  else if (strcmp((*root)->song_name, insert_node->song_name) > 0) {
    return tree_insert(&((*root)->left_child), insert_node);
  }
  return INSERT_SUCCESS;
} /* tree_insert() */

int remove_song_with_root(tree_node_t **node, const char *song_name,
                          tree_node_t **root) {
  tree_node_t **root_copy = root;
  if (!*node) {
    return SONG_NOT_FOUND;
  }
  else if (strcmp((*node)->song_name, song_name) > 0) {
    return remove_song_with_root(&(*node)->left_child, song_name, root);
  }
  else if (strcmp((*node)->song_name, song_name) < 0) {
    return remove_song_with_root(&(*node)->right_child, song_name, root);
  }
  else {
    if (!(*node)->left_child) {
      tree_node_t *temp = *node;
      *node = (*node)->right_child;
      free_node(temp);
      return DELETE_SUCCESS;
    }
    else if (!(*node)->right_child) {
      tree_node_t *temp = *node;
      *node = (*node)->left_child;
      free_node(temp);
      return DELETE_SUCCESS;
    }
    else {
      tree_node_t *right = (*node)->right_child;
      tree_node_t *left = (*node)->left_child;
      free_node(*node);
      *node = NULL;
      tree_insert(root_copy, right);
      tree_insert(root_copy, left);
      return DELETE_SUCCESS;
    }
  }
  return DELETE_SUCCESS;
} /* remove_song_with_root() */

int remove_song_from_tree(tree_node_t **root, const char *song_name) {
  return remove_song_with_root(root, song_name, root);
} /* remove_song_from_tree() */

void free_node(tree_node_t *node) {
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
  if (flag == FTW_D) {
    return SUCCESS;
  }

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
    new_node->song_name = song_name;
    new_node->song = song;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    int insert_result = tree_insert(&g_song_library, new_node);
    assert(insert_result != DUPLICATE_SONG);
    return SUCCESS;
  }
  else {
    return WRITE_FAIL;
  }
}

void make_library(const char *directory) {
  ftw(directory, analyze_file, 15);
}
