#include "whoami.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "../util.h"
#include "../version.h"

static const char *help = "Print the user name associated with"
	" the current effective user ID.\n\n"
	"Options:\n"
	"--help     Print help script\n"
	"--version  Print version\n";

/**
 * @fn void whoami(int argc, char **argv)
 * @brief
 * Print current user's name.
 */
void whoami(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt;
	char *username;
	extern int optind, optopt;
	optind = 0;

	while ((opt = getopt_long(argc, argv, "hv", options, NULL)) != -1) {
		switch (opt) {
		case 'h':
			printf("%s", help);
			return;

		case 'v': /* version */
			printf("whoami (JKsh coreutils) 0.1.0\n");
			printf("%s\n", copyright);
			return;

		case '?':
			printf("Error: %c", optopt);
			printf("%s\n", argv[optind - 1]);
			return;
		}
	}

	username = get_Username(getuid());
	printf("%s\n", username);
}
