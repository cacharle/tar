#include "tar.h"

int	archive_write(char *archive_file_name, char **files)
{
	int			fd;
	char		file_name[PATH_MAX];
	struct stat	root_statbuf;

	if (archive_file_name == NULL)
		fd = STDOUT_FILENO;
	else
	{
		fd = open(archive_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("achive_write");
			return -1;
		}
	}
	for (; *files != NULL; files++)
	{
		bzero(file_name, PATH_MAX);
		strcpy(file_name, *files);
		file_write(fd, file_name);
	}
	if (stat("/", &root_statbuf) == -1)
	{
		perror("achive_write");
		close(fd);
		return -1;
	}
	if (record_write_blank(fd, 2 + (2 * root_statbuf.st_blksize) / RECORD_SIZE) == -1)
	{
		close(fd);
		return -1;
	}
	if (fd != STDOUT_FILENO)
		close(fd);
	return 0;
}

int			archive_read(char *archive_file_name)
{
	int	archive_fd;

	if (archive_file_name == NULL)
		archive_fd = STDIN_FILENO;
	else
	{
		if ((archive_fd = open(archive_file_name, O_RDONLY)) == -1)
		{
			perror("achive_write");
			return -1;
		}
	}

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

		switch (header.file_type[0])
		{
			case '5':
				if (mkdir(header.file_name, statbuf.st_mode) == -1)
				{
					close(archive_fd);
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
	close(archive_fd);
	return 0;
}
