/* Stephanie Yoshimoto, hw10.c, CS 24000, Spring 2020
 * Last updated March 23, 2020
 */

#include "hw10.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

void new_tab(tab_t **first_tab) {
  assert(first_tab);

  int largest_tab_no = 0;
  tab_t *initial_tab = *first_tab;
  while (*first_tab) {
    if ((*first_tab)->tab_no > largest_tab_no) {
      largest_tab_no = (*first_tab)->tab_no;
    }

    if ((*first_tab)->next_tab) {
      *first_tab = (*first_tab)->next_tab;
    }
    else {
      break;
    }
  }

  page_data_t *page_data = NULL;
  page_data = malloc(sizeof(page_data_t));
  assert(page_data);
  page_data->current_page = true;
  page_data->page_no = 1;
  page_data->page_name = NULL;
  char *name = "Purdue CS";
  page_data->page_name = malloc(strlen(name) + 1);
  assert(page_data->page_name);
  strcpy(page_data->page_name, name);
  page_data->url = NULL;
  char *url_string = "cs.purdue.edu";
  page_data->url = malloc(strlen(url_string) + 1);
  assert(page_data->url);
  strcpy(page_data->url, url_string);

  tab_t *new_tab = NULL;
  new_tab = malloc(sizeof(tab_t));
  assert(new_tab);
  new_tab->tab_no = largest_tab_no + 1;
  new_tab->next_tab = NULL;
  new_tab->prev_tab = *first_tab;
  new_tab->page_info = page_data;
  new_tab->prev_page = NULL;
  new_tab->next_page = NULL;

  if (!initial_tab) {
    new_tab->tab_no = 1;
    *first_tab = new_tab;
  }
  else {
    (*first_tab)->next_tab = new_tab;
  }
} /* new_tab() */

void free_tab(tab_t *tab) {
  if (tab) {
    free(tab->page_info->page_name);
    tab->page_info->page_name = NULL;
    free(tab->page_info->url);
    tab->page_info->url = NULL;
    free(tab->page_info);
    tab->page_info = NULL;

    free(tab);
    tab = NULL;
  }
}

int close_tab(tab_t **first_tab, int tab_no) {
  assert((first_tab) && (tab_no > 0));

  tab_t *initial_tab = *first_tab;
  while (*first_tab) {
    if ((*first_tab)->tab_no == tab_no) {
      if ((*first_tab)->next_tab) {
        (*first_tab)->next_tab->prev_tab = (*first_tab)->prev_tab;
      }
      if ((*first_tab)->prev_tab) {
        (*first_tab)->prev_tab->next_tab = (*first_tab)->next_tab;
      }

      if (*first_tab == initial_tab) {
        **first_tab = *initial_tab;
      }

      free_tab(*first_tab);
      return SUCCESS;
    }
    else {
      *first_tab = (*first_tab)->next_tab;
    }
  }

  return NO_TAB;
} /* close_tab() */

void close_browser(tab_t **first_tab) {
  assert(first_tab);

  while (*first_tab) {
    close_tab(first_tab, (*first_tab)->tab_no);
  }
} /* close_browser() */

int page_go_prev(tab_t **first_tab, int tab_no) {
  assert((first_tab) && (tab_no > 0));

  tab_t *initial_tab = *first_tab;
  while (*first_tab) {
    if ((*first_tab)->tab_no == tab_no) {
      if (!(*first_tab)->prev_page) {
        return NO_PAGE;
      }

      (*first_tab)->page_info->current_page = false;
      (*first_tab)->prev_page->page_info->current_page = true;
      if ((*first_tab)->prev_tab) {
        (*first_tab)->prev_tab->next_page = (*first_tab)->prev_page;
      }
      if ((*first_tab)->next_tab) {
        (*first_tab)->next_tab->prev_page = (*first_tab)->prev_page;
      }

      if (*first_tab == initial_tab) {
        *first_tab = initial_tab->prev_page;
      }

      return SUCCESS;
    }
    else {
      *first_tab = (*first_tab)->next_tab;
    }
  }

  return NO_TAB;
} /* page_go_prev() */

int page_go_next(tab_t **first_tab, int tab_no) {
  assert((first_tab) && (tab_no > 0));

  tab_t *initial_tab = *first_tab;
  while (*first_tab) {
    if ((*first_tab)->tab_no == tab_no) {
      if (!(*first_tab)->next_page) {
        return NO_PAGE;
      }

      (*first_tab)->page_info->current_page = false;
      (*first_tab)->next_page->page_info->current_page = true;
      if ((*first_tab)->prev_tab) {
        (*first_tab)->prev_tab->next_page = (*first_tab)->next_page;
      }
      if ((*first_tab)->next_tab) {
        (*first_tab)->next_tab->prev_page = (*first_tab)->next_page;
      }

      if (*first_tab == initial_tab) {
        *first_tab = initial_tab->next_page;
      }

      return SUCCESS;
    }
    else {
      *first_tab = (*first_tab)->next_tab;
    }
  }

  return NO_TAB;
} /* page_go_next() */

int open_page(tab_t **first_tab, int tab_no, char *page_name, char *url) {
  assert((first_tab) && (page_name) && (url) && (tab_no > 0));
  return SUCCESS;
} /* open_page() */

int num_pages(tab_t **first_tab) {
  assert(first_tab);
  int num_pages = 0;
  while (*first_tab) {
    tab_t *current_page = *first_tab;
    tab_t *current_page_next = current_page->next_page;
    while (current_page) {
      num_pages++;
      current_page = current_page->prev_page;
    }
    while (current_page_next) {
      num_pages++;
      current_page_next = current_page_next->next_page;
    }

    *first_tab = (*first_tab)->next_tab;
  }
  return num_pages;
} /* num_pages() */

int write_all_tabs(tab_t **first_tab, char *file_name) {
  FILE *file_ptr_out = NULL;
  file_ptr_out = fopen(file_name, "w");
  if (!file_ptr_out) {
    return NON_WRITABLE_FILE;
  }

  return SUCCESS;
} /* write_all_tabs() */
