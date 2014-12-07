#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "util.h"

const char *copyright = "Copyright (C) 2014 J. K. Kim <copy_x@naver.com>";

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
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt, newline = 1, escapes = 0;
	int cmd_cnt = 0;

	extern int optind;
	optind = 1;

	while ((opt = getopt_long(argc, argv, "neE", options, NULL)) != -1) {
		++cmd_cnt;
		switch (opt) {
		case 'h':	/* help */
			printf("Write arguments to"
				" the standard output.\n\n"
				"echo [SHORT-OPTION]... [STRING]...\n"
				"echo LONG-OPTION\n\n"
				"-n    do not output the trailing"
				" newline\n"
				"-e    enable interpretation of"
				" backslash escapes\n"
				"-E    disable interpretation of"
				" backslash escapes (default)\n\n"
				"--help     Print help script\n"
				"--version  Print version\n");
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
			printf("Error\n");
			printf("%s\n", argv[1]);
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

/**
 * @fn void whoami()
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
	extern int optind, opterr, optopt;

	optind = 1;

	while ((opt = getopt_long_only(argc, argv, "", options, NULL)) != -1) {
		switch (opt) {
		case 'h':
			printf("Print the user name associated with"
				" the current effective user ID.\n\n"
				"--help     Print help script\n"
				"--version  Print version\n");
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

	username = get_Username();
	printf("%s\n", username);
	free(username);
}

void export(int argc, char **argv)
{
	int opt, print = 0, remove = 0;
	char *name, *value;
	extern int optind;
	optind = 1;

	while ((opt = getopt(argc, argv, "fn:p")) != -1) {
		switch (opt) {
		case 'f':	/* Refer to shell functions */
			break;

		case 'n':	/* Remove */
			remove = 1;
			name = optarg;
			break;

		case 'p':	/* Print */
			print = 1;
			break;

		case '?':
			printf("Error\n");
			printf("%s\n", argv[1]);
			return;
		}
	}

	if (remove) {
		unsetenv(name);
	} else if (print || argc == 1) {
	} else {
		name = strtok(argv[1], "=");
		value = name + strlen(name) + 1;
		setenv(name, value, 1);
	}
}
