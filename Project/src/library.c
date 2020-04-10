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
#define NUM_DIRS (20)

tree_node_t *g_song_library = NULL;

/*
 * Finds and returns pointer to node with corresponding song name if node
 * exists in tree.
 */

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

/*
 * Inserts node into tree at sorted position, given root of tree.
 */

int tree_insert(tree_node_t **root, tree_node_t *insert_node) {
  if (!*root) {
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

/*
 * Removes song with corresponding song name from tree.
 */

int remove_song_from_tree(tree_node_t **root, const char *song_name) {
  tree_node_t **root_copy = root;
  tree_node_t **found_node = find_parent_pointer(root, song_name);
  if ((!found_node) || (!*found_node)) {
    return SONG_NOT_FOUND;
  }

  if (!(*found_node)->left_child) {
    tree_node_t *temp = *found_node;
    *found_node = (*found_node)->right_child;
    free_node(temp);
  }
  else if (!(*found_node)->right_child) {
    tree_node_t *temp = *found_node;
    *found_node = (*found_node)->left_child;
    free_node(temp);
  }
  else {
    tree_node_t *right = (*found_node)->right_child;
    tree_node_t *left = (*found_node)->left_child;
    free_node(*found_node);
    tree_insert(root_copy, right);
    tree_insert(root_copy, left);
  }
  return DELETE_SUCCESS;
} /* remove_song_from_tree() */

/*
 * Frees all data attached to tree node.
 */

void free_node(tree_node_t *node) {
  free_song(node->song);
  node->song = NULL;
  free(node);
  node = NULL;
} /* free_node() */

/*
 * Prints song name of node to file.
 */

void print_node(tree_node_t *node, FILE *file_ptr_out) {
  fprintf(file_ptr_out, "%s\n", node->song_name);
} /* print_node() */

/*
 * Traverses tree in preorder, performing operation on each node.
 */

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

/*
 * Traverses tree in inorder, performing operation on each node.
 */

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

/*
 * Traverses tree in postorder, performing operation on each node.
 */

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

/*
 * Frees all tree nodes in library.
 */

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

/*
 * Prints the names of all songs in tree to output in sorted order.
 */

void write_song_list(FILE *fp, tree_node_t *root) {
  traverse_in_order(root, fp, (traversal_func_t) print_node);
} /* write_song_list() */

/*
 * Used by ftw function to check if each file is a MIDI file and if it is a
 * duplicate file.
 */

int analyze_file(const char *file_path, const struct stat *stat_ptr, int flag) {
  if (flag == FTW_F) {
    int last_char = strlen(file_path) - 1;
    if ((file_path[last_char] == 'd') &&
        (file_path[last_char - 1] == 'i') &&
        (file_path[last_char - 2] == 'm') &&
        (file_path[last_char - 3] == '.')) {
      song_data_t *song = parse_file(file_path);
      if (song) {
        tree_node_t *new_node = malloc(sizeof(tree_node_t));
        assert(new_node);

        int index = 0;
        for (int i = 0; file_path[i] != '\0'; i++) {
          if (file_path[i] == '/') {
            index = i;
          }
        }
        new_node->song = song;
        new_node->song_name = new_node->song->path + index + 1;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        assert(tree_insert(&g_song_library, new_node) != DUPLICATE_SONG);
      }
    }
  }
  return SUCCESS;
} /* analyze_file() */

/*
 * Searches through directory structure and adds every MIDI file to
 * g_song_library.
 */

void make_library(const char *directory) {
  ftw(directory, analyze_file, NUM_DIRS);
} /* make_library() */
