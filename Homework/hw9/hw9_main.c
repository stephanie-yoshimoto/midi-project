/* Includes */

#include <stdio.h>
#include <stdbool.h>
#include "hw9.h"

#define CREATE_COMMIT (1)
#define FIND_COMMIT (2)
#define INSERT_COMMIT (3)
#define REMOVE_COMMIT (4)
#define REMOVE_ALL_COMMITS_BY_AUTHOR (5)
#define REPAIR_LOG (6)
#define DISCONNECT_LOOP (7)
#define FREE_COMMIT (8)
#define EXIT (9)

#define MAX_AUTHORS (5)
#define MAX_NAME_LEN (40)
#define MAX_LIST_LEN (40)
#define MAX_HASH_LEN (33)
#define MAX_FILE_NAME_LEN (40)


/*
 * Function to print the list
 */

void print_list(commit_t *list_ptr) {
  if (list_ptr == NULL) {
    printf("No list!\n");
    return;
  }
  while (list_ptr != NULL) {
    printf("(%p)  ", list_ptr);
    if (list_ptr->data == NULL) {
      printf("There is no data entry in commit!!!\n\n");
      return;
    }
    printf("data   ->   author = %s\n", list_ptr->data->author);
    printf("            time   = %f\n", list_ptr->data->time);
    printf("            hash   = %s\n", list_ptr->data->hash);
    printf("next commit = %p\n", list_ptr->next_commit);
    printf("prev commit = %p\n\n", list_ptr->prev_commit);

    list_ptr = list_ptr->next_commit;
  }
} /* print_list() */


/*
 * Function to create a list
 */

commit_t *create_list() {
  printf("Using your create_commit function to create a list\n");
  char author[MAX_NAME_LEN] = "John";
  char hash[MAX_HASH_LEN] = "qwertyuiop";
  float time = 11.5;
  commit_t *c_commit = create_commit(author, time, hash);
  if (c_commit == NULL) {
    printf("create_commit function not implemented. ");
    printf("Please check again after implementing create_commit\n");
    return NULL;
  }
  return c_commit;
} /* create_list() */


/* This function is used to run the different functions implemented in file
 * hw9.c. Specific lists are created for each function. Upon completion the
 * function returns zero.
 */

int main() {

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  while (true) {
    printf("\n\nTest Menu\n\n");
    printf("1. Create commit\n");
    printf("2. Find commit\n");
    printf("3. Insert commit\n");
    printf("4. Remove commit\n");
    printf("5. Remove all commits by author\n");
    printf("6. Repair log\n");
    printf("7. Disconnect loop\n");
    printf("8. Free commit\n");
    printf("9. Exit\n");
    printf("Enter your choice\n");

    char author_name[100] = "";
    char hash[100] = "";
    float time = 0.0;
    int response = 0;

    scanf("%d", &response);
    getchar();

    commit_t *listz = NULL;
    commit_t *list1 = NULL;
    commit_t *lista = NULL;
    commit_t *listb = NULL;
    commit_t *listc = NULL;
    commit_t *listd = NULL;
    commit_t *liste = NULL;
    commit_t *check = NULL;

    switch (response) {
      case CREATE_COMMIT:
        printf("Enter author name\n");
        scanf("%s", author_name);
        printf("Enter time\n");
        scanf("%f", &time);
        printf("Enter hash\n");
        scanf("%s", hash);
        commit_t *commit_ptr = create_commit(author_name, time, hash);
        if (commit_ptr == NULL) {
          printf("create_commit function not implemented");
          printf("Please check again after implementing create_commit\n");
          break;
        }
        printf("\nYour commit struct looks like this:\n");
        print_list(commit_ptr);
        free_commit(commit_ptr);
        break;

      case FIND_COMMIT:
        listz = create_list();
        if (listz == NULL) {
          break;
        }
        printf("A list with one node is created:\n");
        print_list(listz);
        printf("Calling find_commit function with hash of the first node\n");
        commit_t *found = find_commit(listz, listz->data->hash);
        printf("\nThis is the returned list:\n");
        print_list(found);
        free_commit(listz);
        break;

      case INSERT_COMMIT:
        list1 = create_list();
        if (list1 == NULL) {
          break;
        }
        char author1[MAX_NAME_LEN] = "Fred";
        char hash1[MAX_HASH_LEN] = "asdf";
        float time1 = 23.0;
        commit_t *to_insert = create_commit(author1, time1, hash1);
        printf("Sending a list with 1 node with hash: %s", list1->data->hash);
        printf(" Expected to insert a node to create a list of 2 nodes\n");
        print_list(list1);
        insert_commit(list1, to_insert, list1->data->hash);
        printf("\nYour new list looks like this:\n");
        print_list(list1);
        free_commit(list1->next_commit);
        free_commit(list1);
        break;

      case REMOVE_COMMIT:
        lista = create_list();
        if (lista == NULL) {
          break;
        }
        printf("A list with one node is created\n");
        print_list(lista);
        printf("\nSending this list with hash of the first node. ");
        printf("Expected to return the pointer to the same node\n");
        commit_t *remove = remove_commit(lista, lista->data->hash);
        printf("\nThe removed node is\n");
        print_list(remove);
        free_commit(lista);
        break;

      case REMOVE_ALL_COMMITS_BY_AUTHOR:
        listb = create_list();
        if (listb == NULL) {
          break;
        }
        printf("A list with one is created\n");
        print_list(listb);
        printf("\nSending author of the first node. ");
        printf("Expected to remove the same node and return 1\n");
        int num = remove_all_commits_by_author(listb, listb->data->author);
        printf("\nNumber of nodes removed = %d\n", num);
        free_commit(listb);
        break;

      case REPAIR_LOG:
        listc = create_list();
        if (listc == NULL) {
          break;
        }
        char author2[MAX_NAME_LEN] = "Fred";
        char hash2[MAX_HASH_LEN] = "asdf";
        float time2 = 23.0;
        commit_t *repair_node = create_commit(author2, time2, hash2);
        listc->next_commit = repair_node;
        printf("Sending this list to your function to repair. ");
        printf("The list has a dangling prev_commit pointer\n");
        print_list(listc);
        repair_log(listc, listc->next_commit);
        printf("\nThe corrected list is\n");
        print_list(listc);
        free_commit(listc->next_commit);
        free_commit(listc);
        break;

      case DISCONNECT_LOOP:
        listd = create_list();
        if (listd == NULL) {
          break;
        }
        listd->next_commit = listd;
        printf("\nSending a single node list that has a self loop\n");
        commit_t *disconnected = disconnect_loop(listd);
        printf("The disconnected loop is\n");
        print_list(disconnected);
        free_commit(listd);
        break;

      case FREE_COMMIT:
        check = create_list();
        if (check == NULL) {
          break;
        }
        liste = create_list();
        check->next_commit = liste;
        printf("The list created is\n");
        print_list(liste);
        printf("\nCalling your free_commit function\n");
        free_commit(liste);
        printf("Done!\n");
        free_commit(check);
        break;

      case EXIT:
        printf("\nGoodbye!\n\n");
        return 0;

      default:
        printf("Invalid selection.\n");
        break;
    } /* Switch (choice) */
  } /* while (1) */
  return 0;
} /* main() */
