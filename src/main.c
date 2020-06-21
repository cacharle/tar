#include "tar.h"

// only first hard link
// checksum positive and negative bytes

int main(int argc, char **argv)
{
	int		opt;
	char	*archive_file_name = NULL;
	bool	create;

	while ((opt = getopt(argc, argv, "Oxcvtf:")) != -1)
	{
		switch (opt)
		{
			case 'c':
				create = true;
				break;
			case 'x':
				create = false;
				break;
			case 'f':
				archive_file_name = optarg;
				break;
			case 'v':
				break;
			case 't':
				break;
			default:
				return 1;
		}
	}
	if (create)
	{
		if (archive_write(archive_file_name, argv + optind) == -1)
			return 1;
	}
	else
	{
		if (archive_read(archive_file_name) == -1)
			return 1;
	}
	return 0;
}
