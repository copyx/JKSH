/**
 * @file jksh.c
 * @brief
 * This is my own shell.
 */
#include <stdio.h>
#include "util.h"

int main()
{
	char *command;
	size_t len = 0;

	while (1) {
		/* print prompt */
		print_Prompt();

		/* read command */

		if (getline(&command, &len, stdin) != -1)
			/* process command */
			handle_Command(command);
	}

	return 0;
}
