/**
 * @file jksh.c
 * @brief
 * This is my own shell.
 */
#include <stdio.h>
#include "util.h"

int main()
{
//	char input_buf[4096];
	char *command;
	size_t len = 0;

	while (1) {
		/* print prompt */
		print_Prompt();

		/* read command */
//
//		if (fgets(input_buf, 4096, stdin))
//			/* process command */
//			handle_Command(input_buf);
//
		if (getline(&command, &len, stdin) != -1)
			/* process command */
			handle_Command(command);

//		else if (ferror((struct FILE *)input_buf))
//			printf("Input buffer error occured.\n");
	}

	return 0;
}
