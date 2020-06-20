#include "tar.h"

// only first hard link
// checksum positive and negative bytes

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
			perror("main");
			return 1;
		}
	}

	/* printf("out: %s\n", output_file_name); */
	char	file_name[PATH_MAX];
	char	**files = argv + optind;
	for (; *files != NULL; files++)
	{
		/* printf("%s\n", *files); */
		bzero(file_name, PATH_MAX);
		strcpy(file_name, *files);
		file_write(fd, file_name);
	}
	char buf[512] = {'\0'};
	write(fd, buf, 512);
	write(fd, buf, 512);
	if (fd != STDOUT_FILENO)
		close(fd);
	return 0;
}
