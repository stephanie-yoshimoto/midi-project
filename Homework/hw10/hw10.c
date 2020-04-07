/* Stephanie Yoshimoto, hw10.c, CS 24000, Spring 2020
 * Last updated March 23, 2020
 */

#include "hw10.h"

#include <assert.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define NEW_PAGE_NO (1)

/*
 * malloc's a new tab with specified parameters, adds tab to end of tab list.
 */

void new_tab(tab_t **first_tab) {
  assert(first_tab);

  page_data_t *page_data = NULL;
  page_data = malloc(sizeof(page_data_t));
  assert(page_data);
  page_data->current_page = true;
  page_data->page_no = NEW_PAGE_NO;
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
  new_tab->next_tab = NULL;
  new_tab->page_info = page_data;
  new_tab->prev_page = NULL;
  new_tab->next_page = NULL;

  int largest_tab_no = 0;
  tab_t *temp = *first_tab;
  while (temp) {
    if (temp->tab_no > largest_tab_no) {
      largest_tab_no = temp->tab_no;
    }

    if (temp->next_tab) {
      temp = temp->next_tab;
    }
    else {
      break;
    }
  }
  new_tab->tab_no = largest_tab_no + 1;
  new_tab->prev_tab = temp;
  if (!*first_tab) {
    *first_tab = new_tab;
    return;
  }

  tab_t *current_page = temp;
  while (current_page) {
    current_page->next_tab = new_tab;
    current_page = current_page->prev_page;
  }
  tab_t *following_page = temp->next_page;
  while (following_page) {
    following_page->next_tab = new_tab;
    following_page = following_page->next_page;
  }

  temp->next_tab = new_tab;
} /* new_tab() */

/*
 * Used by close_tab() to free tab data.
 */

void free_tab(tab_t *tab) {
  if (tab) {
    /* free page list */

    tab_t *current_page = tab;
    tab_t *current_page_next = tab->next_page;
    while (current_page) {
      free(current_page->page_info->page_name);
      current_page->page_info->page_name = NULL;
      free(current_page->page_info->url);
      current_page->page_info->url = NULL;
      free(current_page->page_info);
      current_page->page_info = NULL;
      tab_t *temp = current_page;
      current_page = current_page->prev_page;
      free(temp);
      temp = NULL;
    }
    while (current_page_next) {
      free(current_page_next->page_info->page_name);
      current_page_next->page_info->page_name = NULL;
      free(current_page_next->page_info->url);
      current_page_next->page_info->url = NULL;
      free(current_page_next->page_info);
      current_page_next->page_info = NULL;
      tab_t *temp = current_page_next;
      current_page_next = current_page_next->next_page;
      free(temp);
      temp = NULL;
    }
  }
} /* free_tab() */

/*
 * Closes tab at tab number, adjusts adjacent pointers.
 */

int close_tab(tab_t **first_tab, int tab_no) {
  assert((first_tab) && (tab_no > 0));

  tab_t *initial_tab = *first_tab;
  tab_t *temp = *first_tab;
  while (temp) {
    if (temp->tab_no == tab_no) {
      if (temp->next_tab) {
        tab_t *current_page = temp->next_tab;
        while (current_page) {
          current_page->prev_tab = temp->prev_tab;
          current_page = current_page->prev_page;
        }
        tab_t *following_page = temp->next_tab->next_page;
        while (following_page) {
          following_page->prev_tab = temp->prev_tab;
          following_page = following_page->next_page;
        }
      }
      if (temp->prev_tab) {
        tab_t *current_page = temp->prev_tab;
        while (current_page) {
          current_page->next_tab = temp->next_tab;
          current_page = current_page->prev_page;
        }
        tab_t *following_page = temp->prev_tab->next_page;
        while (following_page) {
          following_page->next_tab = temp->next_tab;
          following_page = following_page->next_page;
        }
      }
      else {
        /* first tab */

        *first_tab = temp->next_tab;
        free_tab(temp);
        return SUCCESS;
      }

      free_tab(temp);
      *first_tab = initial_tab;
      return SUCCESS;
    }
    else {
      temp = temp->next_tab;
    }
  }

  *first_tab = initial_tab;
  return NO_TAB;
} /* close_tab() */

/*
 * Closes and frees all tabs in tab linked list.
 */

void close_browser(tab_t **first_tab) {
  assert(first_tab);

  while (*first_tab) {
    close_tab(first_tab, (*first_tab)->tab_no);
  }
} /* close_browser() */

/*
 * Goes to previous page in tab, adjusts adjacent pointers.
 */

int page_go_prev(tab_t **first_tab, int tab_no) {
  assert((first_tab) && (tab_no > 0));

  tab_t *head = *first_tab;
  tab_t *initial_tab = *first_tab;
  tab_t *temp = *first_tab;
  while (temp) {
    if (temp->tab_no == tab_no) {
      if (!temp->prev_page) {
        *first_tab = head;
        return NO_PAGE;
      }

      temp->page_info->current_page = false;
      temp = temp->prev_page;
      temp->page_info->current_page = true;
      if (temp->prev_tab) {
        tab_t *current_page = temp->prev_tab;
        while (current_page) {
          current_page->next_tab = temp;
          current_page = current_page->prev_page;
        }
        tab_t *following_page = temp->prev_tab->next_page;
        while (following_page) {
          following_page->next_tab = temp;
          following_page = following_page->next_page;
        }
      }

      if (temp->next_tab) {
        tab_t *current_page = temp->next_tab;
        while (current_page) {
          current_page->prev_tab = temp;
          current_page = current_page->prev_page;
        }
        tab_t *following_page = temp->next_tab->next_page;
        while (following_page) {
          following_page->prev_tab = temp;
          following_page = following_page->next_page;
        }
      }

      if (temp == initial_tab) {
        *first_tab = temp;
      }
      else {
        *first_tab = head;
      }

      return SUCCESS;
    }
    else {
      temp = temp->next_tab;
    }
  }

  *first_tab = head;
  return NO_TAB;
} /* page_go_prev() */

/*
 * Goes to next page in tab, adjusts adjacent tab pointers.
 */

int page_go_next(tab_t **first_tab, int tab_no) {
  assert((first_tab) && (tab_no > 0));

  tab_t *head = *first_tab;
  tab_t *initial_tab = *first_tab;
  tab_t *temp = *first_tab;
  while (temp) {
    if (temp->tab_no == tab_no) {
      if (!temp->next_page) {
        *first_tab = head;
        return NO_PAGE;
      }

      temp->page_info->current_page = false;
      temp = temp->next_page;
      temp->page_info->current_page = true;
      if (temp->prev_tab) {
        tab_t *current_page = temp->prev_tab;
        while (current_page) {
          current_page->next_tab = temp;
          current_page = current_page->prev_page;
        }
        tab_t *following_page = temp->prev_tab->next_page;
        while (following_page) {
          following_page->next_tab = temp;
          following_page = following_page->next_page;
        }
      }

      if (temp->next_tab) {
        tab_t *current_page = temp->next_tab;
        while (current_page) {
          current_page->prev_tab = temp;
          current_page = current_page->prev_page;
        }
        tab_t *following_page = temp->next_tab->next_page;
        while (following_page) {
          following_page->prev_tab = temp;
          following_page = following_page->next_page;
        }
      }

      if (temp == initial_tab) {
        *first_tab = temp;
      }
      else {
        *first_tab = head;
      }

      return SUCCESS;
    }
    else {
      temp = temp->next_tab;
    }
  }

  *first_tab = head;
  return NO_TAB;
} /* page_go_next() */

/*
 * Opens a new page at tab number, arranges adjacent pointers and clears
 * pages after new page.
 */

int open_page(tab_t **first_tab, int tab_no, char *page_name, char *url) {
  assert((first_tab) && (page_name) && (url) && (tab_no > 0));

  tab_t *temp = *first_tab;
  if (!temp) {
    return NO_TAB;
  }

  bool tab_found = false;
  while (temp) {
    if (temp->tab_no == tab_no) {
      tab_found = true;
      break;
    }
    else {
      temp = temp->next_tab;
    }
  }

  if (!tab_found) {
    return NO_TAB;
  }

  page_data_t *page_data = NULL;
  page_data = malloc(sizeof(page_data_t));
  assert(page_data);
  page_data->current_page = true;
  page_data->page_name = NULL;
  page_data->page_name = malloc(strlen(page_name) + 1);
  assert(page_data->page_name);
  strcpy(page_data->page_name, page_name);
  page_data->url = NULL;
  page_data->url = malloc(strlen(url) + 1);
  assert(page_data->url);
  strcpy(page_data->url, url);

  tab_t *tab = NULL;
  tab = malloc(sizeof(tab_t));
  assert(tab);
  tab->tab_no = tab_no;
  tab->page_info = page_data;
  tab->next_tab = temp->next_tab;
  tab->prev_tab = temp->prev_tab;

  /* find largest page number */

  int largest_page_no = 0;
  tab_t *current_page = temp;
  tab_t *following_page = temp->next_page;
  while ((current_page) || (following_page)) {
    if (current_page) {
      if (current_page->page_info->page_no > largest_page_no) {
        largest_page_no = current_page->page_info->page_no;
      }
      current_page = current_page->prev_page;
    }
    if (following_page) {
      if (following_page->page_info->page_no > largest_page_no) {
        largest_page_no = following_page->page_info->page_no;
      }
      following_page = following_page->next_page;
    }
  }
  page_data->page_no = largest_page_no + 1;
  current_page = temp;
  tab->prev_page = current_page;
  tab->prev_page->page_info->current_page = false;
  tab->next_page = current_page->next_page;

  /* free all data after current page */

  temp = current_page->next_page;
  while (temp) {
    free(temp->page_info->page_name);
    temp->page_info->page_name = NULL;
    free(temp->page_info->url);
    temp->page_info->url = NULL;
    free(temp->page_info);
    temp->page_info = NULL;
    tab_t *temp_page = temp;
    temp = temp->next_page;
    free(temp_page);
    temp_page = NULL;
  }

  current_page->next_page = tab;
  tab->prev_page = current_page;
  tab->next_page = NULL;

  /* all pages in next tab must point to new current page */

  tab_t *next_tab = current_page->next_tab;
  if (next_tab) {
    tab_t *next_tab_page = next_tab;
    while (next_tab_page) {
      next_tab_page->prev_tab = tab;
      next_tab_page = next_tab_page->prev_page;
    }

    tab_t *next_tab_next = next_tab->next_page;
    while (next_tab_next) {
      next_tab_next->prev_tab = tab;
      next_tab_next = next_tab_next->next_page;
    }
  }

  /* all pages in previous tab must point to new current page */

  tab_t *head = *first_tab;
  tab_t *prev_tab = current_page->prev_tab;
  if (prev_tab) {
    tab_t *prev_tab_page = prev_tab;
    while (prev_tab_page) {
      prev_tab_page->next_tab = tab;
      prev_tab_page = prev_tab_page->prev_page;
    }

    tab_t *prev_tab_next = prev_tab->next_page;
    while (prev_tab_next) {
      prev_tab_next->next_tab = tab;
      prev_tab_next = prev_tab_next->next_page;
    }
  }
  else {
    /* first tab */

    *first_tab = tab;
    return SUCCESS;
  }

  *first_tab = head;
  return SUCCESS;
} /* open_page() */

/*
 * Finds number of pages in each tab in list.
 */

int num_pages(tab_t **first_tab) {
  assert(first_tab);

  tab_t *head = *first_tab;
  int num_pages = 0;
  while (*first_tab) {
    tab_t *current_page = *first_tab;
    while (current_page) {
      num_pages++;
      current_page = current_page->prev_page;
    }
    current_page = (*first_tab)->next_page;
    while (current_page) {
      num_pages++;
      current_page = current_page->next_page;
    }

    *first_tab = (*first_tab)->next_tab;
  }

  *first_tab = head;
  return num_pages;
} /* num_pages() */

/*
 * Writes all tabs' number, page number, page name, and link to output file.
 */

int write_all_tabs(tab_t **first_tab, char *file_name) {
  assert((first_tab) && (file_name));
  FILE *file_ptr_out = NULL;
  file_ptr_out = fopen(file_name, "w");
  if (!file_ptr_out) {
    return NON_WRITABLE_FILE;
  }

  tab_t *head = *first_tab;
  while (*first_tab) {
    if ((*first_tab)->tab_no) {
      fprintf(file_ptr_out, "TAB %d\n", (*first_tab)->tab_no);
    }
    else {
      fclose(file_ptr_out);
      file_ptr_out = NULL;
      return NO_TAB;
    }
    fprintf(file_ptr_out, "Page number = %d\n",
            (*first_tab)->page_info->page_no);
    fprintf(file_ptr_out, "Page name = %s\n",
            (*first_tab)->page_info->page_name);
    fprintf(file_ptr_out, "Link = %s\n\n", (*first_tab)->page_info->url);
    *first_tab = (*first_tab)->next_tab;
  }

  *first_tab = head;
  fclose(file_ptr_out);
  file_ptr_out = NULL;
  return SUCCESS;
} /* write_all_tabs() */
