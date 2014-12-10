#include "export.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "version.h"

static char *help = "export: export [-fn] [name[=value] ...] or export -p\n"
	"Set export attribute for shell variables.\n\n"
	"Options:\n"
	"      -n	remove the export property from each NAME\n"
	"      --help     Print help script\n"
	"      --version  Print version\n";

void export(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt, print = 0, remove = 0;
	char *name, *value;
	extern int optind;
	extern int optind, opterr, optopt;
	optind = 0;

	while ((opt = getopt_long(argc, argv, "n:p", options, NULL)) != -1) {
		switch (opt) {
		case 'h':	/* help */
			printf("%s", help);
			return;


		case 'v': /* version */
			printf("export (JKsh coreutils) 0.1.0\n");
			printf("%s\n", copyright);
			return;

			/*
		case 'f':	 Refer to shell functions
			break;
			*/

		case 'n':	/* Remove */
			remove = 1;
			name = optarg;
			break;

		case 'p':	/* Print */
			print = 1;
			break;

		case '?':
			printf("Error: %c", optopt);
			printf("%s\n", argv[optind - 1]);
			return;
		}
	}

	if (remove) {
		unsetenv(name);
	} /*else if (print || argc == 1) {
		 export
		printf("Not implemented\n");
	}*/ else {
		name = strtok(argv[1], "=");
		value = name + strlen(name) + 1;
		setenv(name, value, 1);
	}
}
