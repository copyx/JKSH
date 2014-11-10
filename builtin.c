#include "builtin.h"

/**
 * @fn void echo()
 * @brief
 * Print string on STDOUT.
 */
void echo()
{

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
	char *cmd_tok = strtok(command, " ");

	if (!strcmp(cmd_tok, "echo")) {
	} else if (!strcmp(cmd_tok, "whoami")) {
	} else if (!strcmp(cmd_tok, "export")) {
	} else {
	}
}
