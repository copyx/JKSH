#include "cd.h"
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "../version.h"

static const char *help = "cd [dir]\n"
	"Change the shell working directory.\n";

void cd(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt, cmd_cnt = 0;
	extern int optind, optopt;

	optind = 0;

	while ((opt = getopt_long(argc, argv, "lhv", options, NULL)) != -1) {
		++cmd_cnt;
		switch (opt) {
		case 'h':
			printf("%s", help);
			return;

		case 'v': /* version */
			printf("cd (JKsh coreutils) 0.1.0\n");
			printf("%s\n", copyright);
			return;

		case '?':
			printf("Error: %c", optopt);
			printf("%s\n", argv[optind - 1]);
			return;
		}
	}

	if (argc > 1)
		chdir(argv[1]);
}
