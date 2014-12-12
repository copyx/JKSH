#include "util.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
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
char **split_Command(char *command, int *argc)
{
	int i = 0;
	char *pCmd, *last, **argv;
	*argc = 0;

	last = command + strlen(command) - 1;
	pCmd = command;

	/* Count the number of tokens */
	while (pCmd < last) {
		while (isspace(*pCmd))
			pCmd++;
		if (*pCmd == '\"') {
			if ((pCmd = strchr(pCmd + 1, '\"')) == NULL)
				return NULL;
			*pCmd = '\0';
			++*argc;
		} else if (*pCmd != '\0') {
			while (!isspace(*pCmd))
				++pCmd;
			*pCmd = '\0';
			++*argc;
		}
		++pCmd;
	}

	argv = (char **)malloc(sizeof(char *) * *argc + 1);

	pCmd = command;

	/* Store the pointer of tokens */
	while (pCmd < last) {
		while (isspace(*pCmd) && pCmd == '\0')
			pCmd++;
		if (*pCmd == '\"')
			++pCmd;
		argv[i++] = strndup(pCmd, strlen(pCmd) + 1);
		pCmd = strchr(pCmd, '\0');
		++pCmd;
	}
	argv[*argc] = NULL;

	return argv;
}

/**
 * @fn void execute_External(int argc, char **argv)
 * @brief
 * Check if external command is exist and execute it.
 */
void execute_External(char **argv)
{
	pid_t pid;
	int status;

	switch (pid = fork()) {
	case -1:
		perror("fork");
		break;
	case 0:
		execvp(argv[0], argv);
		perror("execvp");
		exit(1);
	default:
		break;
	}

	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		return;
	}

	if (WIFSIGNALED(status))
		printf("Terminated by signal %d.\n", WTERMSIG(status));

	return;
}

/**
 * @fn void execute_Command(int argc, char **argv)
 * @brief
 * Check if command is builtin and execute it.
 */
void execute_Command(int argc, char **argv)
{
	if (!strcmp(argv[0], "ls"))
		ls(argc, argv);
	else if (!strcmp(argv[0], "cd"))
		cd(argc, argv);
	else if (!strcmp(argv[0], "pwd"))
		pwd(argc, argv);
	else if (!strcmp(argv[0], "echo"))
		echo(argc, argv);
	else if (!strcmp(argv[0], "whoami"))
		whoami(argc, argv);
	else if (!strcmp(argv[0], "export"))
		export(argc, argv);
	else
		execute_External(argv);
}

/**
 * @fn char **delete_LastArgv(char **argv)
 * @brief
 * Delete last element of argv
 */
char **delete_LastArgv(int *argc, char **argv)
{
	char **new;
	int i;

	--*argc;
	new = (char **)malloc(sizeof(char *) * *argc);

	for (i = 0; i < *argc; i++)
		new[i] = argv[i];

	free(argv);

	return new;
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
	int argc;
	char **argv = split_Command(command, &argc);
	pid_t pid;

	/* Check and execute command */
	if (argv != NULL) {
		/* Check background character */
		if (!strcmp(argv[argc - 1], "&")) {
			argv[--argc] = NULL;
			switch (pid = fork()) {
			case -1:
				perror("fork");
				exit(0);
			case 0:
				execute_Command(argc, argv);
				free(argv);
				exit(0);
			default:
				printf("[1] %d\n", pid);
				break;
			}
		} else {
			execute_Command(argc, argv);
		}
	}

	while (--argc >= 0)
		free(argv[argc]);
	free(argv);
}
