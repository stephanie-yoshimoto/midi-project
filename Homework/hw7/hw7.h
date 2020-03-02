#ifndef HW7_H
#define HW7_H

/* Constant definitions */
#define MAX_PASSWORDS (50)
#define MAX_NAME_LEN (10)
#define MAX_BUFF_LEN (50)

typedef struct password {
  char code_name[MAX_NAME_LEN];
  char passcode_name[MAX_NAME_LEN];
  char passcode_value[MAX_NAME_LEN];
  char next_member[MAX_NAME_LEN];

  struct password *next_member_ptr;
} password_t;

/* Error codes */
#define NON_READABLE_FILE (-1)
#define NO_PASSWORDS (-2)
#define BAD_RECORD (-3)
#define TOO_MUCH_DATA (-4)
#define NOT_A_SPY (-5)
#define MESSAGE_NOT_SENT (-6)

/* Function prototypes */
int read_passwords(char *);
void connect_members();
int isolate_spy(password_t *);
int send_message(password_t *, password_t *);

/* Global variables */
extern int g_password_count;
extern password_t g_password_array[MAX_PASSWORDS];

#endif // HW7_H
