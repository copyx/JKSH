#ifndef __UTIL_H__
#define __UTIL_H__

#include <unistd.h>

char *get_Username(uid_t);
char *get_Groupname(gid_t);
char *get_Hostname();
void print_Prompt();
void handle_Command(char *);

#endif
