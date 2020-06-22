#include "tar.h"

void		put_file_name(char *file_name)
{
	fputs(file_name, stderr);
	fputc('\n', stderr);
}

void		put_file_verbose(t_header *header, struct stat *statbuf)
{
	char	*time_str;
	char	*new_line_ptr;

	time_str = ctime(&statbuf->st_mtime);
	if ((new_line_ptr = strchr(time_str, '\n')) != NULL)
		*new_line_ptr = '\0';
	printf(
		"%c%c%c%c%c%c%c%c%c%c %s/%s %6lu %s %s\n",
		header->file_type[0] == '0' ? '-' : 'd',
		statbuf->st_mode & S_IRUSR ? 'r' : '-',
		statbuf->st_mode & S_IWUSR ? 'w' : '-',
		statbuf->st_mode & S_IXUSR ? 'x' : '-',
		statbuf->st_mode & S_IRGRP ? 'r' : '-',
		statbuf->st_mode & S_IWGRP ? 'w' : '-',
		statbuf->st_mode & S_IXGRP ? 'x' : '-',
		statbuf->st_mode & S_IROTH ? 'r' : '-',
		statbuf->st_mode & S_IWOTH ? 'w' : '-',
		statbuf->st_mode & S_IXOTH ? 'x' : '-',
		header->user_name,
		header->group_name,
		statbuf->st_size,
		time_str,
		header->file_name
	);
}
