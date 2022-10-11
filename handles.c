#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "handles.h"

void setinfo(struct file_info *info, const char *pathname, size_t len)
{
	FILE *file;
	struct stat sb;

	if (!(file = fopen(pathname, "r"))) {
		die("fopen");
	}

	fseek(file, 0L, SEEK_END);
	info->fpos = ftell(file);
	fclose(file);

	if (stat(pathname, &sb) == -1) {
		die("stat");
	}

	if (!(info->pathname = malloc(len + 1))) {
		die("malloc");
	}
	strncpy(info->pathname, pathname, len + 1);
	info->size = sb.st_size;
}

void printlines(struct file_info *info)
{
	FILE *file;
	struct stat sb;
	char *line = NULL;
	size_t size;
	ssize_t nread;

	if (!(file = fopen(info->pathname, "r"))) {
		die("fopen");
	}
	if (stat(info->pathname, &sb) == -1) {
		die("stat");
	}

	/* If file is truncated, reset stored file size and fpos to keep
	 * printing lines next time more data is added
	 */
	if (sb.st_size < info->size) {
		info->size = sb.st_size;
		fseek(file, 0L, SEEK_END);
		info->fpos = ftell(file);
	} else {
		fseek(file, info->fpos, SEEK_SET);
		while ((nread = getline(&line, &size, file)) != -1) {
			printf("%s", line);
		}

		info->fpos = ftell(file);
	}
	if (line) {
		free(line);
	}
	fclose(file);
}
