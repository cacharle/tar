#include "tar.h"

// only first hard link
// checksum positive and negative bytes

int main(int argc, char **argv)
{
	t_args	args;
	int		archive_fd;
	int		ret;

	if (!args_parse(argc, argv, &args) ||
		(archive_fd = archive_get_fd(&args)) == -1)
		return EXIT_FAILURE;

	ret = archive_dispatch_action(archive_fd, &args);
	if (archive_fd != STDOUT_FILENO && archive_fd != STDIN_FILENO)
		close(archive_fd);
	return ret == -1 ? 1 : 0;
}
