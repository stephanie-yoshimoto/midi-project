#ifndef HW6_H
#define HW6_H

/* Constant definitions */
#define MAX_PASSWORDS (50)
#define MAX_NAME_LEN (10)
#define MAX_BUFF_LEN (50)

typedef struct password {
  char code_name[MAX_NAME_LEN];
  char passcode_name[MAX_NAME_LEN];
  char passcode_value[MAX_NAME_LEN];
} password_t;

/* Error codes */
#define NON_READABLE_FILE (-1)
#define BAD_RECORD (-2)
#define TOO_MUCH_DATA (-3)

/* Function prototypes */
int read_passwords(char *);
int locate_spies(char **, int);

/* Global variables */
extern int g_password_count;
extern password_t g_password_array[MAX_PASSWORDS];

#endif // HW5_H
