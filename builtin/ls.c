#include "ls.h"
#include <time.h>
#include <stdio.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../util.h"
#include "../version.h"

static const char *help = "List information about the FILEs"
	" (the current directory by default).\n\n"
	"Options:\n"
	"      -l      Use a long listing format\n"
	"      --help     Print help script\n"
	"      --version  Print version\n";

void print_Stat(char *name)
{
	/* drwxr-xr-x  2 jk jk 4096 Oct 20 19:31 Desktop */
	struct stat sb;
	char *owner, *group;
	struct tm *t;

	stat(name, &sb);
	/* File type */
	switch (sb.st_mode & S_IFMT) {
	case S_IFREG: printf("-"); break;
	case S_IFDIR: printf("d"); break;
	case S_IFLNK: printf("l"); break;
	case S_IFIFO: printf("p"); break;
	case S_IFSOCK: printf("s"); break;
	case S_IFBLK: printf("b"); break;
	case S_IFCHR: printf("c"); break;
	default: printf("?"); break;
	}

	/* Permission */
	printf((sb.st_mode & S_IRUSR) ? "r" : "-");
	printf((sb.st_mode & S_IWUSR) ? "w" : "-");
	printf((sb.st_mode & S_ISUID) ? "s" :
		(sb.st_mode & S_IXUSR) ? "x" : "-");
	printf((sb.st_mode & S_IRGRP) ? "r" : "-");
	printf((sb.st_mode & S_IWGRP) ? "w" : "-");
	printf((sb.st_mode & S_ISGID) ? "s" :
		(sb.st_mode & S_IXGRP) ? "x" : "-");
	printf((sb.st_mode & S_IROTH) ? "r" : "-");
	printf((sb.st_mode & S_IWOTH) ? "w" : "-");
	printf((sb.st_mode & S_IXOTH) ? "x" : "-");
	printf((sb.st_mode & S_ISVTX) ? "t" : "");

	/* Number of links */
	printf(" %ld", sb.st_nlink);

	/* Owner */
	owner = get_Username(sb.st_uid);
	printf(" %s", owner);

	/* Group */
	group = get_Groupname(sb.st_gid);
	printf(" %s", group);

	/* Size */
	printf(" %7ld", sb.st_size);

	/* Time of last modification */
	t = localtime(&sb.st_mtime);
	/* month date hour:min */
	switch (t->tm_mon) {
	case 0: printf(" Jen"); break;
	case 1: printf(" Feb"); break;
	case 2: printf(" Mar"); break;
	case 3: printf(" Apr"); break;
	case 4: printf(" May"); break;
	case 5: printf(" Jun"); break;
	case 6: printf(" Jul"); break;
	case 7: printf(" Aug"); break;
	case 8: printf(" Sep"); break;
	case 9: printf(" Oct"); break;
	case 10: printf(" Nov"); break;
	case 11: printf(" Dec"); break;
	}
	printf(" %2d %02d:%02d ", t->tm_mday, t->tm_hour, t->tm_min);
}

int print_DirContents(char *path, int opt_l)
{
	DIR *dp;
	struct dirent *dent;
//	int total = 0;

	if ((dp = opendir(path)) == NULL) {
		perror("opendir");
		return -1;
	}

	while ((dent = readdir(dp))) {
		if (opt_l)
			print_Stat(dent->d_name);
		printf("%s", dent->d_name);
		if (opt_l)
			printf("\n");
		else
			printf(" ");
	}

	closedir(dp);
	return 0;
}

/**
 * @fn void ls(int argc, char **argv)
 * @brief
 * List information about the FILEs (the current directory by default).
 */
void ls(int argc, char **argv)
{
	struct option options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{0, 0, 0, 0}
	};
	int opt, cmd_cnt = 0, opt_long = 0;
	extern int optind, optopt;
	struct stat sb;

	optind = 0;

	while ((opt = getopt_long(argc, argv, "lhv", options, NULL)) != -1) {
		++cmd_cnt;
		switch (opt) {
		case 'h':
			printf("%s", help);
			return;

		case 'v': /* version */
			printf("ls (JKsh coreutils) 0.1.0\n");
			printf("%s\n", copyright);
			return;

		case 'l':
			opt_long = 1;
			break;

		case '?':
			printf("Error: %c", optopt);
			printf("%s\n", argv[optind - 1]);
			return;
		}
	}

	if (cmd_cnt == argc - 1)
		print_DirContents(".", opt_long);
	while (++cmd_cnt < argc) {
		if (stat(argv[cmd_cnt], &sb))
			printf("Cannot access to %s\n", argv[cmd_cnt]);
		else if (S_ISDIR(sb.st_mode)) {
			printf("\n%s:\n", argv[cmd_cnt]);
			print_DirContents(argv[cmd_cnt], opt_long);
			printf("\n");
		} else
			printf("%s\n", argv[cmd_cnt]);
	}
	printf("\n");
}
