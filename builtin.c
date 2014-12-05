#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

/**
 * @fn void echo()
 * @brief
 * Print string on STDOUT.
 */
void echo(char *opt)
{
	printf("%s", opt);
}

/**
 * @fn void whoami()
 * @brief
 * Print current user's name.
 */
void whoami()
{
	char *username = get_Username();
	printf("%s\n", username);
	free(username);

}

void export(char *opt)
{
	/* fnp */
	// while ((opt = getopt()))
}
