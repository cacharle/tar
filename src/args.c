#include "tar.h"

bool	args_parse(int argc, char **argv, t_args *args)
{
	int		opt;
	int		action_counter = 0;

	args->flags = 0;
	args->archive_name = NULL;
	args->files = NULL;
	while ((opt = getopt(argc, argv, "Acdtruxvf:")) != -1)
	{
		switch (opt)
		{
			case 'A': args->action = ACTION_CONCAT;  break;
			case 'c': args->action = ACTION_CREATE;  break;
			case 'd': args->action = ACTION_DIFF;    break;
			case 't': args->action = ACTION_LIST;    break;
			case 'r': args->action = ACTION_APPEND;  break;
			case 'u': args->action = ACTION_UPDATE;  break;
			case 'x': args->action = ACTION_EXTRACT; break;

			case 'v': args->flags |= FLAG_VERBOSE; break;
			case 'f':
				args->flags |= FLAG_FILE;
				args->archive_name = optarg;
				break;
			default:
				return false;
		}
		if (strchr("Acdtrux", opt) != NULL)
			action_counter++;
	}
	if (action_counter != 1)
	{
		fprintf(stderr, "%s: You way not specify more than one '-Acdtrux' option\n", argv[0]);
		return false;
	}
	if (!(args->flags & FLAG_FILE) && isatty(STDOUT_FILENO))
	{
		fprintf(stderr, "%s: Refusing to write archive contents to terminal (missing -f option?)\n", argv[0]);
		return false;
	}
	args->files = argv + optind;
	return true;
}
