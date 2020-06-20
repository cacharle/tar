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

# include <limits.h>
# ifdef __linux__
#  include <linux/limits.h>
# endif

# define RECORD_SIZE 512

// # define FILE_NAME_MAX 100

// https://en.wikipedia.org/wiki/Tar_(computing)?oldformat=true#Header
typedef struct
{
	char	file_name[100];
	char	file_mode[8];
	char	user_id[8];
	char	group_id[8];
	char	file_size[12];
	char	last_time[12];
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
}			t_header;

// header.c
int	record_write(int fd, char *s, size_t size);
int	header_write(int fd, char *file_name, struct stat *statbuf);

// fs.c
int file_content_write(int fd, int file_fd, struct stat *statbuf);
int file_write(int fd, char file_name[PATH_MAX]);
int directory_write(int fd, char dir_name[PATH_MAX]);

#endif // TAR_H
