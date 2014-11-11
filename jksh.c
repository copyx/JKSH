/**
 * @file jksh.c
 * @brief
 * This is my own shell.
 *
 * The prompt should be "(username)@(hostname):(cwd)$".
 * Implement the following built-in functions.
 * 	- echo
 * 	- whoami
 * 	- export
 */
#include <sys/types.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

int main()
{
	char *username, *hostname, *cwd;
	char input_buf[4096];
	long path_len;

	username = get_Username();
	hostname = get_Hostname();

	path_len = pathconf("/", _PC_PATH_MAX);
	cwd = getcwd((char *)NULL, path_len);

	while (1) {
		/* print prompt */
		printf("%s@%s:%s$ ", username, hostname, cwd);
		/* read command */
		if (fgets(input_buf, 4096, stdin))
			/* process command */
			handle_Command(input_buf);
		else if (ferror((struct FILE *)input_buf))
			printf("Input buffer error occured.\n");
		break;
	}

	free(username);
	free(hostname);
	free(cwd);

	return 0;
}
