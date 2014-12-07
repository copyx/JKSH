#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "util.h"

const char copyright[] = "Copyright (C) 2014 J. K. Kim <copy_x@naver.com>";

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
 * Print string on STDOUT.
 */
void echo(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 0},
		{"version", 0, 0, 0}
	};
	int opt, index, newline = 1, escapes = 0;
	int cmd_cnt = 0;

	optind = 1;

	while ((opt = getopt_long(argc, argv, "neE", options, &index)) != -1) {
		++cmd_cnt;
		switch (opt) {
		case 0:
			switch (index) {
			case 0:	/* help */
				printf("echo [SHORT-OPTION]... [STRING]...\n"
					"echo LONG-OPTION\n"
					"-n\tdo not output the trailing"
					" newline\n"
					"-e\tenable interpretation of"
					" backslash escapes\n"
					"-E\tdisable interpretation of"
					" backslash escapes (default)\n");
				return;


			case 1: /* version */
				printf("echo (JKsh coreutils) 0.1.0\n");
				printf("%s\n", copyright);
				return;
			}
			break;

		case 'n':
			newline = 0;
			break;

		case 'e':
			escapes = 1;
			break;

		case 'E':
			escapes = 0;
			break;
		}
	}

	if (escapes)
		replace_Escapes(argv[optind]);

	while (++cmd_cnt < argc) {
		printf("%s", argv[cmd_cnt]);
		//cmd_cnt++;
	}

	if (newline)
		printf("\n");
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

void export(int argc, char **argv)
{
	int opt;
	/* fnp */
	while ((opt = getopt(argc, argv, "a:b:c:")) != -1) {
	}
}
