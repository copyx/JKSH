#include "util.h"
#include <sys/types.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "builtin.h"
/**
 * @fn char *get_username()
 * @brief
 * Get username from passwd.
 *
 * @return user name string
 */
char *get_Username(uid_t uid)
{
	struct passwd *pwd = getpwuid(uid);

	return pwd->pw_name;
}

char *get_Groupname(gid_t gid)
{
	struct group *grp = getgrgid(gid);

	return grp->gr_name;
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

	username = get_Username(getuid());
	hostname = get_Hostname();

	path_len = pathconf("/", _PC_PATH_MAX);
	cwd = getcwd((char *)NULL, path_len);

	printf("%s@%s:%s$ ", username, hostname, cwd);

	free(hostname);
	free(cwd);
}

/**
 * @fn char *split_Command(char *command)
 * @brief
 * Split string by ' ' and make two demension array to point each token.
 * This function return the number of tokens.
 * Its memory have to be released.
 */
int split_Command(char *command, char ***tokens)
{
	int nTok = 0, i = 0;
	char *pCmd, *last;

	last = command + strlen(command) - 1;
	*last = '\0';
	pCmd = command;

	/* Count the number of tokens */
	while (pCmd < last) {
		while (isspace(*pCmd) != 0)
			pCmd++;
		if (*pCmd == '\"') {
			if ((pCmd = strchr(pCmd + 1, '\"')) == NULL)
				return -1;
			*pCmd = '\0';
			++nTok;
		} else if (*pCmd != '\0') {
			while (!isspace(*pCmd))
				++pCmd;
			*pCmd = '\0';
			++nTok;
		}
		++pCmd;
	}

	*tokens = (char **)malloc(sizeof(char *) * nTok);

	pCmd = command;

	/* Store the pointer of tokens */
	while (pCmd < last) {
		while (isspace(*pCmd) != 0)
			pCmd++;
		if (*pCmd == '\"')
			++pCmd;
		(*tokens)[i++] = pCmd;
		pCmd = strchr(pCmd, '\0');
		++pCmd;
	}

	return nTok;
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
	char **argv = NULL;
	int argc = split_Command(command, &argv);
	int cmd_len = strlen(argv[0]);


	if (argc > 0) {
		if (cmd_len == 2) {
			if (!strcmp(argv[0], "ls")) {
				ls(argc, argv);
			}
		} else if (cmd_len == 4) {
			if (!strcmp(argv[0], "echo")) {
				echo(argc, argv);
			}
		} else if (cmd_len == 6) {
			if (!strcmp(argv[0], "whoami"))
				whoami(argc, argv);
			else if (!strcmp(argv[0], "export"))
				export(argc, argv);
		} else {
			printf("There is no command\n");
		}
	} else if (argc < 0) {
		printf("Syntax error\n");
	}

	free(argv);
}
