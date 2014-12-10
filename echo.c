#include "echo.h"
#include <stdio.h>
#include <getopt.h>
#include "version.h"

static char *help = "echo: echo [-neE] [arg ...]\n"
	"Write arguments to the standard output.\n\n"
	"Options:\n"
	"      -n	do not append a newline\n"
	"      -e	enable interpretation of the following backslash escapes\n"
	"      -E	explicitly suppress interpretation of backslash escapes\n\n"
	"      --help     Print help script\n"
	"      --version  Print version\n";

/**
 * @fn void replace_Escapes(char *s)
 * @brief
 * Find escape character string and replace it to real escape character.
 */
void replace_Escapes(char *s)
{
	char *tmp, flag;
	while (*s != '\0') {
		if (*s == '\\'){
			flag = 1;
			switch (*(s + 1)) {
			case '\\':
				break;
			case 'a':
				*s = '\a';
				break;
			case 'b':
				*s = '\b';
				break;
				/*
			case 'c':
				*s = '\c';
				break;
				*/
			case 'e':
				*s = '\e';
				break;
			case 'f':
				*s = '\f';
				break;
			case 'n':
				*s = '\n';
				break;
			case 'r':
				*s = '\r';
				break;
			case 't':
				*s = '\t';
				break;
			case 'v':
				*s = '\v';
				break;
			default:
				flag = 0;
				break;
			}

			if (flag) {
				tmp = s + 1;
				while (*tmp++ != '\0')
					*(tmp - 1) = *tmp;
			}

		}
		++s;
	}
}

/**
 * @fn void echo()
 * @brief
 * Print string on STDOUT.157
 */
void echo(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt, newline = 1, escapes = 0;
	int cmd_cnt = 0;

	extern int optind, optopt;
	optind = 0;

	while ((opt = getopt_long(argc, argv, "neEhv", options, NULL)) != -1) {
		++cmd_cnt;
		switch (opt) {
		case 'h':	/* help */
			printf("%s", help);
			return;


		case 'v': /* version */
			printf("echo (JKsh coreutils) 0.1.0\n");
			printf("%s\n", copyright);
			return;

		case 'n':
			newline = 0;
			break;

		case 'e':
			escapes = 1;
			break;

		case 'E':
			escapes = 0;
			break;

		case '?':
			printf("Error: %c", optopt);
			printf("%s\n", argv[optind - 1]);
			return;
		}
	}

	while (++cmd_cnt < argc) {
		if (escapes)
			replace_Escapes(argv[cmd_cnt]);
		printf("%s", argv[cmd_cnt]);
		//cmd_cnt++;
	}

	if (newline)
		printf("\n");
}
