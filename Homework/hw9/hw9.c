/* Stephanie Yoshimoto, hw9.c, CS 24000, Spring 2020
 * Last updated March 6, 2020
 */

#include "hw9.h"

commit_t *create_commit(char *name, float time, char *hash) {
  return NULL;
} /* create_commit() */

commit_t *find_commit(commit_t *head, char *hash_key) {
  return NULL;
} /* find_commit() */

void insert_commit(commit_t *head, commit_t *insert_node, char *hash) {

} /* insert_commit() */

commit_t *remove_commit(commit_t *head, char *hash) {
  return NULL;
} /* remove_commit() */

int remove_all_commits_by_author(commit_t *head, char *author_name) {
  return 0;
} /* remove_all_commits_by_author() */

commit_t *repair_log(commit_t *head, commit_t *tail) {
  return NULL;
} /* repair_log() */

commit_t *disconnect_loop(commit_t *head) {
  return NULL;
} /* disconnect_loop() */

void free_commit(commit_t *entry) {
  if (entry) {
    free(data->author);
    data->author = NULL;
    free(data->hash);
    data->hash = NULL;
    commit_t *temp = entry;
    while (entry->next_commit) {
      /* free next nodes */

      entry = entry->next_commit;
      free(data->author);
      data->author = NULL;
      free(data->hash);
      data->hash = NULL;
      entry->prev_commit = NULL;
    }
    while (temp->prev_commit) {
      /* free previous nodes */

      entry = entry->prev_commit;
      free(data->author);
      data->author = NULL;
      free(data->hash);
      data->hash = NULL;
      entry->next_commit = NULL;
    }
  }
} /* free_commit() */
