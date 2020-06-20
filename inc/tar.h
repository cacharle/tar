#ifndef TAR_H
# define TAR_H

# include <sys/types.h>
# include <sys/stat.h>

# ifndef S_IFDIR
#  define S_IFDIR __S_IFDIR
# endif
# ifndef S_IFMT
#  define S_IFMT __S_IFMT
# endif

# include <unistd.h>
# include <stdbool.h>
# include <getopt.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <strings.h>
# include <string.h>


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


#endif // TAR_H
