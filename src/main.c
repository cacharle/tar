#include "tar.h"

// only first hard link
// checksum positive and negative bytes

int write_file(int fd, char *file_name)
{
	struct stat	statbuf;
	t_header	header;

	stat(file_name, &statbuf);
	bzero(&header, sizeof(t_header));
	strncpy(header.file_name, file_name, sizeof(header.file_name));
	sprintf(header.file_mode, "%07o", statbuf.st_mode & 0777);
	sprintf(header.user_id,   "%07o", statbuf.st_uid);
	sprintf(header.group_id,  "%07o", statbuf.st_gid);
	sprintf(header.file_size, "%011lo", statbuf.st_size);
	sprintf(header.last_time, "%011lo", statbuf.st_mtime);
	memset(header.checksum, ' ', sizeof(header.checksum));
	header.file_type[0] = '0';

	/* strncpy(header.ustar, "ustar", sizeof(header.ustar)); */
	char			*header_ptr = (char*)&header;
	unsigned int	sum = 0;
	for (size_t i = 0; i < sizeof(t_header); i++)
		sum += header_ptr[i];
	sprintf(header.checksum,  "%06o", sum);

	char buf[512] = {0};
	memcpy(buf, &header, sizeof(t_header));
	write(fd, buf, 512);


	int from;
	char *content;
	switch (statbuf.st_mode & S_IFMT)
	{
		case S_IFDIR: break;
		default:
			from = open(file_name, O_RDONLY);
			content = calloc(statbuf.st_size + 1, sizeof(char));
			read(from, content, statbuf.st_size);
			write(fd, content, statbuf.st_size);
			free(content);
			close(from);
			memset(buf, '\0', 512);
			write(fd, buf, 512 - statbuf.st_size % 512);
	}

	// if dir
	//    write_dir
	// else
	// 	  write content
	return (0);
}

int write_directory(int fd, char *dirname)
{
	// for f in files
	//     write_file
}

int main(int argc, char **argv)
{
	int		opt;
	char	*output_file_name = NULL;

	while ((opt = getopt(argc, argv, "cvtf:")) != -1)
	{
		switch (opt)
		{
			case 'c':
				break;
			case 'f':
				output_file_name = optarg;
				break;
			case 'v':
				break;
			case 't':
				break;
			default:
				return 1;
		}
	}
	int fd = -1;
	if (output_file_name == NULL)
		fd = STDOUT_FILENO;
	else
	{
		fd = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(NULL);
			return 1;
		}
	}

	/* printf("out: %s\n", output_file_name); */
	char	**files = argv + optind;
	for (; *files != NULL; files++)
	{
		/* printf("%s\n", *files); */
		write_file(fd, *files);
	}
	char buf[512] = {'\0'};
	write(fd, buf, 512);
	write(fd, buf, 512);
	if (fd != STDOUT_FILENO)
		close(fd);
	return 0;
}
