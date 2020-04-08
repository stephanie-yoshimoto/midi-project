#ifndef HW11_H
#define HW11_H

/* Error codes */

// No error codes for this assignment.

/* typedefs */

typedef struct tree_node {
  int hash_value;
  struct tree_node *left_child_ptr;
  struct tree_node *right_child_ptr;
} tree_node_t;

typedef struct hash_list {
  int hash_value;
  struct hash_list *next_hash_ptr;
} hash_list_t;

/* Function prototypes */

tree_node_t *create_node(int);
tree_node_t *search_node(tree_node_t *, int);
void delete_tree(tree_node_t *);
void insert_node(tree_node_t **, tree_node_t *, int);
hash_list_t *get_hash_list_prefix(tree_node_t *);
hash_list_t *get_hash_list_postfix(tree_node_t *);
hash_list_t *get_hash_list_forward(tree_node_t *);
hash_list_t *get_hash_list_reverse(tree_node_t *);
void delete_hash_list(hash_list_t *);

#endif // HW11_H
