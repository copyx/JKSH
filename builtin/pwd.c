#include "pwd.h"
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "../version.h"

static const char *help = "pwd\n"
	"Print the name of the current working directory.\n";

void pwd(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt, cmd_cnt = 0;
	extern int optind, optopt;
	char cwd[4096];

	optind = 0;

	while ((opt = getopt_long(argc, argv, "lhv", options, NULL)) != -1) {
		++cmd_cnt;
		switch (opt) {
		case 'h':
			printf("%s", help);
			return;

		case 'v': /* version */
			printf("pwd (JKsh coreutils) 0.1.0\n");
			printf("%s\n", copyright);
			return;

		case '?':
			printf("Error: %c", optopt);
			printf("%s\n", argv[optind - 1]);
			return;
		}
	}
	getcwd(cwd, 4096);
	printf("%s\n", cwd);
}
