#include "tar.h"

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

	// pipe to fd
	printf("out: %s\n", output_file_name);
	char	**files = argv + optind;
	for (; *files != NULL; files++)
	{
		// stat file
		// write header to pipe
		// if dir
		//     recursion on files in dir
		// else
		//     write content to pipe
		printf("%s\n", *files);
	}
	return 0;
}
