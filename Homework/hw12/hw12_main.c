#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw12.h"

#define QUIT (0)
#define MEMORY_TREE_MANIPULATIONS (1)
#define CPU_TREE_MANIPULATIONS (2)
#define CREATE_NODE (1)
#define INSERT_NODE (2)
#define FIND_NODES (3)
#define REMOVE_NODE (4)
#define DELETE_NODE (5)
#define DELETE_TREE (6)
#define PRINT_TREE (7)
#define MAX_STRING_LENGTH (100)

/*
 * Function to print the internal data of a cpu_t ptruct passed as a void
 * pointer to standard out
 */

void print_cpu_data(void *p) {
  cpu_t *ptr = (cpu_t *)p;
  assert(ptr && ptr->model && ptr->manufacturer);

  printf("%-32s %-16s %4d Mhz %2d\n",
      ptr->model, ptr->manufacturer,
      ptr->speed, ptr->cores);
} /* print_cpu_data() */

/*
 * Function to print the internal data of a memory_t ptruct passed as a void
 * pointer to sandard out
 */

void print_memory_data(void *p) {
  memory_t *ptr = (memory_t *)p;
  assert(ptr && ptr->model && ptr->manufacturer);

  printf("%-32s %-16s %8d MiB %5d DDR%d\n",
      ptr->model, ptr->manufacturer,
      ptr->size, ptr->speed, ptr->ddr_gen);
} /* print_memory_data() */

/*
 * Function to print theptree node by dynamically dispatching the print function
 * stored in the node on the data stored in the node
 */

void print_node(struct node *n_ptr) {
  assert(n_ptr && n_ptr->data && n_ptr->print);

  (n_ptr->print)(n_ptr->data);
} /* print_node() */

/*
 * Function to recursively print theptree using an inorderptraversal
 */

void print_tree(struct node *root ) {
  if (!root) {
    return;
  }

  print_tree(root->left);
  print_node(root);
  print_tree(root->right);

} /* print_tree() */

/*
 *  This function is used to run the functions in hw12.c. User input is used
 *  to determine which function to run and what is passed to it. main()
 *  prints information related to running the chosen function.
 */

int main(int argc, char *argv[]) {
  struct node **array_of_duplicates = NULL;
  struct node *root_node = NULL;
  struct node *node = NULL;
  void *data_node = NULL;

  while (1) {
    int data_type = 0;
    int user_choice = 0;
    while (!((data_type == MEMORY) || (data_type == CPU))) {
      printf("\n\n"
          "0. Quit\n"
          "1. memory Tree manipulations\n"
          "2. cpu Tree manipulations\n");

      user_choice = 0;
      printf("Enter user_choice: ");
      scanf("%d", &user_choice);
      getchar();

      if ((user_choice < QUIT) || (user_choice > CPU_TREE_MANIPULATIONS)) {
        printf("Invalid response!\n");
        continue;
      }

      if (user_choice == QUIT) {
        printf("\nHave a fine day.\n\n");
        return 0;
      }
      else if (user_choice == MEMORY_TREE_MANIPULATIONS) {
        data_type = MEMORY;
      }
      else {
        data_type = CPU;
      }
    }

    int exit = 0;
    while (!exit) {
      char model[MAX_STRING_LENGTH] = "\0";
      char manufacturer[MAX_STRING_LENGTH] = "\0";
      int speed = 0;
      int size = 0;
      int cores = 0;
      int ddr_gen = 0;

      user_choice = 666;
      printf("\n\n(dataType:Comparison) : %s\n\n",
          data_type == MEMORY ? "(MEMORY:model,manufacturer)"
          : "(CPU:cores * speed)");
      printf("0. Quit\n"
          "1. create_node\n"
          "2. insert_node\n"
          "3. find_nodes\n"
          "4. remove_node\n"
          "5. delete_node\n"
          "6. delete_tree\n"
          "7. print_tree\n");

      printf("Enter user_choice: ");
      scanf("%d", &user_choice);
      getchar();

      if ((user_choice < QUIT) || (user_choice > PRINT_TREE)) {
        printf("Invalid response!\n");
        continue;
      }

      if (user_choice == QUIT) {
        printf("Cleaning up!\n");
        if (root_node) {
          delete_tree(&root_node);
        }
        if (node) {
          delete_node(&node);
        }
        exit = 1;
      }

      if ((user_choice == CREATE_NODE) && (!node)) {
        printf("Manufacturer: ");
        scanf("%s", manufacturer);
        getchar();
        printf("Model: ");
        scanf("%s", model);
        getchar();

        if (data_type == MEMORY) {
          printf("size: ");
          scanf("%d", &size);
          getchar();
          printf("speed : ");
          scanf("%d", &(speed) );
          getchar();
          printf("ddr_gen : ");
          scanf("%d", &(ddr_gen) );
          getchar();
        }
        else {
          printf("speed: ");
          scanf("%d", &cores);
          getchar();
          printf("cores : ");
          scanf("%d", &cores);
          getchar();
        }
      }

      if (((user_choice == FIND_NODES) || (user_choice == REMOVE_NODE))
          && (!node)) {
        if (data_type == MEMORY) {
          printf("model: ");
          scanf("%s", model );
          getchar();
          printf("manufacturer: ");
          scanf("%s", manufacturer );
          getchar();
        }
        else {
          printf("speed: ");
          scanf("%d", &speed);
          getchar();
          printf("cores: ");
          scanf("%d", &cores);
          getchar();
        }
      }

      switch (user_choice) {
        case CREATE_NODE:
          if (node) {
            printf("There's a node created already. Try "
                "inserting it in the tree or deleting it before "
                "allocating another node");
            break;
          }

          if (data_type == MEMORY) {
            // Create the data node of the Generic node

            create_memory_data(&data_node, model, manufacturer,
                size, speed, ddr_gen);

            // Use the pointer returned by create_person_data to
            // create the generic node with a pointer to the data
            // node created above

            create_node(&node, data_node, print_memory_data,
                delete_memory_data, compare_memory_data);
          }
          else {
            create_cpu_data(&data_node, model, manufacturer,
                speed, cores);
            create_node(&node, data_node, print_cpu_data,
                delete_cpu_data, compare_cpu_data);
          }

          data_node = NULL;
          printf("Created Item is:\n");
          print_node(node);
          break;

        case INSERT_NODE:
          if (!node) {
            printf("Create a node first!\n");
            break;
          }
          printf("The tree before insertion :\n");
          print_tree(root_node);
          printf("After inserting node:\n");
          print_node(node);
          insert_node(&root_node, node);
          printf("\nThe new tree looks like :\n");
          print_tree(root_node);
          node = NULL;
          break;

        case FIND_NODES:
          if (!root_node) {
            printf("There is no tree!\n");
            break;
          }

          if (node) {
            printf("There's a node created already. Try "
                "inserting it in the tree or deleting it before "
                "allocating another node");
            break;
          }

          if (data_type == MEMORY) {
            // Create a node with only the comparison element

            create_memory_data(&data_node, model, manufacturer, 0, 0, 0);
          }
          else {
            // Create a node with only the comparison element

            create_cpu_data(&data_node, "", "", speed, cores);
          }

          // Now use that node to find nodes of the tree that
          // match the comparison
          // array_of_duplicates holds pointers to the duplicate nodes

          int nodes = 0;
          array_of_duplicates = find_nodes(root_node, data_node, &nodes);
          if (data_type == MEMORY) {
            printf("Found %d nodes with model,manufacturer '%s,%s'\n",
                nodes, model, manufacturer);
          }
          else {
            printf("Found %d nodes with speed * cores '%d * %d'\n",
                nodes, speed, cores);
          }
          for (int i = 0; i < nodes; i++) {
            printf("%d. ", i + 1);
            print_node(array_of_duplicates[i]);
          }

          // Free the 'dummy' node used to pass the comparison element
          // this function deallocates both the node and its data

          data_node = NULL;
          break;

        case REMOVE_NODE:
          if (!root_node) {
            printf("There is no tree!\n");
            break;
          }
          if (data_type == MEMORY) {
            // Create a node with only the comparison element

            create_memory_data(&data_node, model, manufacturer, 0, 0, 0);
            create_node(&node, data_node, print_memory_data,
                delete_memory_data, compare_memory_data);
          }
          else {
            // Create a node with only the comparison element

            create_cpu_data(&data_node, "", "", speed, cores);
            create_node(&node, data_node, print_cpu_data,
                delete_cpu_data, compare_cpu_data);
          }

          // Now use that node to find nodes of the tree that
          // match the comparison
          // array_of_duplicates holds pointers to the duplicate nodes

          array_of_duplicates = find_nodes(root_node, data_node, &nodes);
          if (data_type == MEMORY) {
            printf("Found %d nodes with model,manufacturer '%s,%s'\n",
                nodes, model, manufacturer);
          }
          else {
            printf("Found %d nodes with speed * cores '%d * %d'\n",
                nodes, speed, cores);
          }
          for (int i = 0; i < nodes; i++) {
            printf("%d. ", i + 1);
            print_node(array_of_duplicates[i]);
          }


          int index = 0;
          if (nodes > 1) {
            printf("Enter the index of part to remove: ");
            scanf("%d", &index);
            getchar();
            index--;
          }

          // If there's only one element in array, there are no
          // duplicates, only one node at index 0

          else if (nodes == 1) {
            index = 0;
          }

          // No matching nodes, nodes == 0

          else {
            printf("No part found!\n");
            break;
          }

          printf("Removing ");
          print_node(array_of_duplicates[index]);
          remove_node(&root_node, array_of_duplicates[index]);
          printf("New tree looks like: \n");
          print_tree(root_node);
          node = array_of_duplicates[index];

          // Free the 'dummy' node used to pass the comparison element
          // this function deallocates both the node and its data

          delete_node(&node);
          data_node = NULL;
          if (array_of_duplicates) {
            free(array_of_duplicates);
            array_of_duplicates = NULL;
          }
          break;

        case DELETE_NODE:
          if (!node) {
            printf("You need to remove or create a node first\n");
            break;
          }
          printf("Deleting node :\n");
          print_node(node);
          delete_node(&node);
          break;

        case DELETE_TREE:
          printf("Deleting entire Tree.\n");
          delete_tree(&root_node);
          break;

        case PRINT_TREE:
          printf("Current Tree: \n");
          print_tree(root_node);
          break;

      }
      /* end of switch */

    }
    /* end of  while (data_type ...  */

  }
  /* end of while(1) */

  /* never reached */

} /* main() */
