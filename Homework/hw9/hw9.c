/* Stephanie Yoshimoto, hw9.c, CS 24000, Spring 2020
 * Last updated March 6, 2020
 */

#include "hw9.h"

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

commit_t *create_commit(char *name, float time, char *hash) {
  assert((name) && ((time >= 0.0) && (time < 24.0)) && (hash));

  entry_t *entry = NULL;
  entry = malloc(sizeof(entry_t));
  assert(entry);
  char *author_name = NULL;
  author_name = malloc(strlen(name) + 1);
  assert(author_name);
  entry->author = author_name;
  strcpy(entry->author, name);
  entry->time = time;
  char *hash_string = NULL;
  hash_string = malloc(strlen(hash) + 1);
  assert(hash_string);
  entry->hash = hash_string;
  strcpy(entry->hash, hash);

  commit_t *commit = NULL;
  commit = malloc(sizeof(commit_t));
  assert(commit);
  commit->next_commit = NULL;
  commit->prev_commit = NULL;
  commit->data = entry;

  return commit;
} /* create_commit() */

commit_t *find_commit(commit_t *head, char *hash_key) {
  assert((head) && (hash_key));
  commit_t *current = head;
  commit_t *temp = NULL;
  while (current) {
    if (strcmp(current->data->hash, hash_key) == 0) {
      temp = current;
      break;
    }
    current = current->next_commit;
  }
  return temp;
} /* find_commit() */

void insert_commit(commit_t *head, commit_t *insert_node, char *hash) {
  assert((head) && (insert_node) && (hash));
  if (strlen(hash) == 0) {
    insert_node->next_commit = head;
    insert_node->next_commit->prev_commit = insert_node;
    insert_node->prev_commit = NULL;
    head = insert_node;
    return;
  }
  else {
    commit_t *current = head;
    while (current) {
      /* traverse until hash is found in list */

      if (strcmp(current->data->hash, hash) == 0) {
        if (current->next_commit) {
          current = current->next_commit;
          if (current->prev_commit) {
            current->prev_commit->next_commit = insert_node;
          }
          insert_node->prev_commit = current->prev_commit;
          insert_node->next_commit = current;
          current->prev_commit = insert_node;
          return;
        }
        else {
          insert_node->prev_commit = current;
          insert_node->next_commit = NULL;
          insert_node->prev_commit->next_commit = insert_node;
          return;
        }
      }
      current = current->next_commit;
    }

    /* end of list reached, hash not found in list */

    insert_node->next_commit = head;
    insert_node->next_commit->prev_commit = insert_node;
    insert_node->prev_commit = NULL;
    head = insert_node;
  }
} /* insert_commit() */

commit_t *remove_commit(commit_t *head, char *hash) {
  assert((head) && (hash));
  commit_t *removed_node = NULL;
  commit_t *current = head;
  while (current) {
    if (strcmp(current->data->hash, hash) == 0) {
      removed_node = current;
      if (current->next_commit) {
        current->next_commit->prev_commit = current->prev_commit;
      }
      if (current->prev_commit) {
        current->prev_commit->next_commit = current->next_commit;
      }

      current->next_commit = NULL;
      current->prev_commit = NULL;
      break;
    }
    current = current->next_commit;
  }
  return removed_node;
} /* remove_commit() */

int remove_all_commits_by_author(commit_t *head, char *author_name) {
  assert((head) && (author_name));
  int nodes_deleted = 0;
  int was_removed = false;
  commit_t *current = head;
  while (current) {
    if (strcmp(current->data->author, author_name) == 0) {
      nodes_deleted++;
      was_removed = true;
      if (current->next_commit) {
        current->next_commit->prev_commit = current->prev_commit;
      }
      if (current->prev_commit) {
        current->prev_commit->next_commit = current->next_commit;
      }

      current->prev_commit = NULL;
    }

    commit_t *temp = current;
    current = current->next_commit;
    if (was_removed) {
      temp->next_commit = NULL;
      was_removed = false;
    }
  }
  return nodes_deleted;
} /* remove_all_commits_by_author() */

commit_t *repair_log(commit_t *head, commit_t *tail) {
  assert((head) && (tail));
  commit_t *head_copy = head;
  commit_t *tail_copy = tail;

  while (head != tail) {
    if (!head->next_commit) {
      while (tail) {
        if (tail->prev_commit == head) {
          head->next_commit = tail;
          return head;
        }
        else {
          tail = tail->prev_commit;
        }
      }
    }
    head = head->next_commit;
  }

  head = head_copy;
  tail = tail_copy;
  while (tail != head) {
    if (!tail->prev_commit) {
      while (head) {
        if (head->next_commit == tail) {
          tail->prev_commit = head;
          return tail;
        }
        else {
          head = head->next_commit;
        }
      }
    }
    tail = tail->prev_commit;
  }

  return NULL;
} /* repair_log() */

commit_t *disconnect_loop(commit_t *head) {
  assert(head);
  commit_t *current = head;
  if (current->next_commit == head) {
    current->next_commit = NULL;
    return current;
  }/*
  commit_t *fast_ptr = head->next_commit;
  while (current) {
    if (current->next_commit == head) {
      current->next_commit = NULL;
      if (head->prev_commit) {
        head->prev_commit = NULL;
      }
      return current;
    }

    current = current->next_commit;
    if (current->next_commit) {
      fast_ptr = current->next_commit;
    }

    if (current == fast_ptr) {
      head->prev_commit->next_commit = NULL;
      commit_t *temp = head->prev_commit;
      head->prev_commit = NULL;
      return temp;
    }
  }*/
  return NULL;
} /* disconnect_loop() */

void free_commit(commit_t *commit) {
  if (commit) {
    if ((!commit->next_commit) || (commit == commit->next_commit)) {
      free(commit->data->author);
      commit->data->author = NULL;
      free(commit->data->hash);
      commit->data->hash = NULL;
      free(commit->data);
      commit->data = NULL;
      free(commit);
      commit = NULL;
    }
  }
} /* free_commit() */
