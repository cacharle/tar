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
	int	fd;

	if ((fd = open(archive_file_name, O_RDONLY)) == -1)
	{
		perror("achive_write");
		return -1;
	}

	int			ret;
	char		buf[RECORD_SIZE];
	t_header	header;
	int			current_fd;

	while ((ret = read(fd, buf, RECORD_SIZE)) == RECORD_SIZE)
	{
		/* header_parse(&header, buf); */
		/* current_fd = open(header.file_name, O_WRONLY | O_CREAT | O_TRUNC, header. */
		// read content
		// TODO
	}
	if (ret == -1)
	{
		perror(NULL);
		return -1;
	}
	if (ret < RECORD_SIZE)
		return -1;
	close(fd);
	return 0;
}
