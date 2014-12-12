/**
 * @file jksh.c
 * @brief
 * This is my own shell.
 */
#include <stdio.h>
#include "util.h"

int main()
{
	char command[4096];

	while (1) {
		/* print prompt */
		print_Prompt();

		/* read command */
		if (fgets(command, 4096, stdin) != NULL)
			/* process command */
			handle_Command(command);
	}

	return 0;
}
