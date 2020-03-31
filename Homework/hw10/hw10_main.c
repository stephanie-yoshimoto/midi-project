/* Includes */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include "hw10.h"

/* Defines */

#define PRINT_BROWSER   (1)
#define NEW_TAB         (2)
#define CLOSE_TAB       (3)
#define CLOSE_BROWSER   (4)
#define PAGE_GO_PREV    (5)
#define PAGE_GO_NEXT    (6)
#define OPEN_PAGE       (7)
#define NUM_PAGES       (8)
#define WRITE_ALL_TABS  (9)
#define EXIT            (0)
#define MAX_STRING_LEN  (100)

/*
 * Function to print the full browser.
 */

void print_browser(tab_t **list_ptr) {
  if (list_ptr == NULL) {
    printf("\nBrowser not opened! Check global browser variable in main().\n");
    return;
  }

  tab_t *tab_ptr = *list_ptr;
  if (tab_ptr == NULL) {
    printf("\nBrowser not open!\n");
    return;
  }

  /* Rewind to print from first tab in browser. */
  while (tab_ptr->prev_tab != NULL) {
    tab_ptr = tab_ptr->prev_tab;
  }

  printf("\nYour browser currently looks like this:\n");
  printf("\n  Browser ptr: [%p]\n", list_ptr);

  while (tab_ptr != NULL) {

    printf("\n  tab ptr: [%p]\n", tab_ptr);
    printf("  tab_no: [%d]\n", tab_ptr->tab_no);
    printf("  prev_tab ptr: [%p]\n", tab_ptr->prev_tab);
    printf("  next_tab ptr: [%p]\n", tab_ptr->next_tab);
    tab_t *page_ptr = tab_ptr;

    /* Rewind to print from first page in tab */
    while (page_ptr->prev_page != NULL) {
      page_ptr = page_ptr->prev_page;
    }

    while (page_ptr != NULL) {
      page_data_t *page_info_ptr = page_ptr->page_info;
      printf("\n    page_info ptr: [%p]\n", page_info_ptr);
      if (page_info_ptr == NULL) {
        printf("    (This page is empty!)\n");
        page_ptr = page_ptr->next_page;
      }
      printf("    page_info -> current_page: [%s]\n",
          (page_info_ptr->current_page) ? "true" : "false");
      printf("    page_info -> page_no: [%d]\n", page_info_ptr->page_no);
      printf("    page_info -> page_name: [%s]\n", page_info_ptr->page_name);
      printf("    page_info -> url: [%s]\n", page_info_ptr->url);
      printf("    prev_page ptr: [%p]\n", page_ptr->prev_page);
      printf("    next_page ptr: [%p]\n", page_ptr->next_page);
      page_ptr = page_ptr->next_page;
    }
    tab_ptr = tab_ptr->next_tab;
  }
} /* print_browser() */

/*
 * This function is used to run the different functions implemented in file
 * hw10.c. Upon completion main returns zero.
 */

int main() {

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  int response = 0;
  int tab_no = 0;
  int return_val = 0;

  char page_name[MAX_STRING_LEN] = "";
  char url[MAX_STRING_LEN] = "";
  char file_name[MAX_STRING_LEN] = "";

  tab_t **browser = malloc(sizeof(tab_t**));
  (*browser) = NULL;
  new_tab(browser);

  printf("\nA browser has been opened for you with single tab and page...\n");

  while (true) {
    printf("\nTest Menu:\n");
    printf("1. print_browser()\n"
      "   [Use this to view full browser after any function.]\n");
    printf("2. new_tab()\n");
    printf("3. close_tab()\n");
    printf("4. close_browser()\n");
    printf("5. page_go_prev()\n");
    printf("6. page_go_next()\n");
    printf("7. open_page()\n");
    printf("8. num_pages()\n");
    printf("9. write_all_tabs()\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");

    int status = fscanf(stdin, "%d", &response);
    while (getchar() != '\n'){
    }
    if (status <= 0) {
      printf("\nInvalid input! Try again...\n");
      continue;
    }

    switch (response) {

      case PRINT_BROWSER:
        print_browser(browser);
        break;

      case NEW_TAB:
        new_tab(browser);
        printf("\nNew tab created.\n");
        break;

      case CLOSE_TAB:
        printf("\nEnter the tab number to be closed: ");
        fscanf(stdin, "%d", &tab_no);
        while (getchar() != '\n'){
        }
        return_val = close_tab(browser, tab_no);
        printf("close_tab() returned (%d).\n", return_val);
        break;

      case CLOSE_BROWSER:
        close_browser(browser);
        printf("\nBrowser closed.\n");
        break;

      case PAGE_GO_PREV:
        printf("\nEnter the tab number: ");
        fscanf(stdin, "%d", &tab_no);
        while (getchar() != '\n'){
        }
        return_val = page_go_prev(browser, tab_no);
        printf("page_go_prev() returned (%d).\n", return_val);
        break;

      case PAGE_GO_NEXT:
        printf("\nEnter the tab number: ");
        fscanf(stdin, "%d", &tab_no);
        while (getchar() != '\n'){
        }
        return_val = page_go_next(browser, tab_no);
        printf("page_go_next() returned (%d).\n", return_val);
        break;

      case OPEN_PAGE:
        printf("\nEnter the tab number: ");
        fscanf(stdin, "%d", &tab_no);
        while (getchar() != '\n'){
        }
        printf("Enter the page name: ");
        fscanf(stdin, "%s", page_name);
        printf("Enter the url: ");
        fscanf(stdin, "%s", url);
        return_val = open_page(browser, tab_no, page_name, url);
        printf("open_page() returned (%d).\n", return_val);
        break;

      case NUM_PAGES:
        return_val = num_pages(browser);
        printf("\nnum_pages() returned (%d).\n", return_val);
        break;

      case WRITE_ALL_TABS:
        printf("\nEnter the output file name: ");
        fscanf(stdin, "%s", file_name);
        return_val = write_all_tabs(browser, file_name);
        printf("write_all_tabs() returned (%d).\n", return_val);
        break;

      case EXIT:
        close_browser(browser);
        free(browser);
        browser = NULL;
        printf("\nExecuted close_browser().");
        printf("\nGoodbye!\n\n");
        return 0;

      default:
        printf("\nInvalid input! Try again...\n");
        break;

    } /* switch (response) */
  } /* while (true) */

  return 0;
} /* main() */
