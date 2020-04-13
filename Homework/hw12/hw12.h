#ifndef HW12_H
#define HW12_H

#define CPU     (1)
#define MEMORY  (2)

/* Generic node structure */

struct node {                                /* generic Tree node           */
  struct node *left;                         /* pointer to left child       */
  struct node *right;                        /* pointer to right child      */
  void        *data;                         /* pointer to the data struct  */
  void        (*print)(void *);              /* pointer to print function   */
  void        (*delete)(void **);            /* pointer to delete function  */
  int         (*compare)(void *, void *);    /* pointer to compare function */
};

typedef struct cpu_s {
  char * model;
  char * manufacturer;
  int speed;
  int cores;
} cpu_t;

typedef struct memory_s {
  char * model;
  char * manufacturer;
  int size;
  int speed;
  int ddr_gen;
} memory_t;

/*
 * Functions of the Tree
 */

void print_tree(struct node *);
void delete_tree(struct node **);

/*
 * Functions of the Tree-nodes
 */

void print_node(struct node *);
void create_node(struct node **, void *, void (*)(void*),
                 void (*)(void **), int (*)(void *, void *));
void delete_node(struct node **);
void insert_node(struct node **, struct node *);
void remove_node(struct node **, struct node *);
struct node **find_nodes(struct node *, void *, int *);

/*
 * Functions for accessing/manipulating node-data
 */

/* data access/manipulation functions for person structure */

void print_cpu_data(void *);
void create_cpu_data(void **, const char *,
                        const char *, int, int);
void delete_cpu_data(void **);
int  compare_cpu_data(void *, void *);


/* data access/manipulation functions for student structure */

void print_memory_data(void *);
void create_memory_data(void **, const char *,
                         const char *, int, int, int);
void delete_memory_data(void **);
int  compare_memory_data(void *, void *);

/* Double linked list functions */
void insert_node_to_list(struct node**, struct node*);
void delete_node_from_list(struct node**, struct node*);
struct node**find_nodes_in_list(struct node *,void *, int *);
#endif // HW12_H
