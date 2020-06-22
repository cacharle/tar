#include "tar.h"

int file_content_write(int fd, int file_fd, struct stat *statbuf)
{
	char	*content;

	if ((content = malloc(statbuf->st_size)) == NULL)
	{
		perror("file_content_write");
		close(file_fd);
		return -1;
	}
	if (read(file_fd, content, statbuf->st_size) == -1)
	{
		perror("file_content_write");
		close(file_fd);
		free(content);
		return -1;
	}
	int	ret = record_write(fd, content, statbuf->st_size);
	close(file_fd);
	free(content);
	return (ret);
}

int file_write(int fd, char file_name[PATH_MAX])
{
	int			file_fd;
	struct stat	statbuf;

	fprintf(stderr, "|%s|\n", file_name);
	if (stat(file_name, &statbuf) == -1)
	{
		perror("file_write stat");
		return -1;
	}
	if (header_write(fd, file_name, &statbuf) == -1)
		return -1;

	switch (statbuf.st_mode & S_IFMT)
	{
		case S_IFDIR:
			return directory_write(fd, file_name);
		default:
			if ((file_fd = open(file_name, O_RDONLY)) == -1)
			{
				perror("file_write open");
				return -1;
			}
			return file_content_write(fd, file_fd, &statbuf);
	}
	return (0);
}

int directory_write(int fd, char dir_name[PATH_MAX])
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			len;

	if ((dir = opendir(dir_name)) == NULL)
	{
		perror("directory_write");
		return -1;
	}
	len = strlen(dir_name);
	strcat(dir_name, "/");
	len++;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0)
			continue;
		strcat(dir_name, entry->d_name);
		if (file_write(fd, dir_name) == -1)
		{
			closedir(dir);
			return -1;
		}
		dir_name[len] = '\0';
	}
	dir_name[len - 1] = '\0';
	closedir(dir);
	return 0;
}

/* int	file_content_read(int archive_fd,  */
