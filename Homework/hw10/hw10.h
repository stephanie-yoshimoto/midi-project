#ifndef HW10_H
#define HW10_H

#include <stdbool.h>

/* Error codes */
#define SUCCESS (0)
#define NO_TAB (-1)
#define NO_PAGE (-2)
#define NON_WRITABLE_FILE (-3)

/* typedefs */

typedef struct tab {
  int tab_no;
  struct tab *next_tab;
  struct tab *prev_tab;
  struct page_data *page_info;
  struct tab *next_page;
  struct tab *prev_page;
} tab_t;

typedef struct page_data {
  bool current_page;
  int page_no;
  char *page_name;
  char *url;

} page_data_t;

/* Function prototypes */

void new_tab(tab_t **);
int close_tab (tab_t **, int);
void close_browser(tab_t **);
int page_go_prev(tab_t **, int);
int page_go_next(tab_t **, int);
int open_page (tab_t **, int, char*, char*);
int num_pages (tab_t **);
int write_all_tabs (tab_t **, char *);

#endif // HW10_H
