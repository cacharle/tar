#ifndef TAR_H
# define TAR_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <getopt.h>
# include <fcntl.h>

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
	char	link_indicator[1];
	char	link_file_name[100];
}			t_header;


#endif // TAR_H
