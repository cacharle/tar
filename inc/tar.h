#ifndef TAR_H
# define TAR_H

# include <sys/types.h>
# include <sys/stat.h>

# ifndef S_IFMT
#  define S_IFMT __S_IFMT
# endif
# ifndef S_IFDIR
#  define S_IFDIR __S_IFDIR
# endif
# ifndef S_IFREG
#  define S_IFREG __S_IFREG
# endif

# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <getopt.h>
# include <fcntl.h>
# include <dirent.h>
# include <stdio.h>
# include <stdlib.h>
# include <strings.h>
# include <string.h>

# include <grp.h>
# include <pwd.h>
# include <time.h>

# include <limits.h>
# ifdef __linux__
#  include <linux/limits.h>
# endif

# define RECORD_SIZE 512

# define DEFAULT_UMASK 0644

// # define FILE_NAME_MAX 100

// https://en.wikipedia.org/wiki/Tar_(computing)?oldformat=true#Header
typedef struct
{
	char	file_name[100];
	char	file_mode[8];
	char	user_id[8];
	char	group_id[8];
	char	file_size[12];
	char	last_time[12];   // utime
	char	checksum[8];
	char	file_type[1];
	char	link_file_name[100];
	char	ustar[6];
	char	ustar_version[2];
	char	user_name[32];
	char	group_name[32];
	char	device_major_number[8];
	char	device_minor_number[8];
	char	file_name_prefix[155];
}				t_header;

typedef enum
{
	ACTION_CONCAT,
	ACTION_CREATE,
	ACTION_DIFF,
	ACTION_LIST,
	ACTION_APPEND,
	ACTION_UPDATE,
	ACTION_EXTRACT,
}				t_action;

typedef enum
{
	FLAG_VERBOSE = 1 << 0,
	FLAG_FILE    = 1 << 1,
}				t_flags;

typedef struct
{
	char		*archive_name;
	char		**files;
	t_action	action;
	t_flags		flags;
}				t_args;

// header.c
int			header_write(int fd, char *file_name, struct stat *statbuf);
int			header_parse(char record[RECORD_SIZE], struct stat *statbuf, t_header *header);

// fs.c
int 		file_content_write(int fd, int file_fd, struct stat *statbuf);
int 		file_write(int fd, char file_name[PATH_MAX], bool verbose);
int 		directory_write(int fd, char dir_name[PATH_MAX], bool verbose);

// archive.c
int			archive_dispatch_action(int archive_fd, t_args *args);
int			archive_create(int archive_fd, char **files, bool verbose);
int			archive_extract(int archive_fd, bool verbose);
int			archive_get_fd(t_args *args);

// record.c
int			record_write(int fd, char *s, size_t size);
int			record_write_blank(int fd, size_t count);
int			record_read(int fd, char record[RECORD_SIZE]);
bool		record_is_blank(char record[RECORD_SIZE]);

// args.c
bool		args_parse(int argc, char **argv, t_args *args);

// utils.c
void		put_file_name(char *file_name);
void		put_file_verbose(t_header *header, struct stat *statbuf);

#endif // TAR_H
