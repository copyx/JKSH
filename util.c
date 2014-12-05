#include "util.h"
#include <sys/types.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtin.h"
/**
 * @fn char *get_username()
 * @brief
 * Get username from passwd.
 *
 * @return user name string
 */
char *get_Username()
{
	struct passwd *pwd = getpwuid(getuid());

	return strdup(pwd->pw_name);
}

/**
 * @fn char *get_hostname()
 * @brief
 * Get hostname from system information.
 *
 * @return host name string
 */
char *get_Hostname()
{
	struct utsname uts;

	if (uname(&uts) == -1) {
		perror("uname");
		exit(1);
	}

	return strdup(uts.nodename);
}

/**
 * @fn void print_Prompt()
 * @brief
 * Print prompt string.
 */
void print_Prompt()
{
	char *username, *hostname, *cwd;
	long path_len;

	username = get_Username();
	hostname = get_Hostname();

	path_len = pathconf("/", _PC_PATH_MAX);
	cwd = getcwd((char *)NULL, path_len);

	printf("%s@%s:%s$ ", username, hostname, cwd);

	free(username);
	free(hostname);
	free(cwd);
}

/**
 * @fn void handle_Command
 * @brief
 * Built-in functions
 * 	- echo
 * 	- whoami
 * 	- export
 */
void handle_Command(char *command)
{
	char *cmd_tok = strtok(command, " \n");
	char *opt_tok = command + strlen(cmd_tok) + 1;

	if (!strcmp(cmd_tok, "echo")) {
		echo(opt_tok);
	} else if (!strcmp(cmd_tok, "whoami")) {
		whoami();
	} else if (!strcmp(cmd_tok, "export")) {
	} else {
	}
}
