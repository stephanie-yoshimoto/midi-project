/* Includes */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "hw11.h"

/* Defines */

#define CREATE_NODE           (1)
#define SEARCH_NODE           (2)
#define DELETE_TREE           (3)
#define INSERT_NODE           (4)
#define GET_HASH_LIST_PREFIX  (5)
#define GET_HASH_LIST_POSTFIX (6)
#define GET_HASH_LIST_FORWARD (7)
#define GET_HASH_LIST_REVERSE (8)
#define DELETE_HASH_LIST      (9)
#define EXIT                  (0)

/*
 * This function is used to run the different functions implemented in file
 * hw11.c. Upon completion main returns zero.
 */

int main() {

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  int response = 0;
  int hash_value = 0;
  int child_type = 0;
  char reply = '\0';
  tree_node_t *new_node = NULL;
  tree_node_t *tree_head = NULL;
  tree_node_t *search_return = NULL;
  hash_list_t *hash_list_head = NULL;
  hash_list_t *hash_node = NULL;

  printf("\nINFO: You can use this main function to create your own binary tree"
    "\nusing the functions given, and use that tree to test other functions."
    "\nFor example, you can create a full binary tree of depth 3 using the"
    "\nfirst four functions and the following combination of inputs:\n\n");
  printf("  Node 1: 1-1-4-0-y\n"
         "  Node 2: 1-2-4-1-y-0\n"
         "  Node 3: 1-3-4-1-y-1\n"
         "  Node 4: 1-4-4-2-y-0\n"
         "  Node 5: 1-5-4-2-y-1\n"
         "  Node 6: 1-6-4-3-y-0\n"
         "  Node 7: 1-7-4-3-y-1\n");
  printf("\nThe four traversals would yield the following hash lists:\n"
         "\n  PREFIX:  1->2->4->5->3->6->7"
         "\n  POSTFIX: 4->5->2->6->7->3->1"
         "\n  FORWARD: 4->2->5->1->6->3->7"
         "\n  REVERSE: 7->3->6->1->5->2->4\n");

  printf("\nAn empty binary tree head pointer [%p] has been created.\n",
    &tree_head);

  while (true) {
    printf("\n  Test Menu:\n");
    printf("  1. create_node()\n");
    printf("  2. search_node()\n");
    printf("  3. delete_tree()\n");
    printf("  4. insert_node()\n");
    printf("  5. get_hash_list_prefix()\n");
    printf("  6. get_hash_list_postfix()\n");
    printf("  7. get_hash_list_forward()\n");
    printf("  8. get_hash_list_reverse()\n");
    printf("  9. delete_hash_list()\n");
    printf("  0. Exit\n");
    printf("\nEnter your choice: ");

    int status = fscanf(stdin, "%d", &response);
    while (getchar() != '\n') {
    }
    if (status <= 0) {
      printf("\nInvalid input! Try again...\n");
      continue;
    }

    switch (response) {

      case CREATE_NODE:
        printf("\nEnter a hash value integer: ");
        fscanf(stdin, "%d", &hash_value);
        while (getchar() != '\n'){
        }
        if (new_node) {
          free(new_node);
          new_node = NULL;
        }
        new_node = create_node(hash_value);
        printf("New node created with pointer [%p].\n", new_node);
        break;

      case SEARCH_NODE:
        printf("\nEnter a hash value integer for search: ");
        fscanf(stdin, "%d", &hash_value);
        while (getchar() != '\n'){
        }
        search_return = search_node(tree_head, hash_value);
        if (search_return == NULL) {
          printf("Node with hash value (%d) not found in tree.\n", hash_value);
        }
        else {
          printf("Node with hash value (%d) found at [%p]\n",
            search_return->hash_value, search_return);
        }
        break;

      case DELETE_TREE:
        printf("Are you sure you wish to delete the entire tree? (y/n): ");
        fscanf(stdin, "%c", &reply);
        while (getchar() != '\n'){
        }
        if ((reply == 'y') || (reply == 'Y')) {
          delete_tree(tree_head);
          printf("Tree deleted.\n");
          tree_head = NULL;
        }
        break;

      case INSERT_NODE:
        printf("\nThe new node created will be inserted in the tree.\n");
        printf("Enter the parent node hash value (0 to insert at root): ");
        fscanf(stdin, "%d", &hash_value);
        while (getchar() != '\n'){
        }
        if (hash_value == 0) {
          search_return = NULL;
        }
        else {
          search_return = search_node(tree_head, hash_value);
          printf("Search returned [%p].\n", search_return);
          if (search_return == NULL) {
            search_return = tree_head;
          }
        }
        printf("Do you wish to continue? (y/n): ");
        fscanf(stdin, "%c", &reply);
        while (getchar() != '\n'){
        }
        if ((reply == 'y') || (reply == 'Y')) {
          if (hash_value != 0) {
            printf("Enter 0 for left child and 1 for right child: ");
            fscanf(stdin, "%d", &child_type);
            while (getchar() != '\n'){
            }
            insert_node(&search_return, new_node, child_type);
          }
          else {
            insert_node(&tree_head, new_node, 0);
          }
          printf("Node inserted.\n");
          new_node = NULL;
        }
        break;

      case GET_HASH_LIST_PREFIX:
        if (hash_list_head) {
          printf("Deleting previous hash list using delete_hash_list()\n");
          delete_hash_list(hash_list_head);
          hash_list_head = NULL;
        }
        printf("Getting hash list by traversing the tree in PREFIX order.\n");
        hash_list_head = get_hash_list_prefix(tree_head);
        if (hash_list_head == NULL) {
          printf("NULL hash list!\n");
        }
        else {
          hash_node = hash_list_head;
          printf("%d", hash_node->hash_value);
          hash_node = hash_node->next_hash_ptr;
          while (hash_node != NULL) {
            printf("->%d", hash_node->hash_value);
            hash_node = hash_node->next_hash_ptr;
          }
          printf("\n");
        }
        break;

      case GET_HASH_LIST_POSTFIX:
        if (hash_list_head) {
          printf("Deleting previous hash list using delete_hash_list()\n");
          delete_hash_list(hash_list_head);
          hash_list_head = NULL;
        }
        printf("Getting hash list by traversing the tree in POSTFIX order.\n");
        hash_list_head = get_hash_list_postfix(tree_head);
        if (hash_list_head == NULL) {
          printf("NULL hash list!\n");
        }
        else {
          hash_node = hash_list_head;
          printf("%d", hash_node->hash_value);
          hash_node = hash_node->next_hash_ptr;
          while (hash_node != NULL) {
            printf("->%d", hash_node->hash_value);
            hash_node = hash_node->next_hash_ptr;
          }
          printf("\n");
        }
        break;

      case GET_HASH_LIST_FORWARD:
        if (hash_list_head) {
          printf("Deleting previous hash list using delete_hash_list()\n");
          delete_hash_list(hash_list_head);
          hash_list_head = NULL;
        }
        printf("Getting hash list by traversing the tree in FORWARD order.\n");
        hash_list_head = get_hash_list_forward(tree_head);
        if (hash_list_head == NULL) {
          printf("NULL hash list!\n");
        }
        else {
          hash_node = hash_list_head;
          printf("%d", hash_node->hash_value);
          hash_node = hash_node->next_hash_ptr;
          while (hash_node != NULL) {
            printf("->%d", hash_node->hash_value);
            hash_node = hash_node->next_hash_ptr;
          }
          printf("\n");
        }
        break;

      case GET_HASH_LIST_REVERSE:
        if (hash_list_head) {
          printf("Deleting previous hash list using delete_hash_list()\n");
          delete_hash_list(hash_list_head);
          hash_list_head = NULL;
        }
        printf("Getting hash list by traversing the tree in REVERSE order.\n");
        hash_list_head = get_hash_list_reverse(tree_head);
        if (hash_list_head == NULL) {
          printf("NULL hash list!\n");
        }
        else {
          hash_node = hash_list_head;
          printf("%d", hash_node->hash_value);
          hash_node = hash_node->next_hash_ptr;
          while (hash_node != NULL) {
            printf("->%d", hash_node->hash_value);
            hash_node = hash_node->next_hash_ptr;
          }
          printf("\n");
        }
        break;

      case DELETE_HASH_LIST:
        printf("Do you wish to delete the entire hash list? (y/n): ");
        fscanf(stdin, "%c", &reply);
        while (getchar() != '\n'){
        }
        if ((reply == 'y') || (reply == 'Y')) {
          delete_hash_list(hash_list_head);
          printf("Hash list deleted.\n");
          hash_list_head = NULL;
        }
        break;

      case EXIT:
        if (new_node) {
          printf("\nFreeing the created new node.");
          free(new_node);
          new_node = NULL;
        }
        if (tree_head) {
          printf("\nDeleting tree using delete_tree()");
          delete_tree(tree_head);
          tree_head = NULL;
        }
        if (hash_list_head) {
          printf("\nDeleting hash list using delete_hash_list()");
          delete_hash_list(hash_list_head);
          hash_list_head = NULL;
        }
        printf("\nGoodbye!\n\n");
        return 0;

      default:
        printf("\nInvalid input! Try again...\n");
        break;

    } /* switch (response) */
  } /* while (true) */

  return 0;
} /* main() */
