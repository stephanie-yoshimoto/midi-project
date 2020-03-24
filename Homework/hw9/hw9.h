#ifndef HW9_H
#define HW9_H

/* Error codes */

/* typedefs */

typedef struct entry {
  char *author;
  float time;
  char *hash;
} entry_t;

typedef struct commit {
  entry_t *data;
  struct commit *next_commit;
  struct commit *prev_commit;
} commit_t;

/* Function prototypes */

commit_t *create_commit(char *, float, char *);
commit_t *find_commit(commit_t *, char *);
void insert_commit(commit_t *, commit_t *, char *);
commit_t *remove_commit(commit_t *, char *);
int remove_all_commits_by_author(commit_t *, char *);
commit_t *repair_log(commit_t *, commit_t *);
commit_t *disconnect_loop(commit_t *);
void free_commit(commit_t *);

#endif // HW9_H
