#include "tar.h"

int	archive_create(int archive_fd, char **files, bool verbose)
{
	char		file_name[PATH_MAX];
	struct stat	root_statbuf;

	for (; *files != NULL; files++)
	{
		bzero(file_name, PATH_MAX);
		strcpy(file_name, *files);
		file_write(archive_fd, file_name, verbose);
	}
	if (stat("/", &root_statbuf) == -1)
	{
		perror("achive_write");
		return -1;
	}
	if (record_write_blank(archive_fd, 2 + (2 * root_statbuf.st_blksize) / RECORD_SIZE) == -1)
		return -1;
	return 0;
}

int			archive_extract(int archive_fd, bool verbose)
{
	char		record[RECORD_SIZE];
	t_header	header;
	int			fd;
	struct stat	statbuf;

	while (true)
	{
		if (record_read(archive_fd, record) == -1)
			return -1;
		if (record_is_blank(record))
		{
			if (record_read(archive_fd, record) == -1)
				return -1;
			if (record_is_blank(record))
				break;
			else
				return -1;
		}
		if (header_parse(record, &statbuf, &header) == -1)
			return -1;

		if (verbose)
			put_file_name(header.file_name);
		switch (header.file_type[0])
		{
			case '5':
				if (mkdir(header.file_name, statbuf.st_mode) == -1)
				{
					perror(NULL);
					return -1;
				}
				break;
			case '0':
				fd = open(header.file_name, O_WRONLY | O_CREAT | O_TRUNC, statbuf.st_mode);
				if (fd == -1)
				{
					perror(NULL);
					return -1;
				}
				char *content = malloc(statbuf.st_size);
				if (content == NULL ||
					read(archive_fd, content, statbuf.st_size) == -1 ||
					lseek(archive_fd, RECORD_SIZE - statbuf.st_size % RECORD_SIZE, SEEK_CUR) == -1 ||
					write(fd, content, statbuf.st_size) == -1)
				{
					perror(NULL);
					return -1;
				}
				break;
			default:
				return -1;
		}
	}
	return 0;
}

int			archive_list(int archive_fd, bool verbose)
{
	char		record[RECORD_SIZE];
	struct stat	statbuf;
	t_header	header;

	while (true)
	{
		if (record_read(archive_fd, record) == -1)
			return -1;
		if (record_is_blank(record))
		{
			if (record_read(archive_fd, record) == -1)
				return -1;
			if (record_is_blank(record))
				break;
			else
				return -1;
		}
		if (header_parse(record, &statbuf, &header) == -1)
			return -1;
		if (!verbose)
			put_file_name(header.file_name);
		else
			put_file_verbose(&header, &statbuf);

		if (header.file_type[0] == '0' &&
			lseek(archive_fd, statbuf.st_size + (RECORD_SIZE - statbuf.st_size % RECORD_SIZE), SEEK_CUR) == -1)
		{
			perror(NULL);
			return -1;
		}
	}
	return 0;
}

int	archive_get_fd(t_args *args)
{
	int		fd;
	bool	is_read;

	is_read = args->action == ACTION_DIFF ||
			  args->action == ACTION_LIST ||
			  args->action == ACTION_EXTRACT;
	if (args->archive_name == NULL)
		return is_read ? STDIN_FILENO : STDOUT_FILENO;
	if (is_read)
		fd = open(args->archive_name, O_RDONLY);
	else
		fd = open(args->archive_name, O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_UMASK);
	if (fd == -1)
	{
		perror("archive fd");
		return -1;
	}
	return fd;
}

int	archive_dispatch_action(int archive_fd, t_args *args)
{
	bool	verbose = args->flags & FLAG_VERBOSE;

	switch (args->action)
	{
		case ACTION_CREATE:
			return archive_create(archive_fd, args->files, verbose);
		case ACTION_EXTRACT:
			return archive_extract(archive_fd, verbose);
		case ACTION_LIST:
			return archive_list(archive_fd, verbose);
		default:
			return -1;
	}
	return 0;
}
